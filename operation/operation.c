/*
 * operation.c
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#include "operation.h"

OP_STATE_t mcu_state;
OP_CTRL_FLAG_t controlFlag;


void OP_SetupConnection(OP_STATE_t *state, MISC_SYSTICK_PARAM_t *sysTickParam,
						OP_CTRL_FLAG_t *ctrlFlag, float pvVoltage, PCS_BMC_STATUS_t *pcsBmsStatus, PCS_STATUS_t *pcsStatus)
{
	static bool toggle_timer = false;
	/* 1. Ensure all relay at initial state */
	/* Initiate relay status */
	PDU_Switch_Relay(PDU_RELAY_KBAT, PDU_RELAY_OPEN);
	PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_OPEN);
	PDU_Switch_Relay(PDU_RELAY_KMP,  PDU_RELAY_OPEN);
	PDU_Switch_Relay(PDU_RELAY_KMF,  PDU_RELAY_OPEN);

	/* CAN Status Setup */
	pcsBmsStatus->bmc_main_relay		= RELAY_OPEN;
	pcsBmsStatus->bmc_precharge_relay	= RELAY_OPEN;
	pcsBmsStatus->bmc_status			= BMU_PRECHARGE;
	pcsBmsStatus->bmc_pcs_command		= BMU_PCS_STOP;

	/* 2. Checking PV Condition */
	if((pvVoltage > 150.0 || ctrlFlag->isBypass_power_mode))
	{
		if((!sysTickParam->isTimeOut) && !toggle_timer) // start timer
		{
			toggle_timer = true;
			sysTickParam->enable	= true;
			sysTickParam->count		= 15;
		}
		else if((sysTickParam->isTimeOut) && toggle_timer)
		{
			toggle_timer = false;
			sysTickParam->isTimeOut	= false;
			sysTickParam->count		= 0;
			PDU_Switch_Relay(PDU_RELAY_KMF,  PDU_RELAY_CLOSE);
			PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_CLOSE);
			pcsBmsStatus->bmc_precharge_relay = RELAY_CLOSE;
			*state	= OP_TURN_ON;
		}
	}
	else if((pvVoltage < 150.0 || !ctrlFlag->isBypass_power_mode))
	{
		toggle_timer = false;
		sysTickParam->enable	= false;
		sysTickParam->isTimeOut	= false;
		sysTickParam->count		= 0;
	}
}

void OP_TurnOnPCS(OP_STATE_t *state, MISC_SYSTICK_PARAM_t *sysTickParam,
		OP_CTRL_FLAG_t *ctrlFlag, PCS_BMC_STATUS_t *pcsBmsStatus, PCS_STATUS_t *pcsStatus)
{
	static bool toggle_timer = false;
	static bool isPrechargeExecuted = false;

	if(!XMC_GPIO_GetInput(P2_2) && !isPrechargeExecuted && (pcsStatus->pcs_system_status == PCS_STAT_STANDBY))
	{
		if((!sysTickParam->isTimeOut) && (!sysTickParam->enable) && !toggle_timer)
		{
			toggle_timer			= true;
			sysTickParam->enable	= true;
			sysTickParam->count		= 2;
		}
		else if ((sysTickParam->isTimeOut) && (!sysTickParam->enable))
		{
			ctrlFlag->isPerforming_Precharge = true;
			toggle_timer			= false;
			sysTickParam->isTimeOut = false;
			sysTickParam->count		= 0;
		}
	}
	else if(XMC_GPIO_GetInput(P2_2) && !isPrechargeExecuted)
	{
		toggle_timer			= false;
		sysTickParam->enable 	= false;
		sysTickParam->count 	= 0;
	}

	if(ctrlFlag->isPerforming_Precharge)
	{
		if(!isPrechargeExecuted)
		{
			pcsBmsStatus->bmc_main_relay 		= RELAY_OPEN;
			pcsBmsStatus->bmc_precharge_relay	= RELAY_CLOSE;
			pcsBmsStatus->bmc_status			= BMU_PRECHARGE;
			pcsBmsStatus->bmc_pcs_command		= BMU_PCS_STOP;
			isPrechargeExecuted					= true;
		}

		if(pcsStatus->pcs_precharge_IsComplete)
		{
			/* CLose Main Relay */
			PDU_Switch_Relay(PDU_RELAY_KMP, PDU_RELAY_CLOSE);

			/* Open Predischarge Relay */
			PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_OPEN);

			pcsBmsStatus->bmc_main_relay		= RELAY_CLOSE;
			pcsBmsStatus->bmc_precharge_relay	= RELAY_OPEN;
			pcsBmsStatus->bmc_status			= BMU_RUNNING;
			pcsBmsStatus->bmc_pcs_command		= BMU_PCS_START;

			*state = OP_RUNNING;

			/* close battery relay */
			PDU_Switch_Relay(PDU_RELAY_KBAT, PDU_RELAY_CLOSE);
			isPrechargeExecuted					= false;
			ctrlFlag->isPerforming_Precharge	= false;
		}
	}
}

void OP_CheckingPCSStatus(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						  MISC_SYSTICK_PARAM_t *sysTickParam,
						  OP_CTRL_FLAG_t *ctrlFlag)
{
	static bool toggle_timer = false;
	static bool isPutToStandbyExecuted = false;

	/* 1. Check PCS Warning Status */
	if(pcsStatus->pcs_system_status == PCS_STAT_STANDBY)
	{
		*state = OP_ERROR;
	}

	if(pcsStatus->pcs_system_status == PCS_STAT_INITIAL)
	{
		*state = OP_SHUTDOWN;
	}

	if(!XMC_GPIO_GetInput(P2_2) && !isPutToStandbyExecuted)
	{
		if((!sysTickParam->isTimeOut) && (!sysTickParam->enable) && !toggle_timer)
		{
			toggle_timer			= true;
			sysTickParam->enable	= true;
			sysTickParam->count		= 3;
		}
		else if ((sysTickParam->isTimeOut) && (!sysTickParam->enable) && !isPutToStandbyExecuted)
		{
			isPutToStandbyExecuted	= true;
			toggle_timer			= false;
			sysTickParam->isTimeOut = false;
			sysTickParam->count		= 0;
		}
	}
	else if(XMC_GPIO_GetInput(P2_2) && !isPutToStandbyExecuted)
	{
		toggle_timer			= false;
		sysTickParam->enable 	= false;
		sysTickParam->count 	= 0;
	}

	/* Change to STANDBY upon the timer's time-out and button is released */
	if(isPutToStandbyExecuted)
	{
		if(XMC_GPIO_GetInput(P2_2))
		{
			isPutToStandbyExecuted	= false;
			*state					= OP_STANDBY;
		}
	}
}

void OP_PutSystemStandby(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						 OP_CTRL_FLAG_t *ctrlFlag,
						 PCS_BMC_STATUS_t *pcsBmsStatus)
{
	static bool isStandbyExecuted = false;

	if(pcsStatus->pcs_system_status == PCS_STAT_RUNNING)
	{
		if(!isStandbyExecuted)
		{
			pcsBmsStatus->bmc_pcs_command = BMU_PCS_STOP;
			isStandbyExecuted = true;
		}
	}

	if(pcsStatus->pcs_system_status == PCS_STAT_STANDBY)
	{
		/* Close Pre Discharge Relay */
		PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_CLOSE);

		/* Open Main Relay */
		PDU_Switch_Relay(PDU_RELAY_KMP, PDU_RELAY_OPEN);

		pcsBmsStatus->bmc_main_relay		= RELAY_OPEN;
		pcsBmsStatus->bmc_precharge_relay	= RELAY_CLOSE;
		pcsBmsStatus->bmc_status			= BMU_PRECHARGE;

		*state = OP_TURN_ON;
		isStandbyExecuted = false;
	}
}

void OP_HandlingPCSWarning(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						   MISC_SYSTICK_PARAM_t *sysTickParam,
						   OP_CTRL_FLAG_t *ctrlFlag, BMC_PARAMETER_t *bmsParam,
						   PCS_BMC_STATUS_t *pcsBmsStatus)
{

	/* 1. if error elapsed, attempt for change PCS to RUN state */
	if(!ctrlFlag->isStandby)
	{
		pcsBmsStatus->bmc_pcs_command = BMU_PCS_START;
	}

	/* 1. if PCS acknowledged that already change to RUN, return the state to RUNNING */
	if(pcsStatus->pcs_system_status == PCS_STAT_RUNNING)
	{
		*state = OP_RUNNING;
	}
	/* 2. Check SoC Level and BMC Dip Switch Mode*/
	if((pcsStatus->pcs_system_status == PCS_STAT_INITIAL) || (!ctrlFlag->isBypass_power_mode && bmsParam->soc < 50))
	{
		*state = OP_SHUTDOWN;
	}
}

void OP_PutSystemShutDown(OP_STATE_t *state, MISC_SYSTICK_PARAM_t *sysTickParam, OP_CTRL_FLAG_t *ctrlFlag,
						  PCS_BMC_STATUS_t *pcsBmsStatus)
{
	if(!sysTickParam->isTimeOut)
	{
		sysTickParam->enable	= true;
		sysTickParam->count		= 10;
	}
	else
	{
		sysTickParam->isTimeOut = false;
		sysTickParam->count = 0;

		/* Close Pre Discharge Relay */
		PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_CLOSE);

		/* Open Main Relay */
		PDU_Switch_Relay(PDU_RELAY_KMP, PDU_RELAY_OPEN);
		pcsBmsStatus->bmc_main_relay		= RELAY_OPEN;
		pcsBmsStatus->bmc_precharge_relay	= RELAY_CLOSE;
		pcsBmsStatus->bmc_status			= BMU_PRECHARGE;
		if(!ctrlFlag->isBypass_power_mode)
		{
			PDU_Switch_Relay(PDU_RELAY_KBAT, PDU_RELAY_OPEN);
			*state = OP_POWERUP;
		}
		else
		{
			*state = OP_TURN_ON;
		}
	}
}

void OP_CheckingMode(OP_CTRL_FLAG_t *ctrlFlag)
{
	/* Read Dip Switch */
	(XMC_GPIO_GetInput(P4_2) == 1)?(ctrlFlag->isBypass_power_mode = true):
								   (ctrlFlag->isBypass_power_mode = false);
	(XMC_GPIO_GetInput(P4_3) == 1)?(ctrlFlag->isAuto_mode = true):
								   (ctrlFlag->isAuto_mode = false);
	(XMC_GPIO_GetInput(P4_4) == 1)?(ctrlFlag->isChecking_peripheral = true):
								   (ctrlFlag->isChecking_peripheral = false);
	(XMC_GPIO_GetInput(P4_5) == 1)?(ctrlFlag->isForced_shutting_down = true):
								   (ctrlFlag->isForced_shutting_down = false);
}

void OP_CheckingError(OP_CTRL_FLAG_t *ctrlFlag, PCS_STATUS_t *pcsStatus,
					  DCGRID_STATUS_t *gridStatus, ACINV_STATUS_t *invStatus,
					  PVCONV_STATUS_t *pvStatus, BATCONV_STATUS_t *BatStatus)
{
	  /* Check if any system in Standby mode */
	ctrlFlag->isStandby = pcsStatus->pcs_system_pcsIsStandBy 	 |\
						  gridStatus->dcgrid_system_pcsIsStandBy |\
						  invStatus->acinv_system_pcsIsStandBy	 |\
						  pvStatus->pvconv_system_pcsIsStandBy   |\
						  BatStatus->batconv_system_pcsIsStandBy;
}

