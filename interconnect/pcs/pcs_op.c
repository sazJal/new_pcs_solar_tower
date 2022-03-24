/*
 * pcs_op.c
 *
 *  Created on: Nov 23, 2021
 *      Author: Lenovo
 */

#include "pcs_op.h"

void PCS_ConnectionInit(void)
{
	/* Configure CAN Module */
	XMC_GPIO_Init(CAN0_TXD, &CAN0_txd_config);
	XMC_GPIO_Init(CAN0_RXD, &CAN0_rxd_config);

	XMC_CAN_Init(CAN, XMC_CAN_CANCLKSRC_MCLK, 11000000);

	XMC_CAN_NODE_EnableConfigurationChange(CAN_NODE0);
	XMC_CAN_NODE_SetReceiveInput(CAN_NODE0, XMC_CAN_NODE_RECEIVE_INPUT_RXDCA);
	XMC_CAN_NODE_DisableConfigurationChange(CAN_NODE0);

	/* NODE 1 Initialization */
	XMC_CAN_NODE_EnableConfigurationChange(CAN_NODE0);
	XMC_CAN_NODE_NominalBitTimeConfigure(CAN_NODE0,&can_baud);
	XMC_CAN_NODE_DisableConfigurationChange(CAN_NODE0);

	XMC_CAN_MO_Config(&msgObjToPCS);
	XMC_CAN_MO_Config(&msgObjFromPCS);

	/* Enable receive event */
	XMC_CAN_MO_EnableEvent(&msgObjFromPCS, XMC_CAN_MO_EVENT_RECEIVE);

	/* Set receive interrupt Service request number */
	XMC_SCU_SetInterruptControl(7, XMC_SCU_IRQCTRL_CAN0_SR3_IRQ7);
	XMC_CAN_MO_SetEventNodePointer(&msgObjFromPCS, XMC_CAN_MO_POINTER_EVENT_RECEIVE, 3);
	NVIC_SetPriority(IRQ7_IRQn, 1);
	NVIC_EnableIRQ(IRQ7_IRQn);

	XMC_CAN_AllocateMOtoNodeList(CAN,0,4);
	XMC_CAN_AllocateMOtoNodeList(CAN,0,2);
	XMC_CAN_NODE_ResetInitBit(CAN_NODE0);
}

void PCS_BMUTransmitToPCS(uint32_t id, PCS_BMC_STATUS_t *bmuStatus, BMC_PARAMETER_t *bmuParam)
{
	switch(id)
	{
		case (0x100)	:
			msgObjToPCS.can_identifier 	 = id;
			msgObjToPCS.can_data_byte[0] = 0x00;
			msgObjToPCS.can_data_byte[1] = bmuStatus->bmc_status;
			msgObjToPCS.can_data_byte[2] = 0x00;
			msgObjToPCS.can_data_byte[3] = bmuStatus->bmc_pcs_command;
			msgObjToPCS.can_data_byte[4] = 0x00;
			msgObjToPCS.can_data_byte[5] = 0x00;
			msgObjToPCS.can_data_byte[5]|= (bmuStatus->bmc_precharge_relay<<1) | bmuStatus->bmc_main_relay;
			msgObjToPCS.can_data_byte[6] = 0x00;
			msgObjToPCS.can_data_byte[7] = 0x00;
			break;
		case (0x101)	:
			msgObjToPCS.can_identifier 	 = id;
			msgObjToPCS.can_data_byte[0] = (uint8_t)(bmuParam->soc>>8);
			msgObjToPCS.can_data_byte[1] = (uint8_t)(bmuParam->soc);
			msgObjToPCS.can_data_byte[2] = (uint8_t)(bmuParam->vbat>>8);
			msgObjToPCS.can_data_byte[3] = (uint8_t)(bmuParam->vbat);
			msgObjToPCS.can_data_byte[4] = (uint8_t)(bmuParam->ibat>>8);
			msgObjToPCS.can_data_byte[5] = (uint8_t)(bmuParam->ibat);
			msgObjToPCS.can_data_byte[6] = (uint8_t)(bmuParam->tempbat>>8);
			msgObjToPCS.can_data_byte[7] = (uint8_t)(bmuParam->tempbat);
			break;
		default		:
			break;
	}
	XMC_CAN_MO_Config(&msgObjToPCS);
	XMC_CAN_MO_Transmit(&msgObjToPCS);
}

PCS_STATUS_t PCS_GetPCSStatus(XMC_CAN_MO_t *can_m0_obj)
{
	PCS_STATUS_t param;
	uint8_t 	 status;
	status = can_m0_obj->can_data_byte[0];
	if(status<=3) 						{param.pcs_system_status = PCS_STAT_INITIAL;}
	else if((status==4) ) 				{param.pcs_system_status = PCS_STAT_STANDBY;}
	else if((status==6) || (status==5)) {param.pcs_system_status = PCS_STAT_RUNNING;}

	status = can_m0_obj->can_data_byte[1];
	if(status)	{param.pcs_precharge_IsComplete = true;}
	else 		{param.pcs_precharge_IsComplete = false;}

	param.pcs_system_warning = PCS_WARN_NORMAL;
	param.pcs_system_pcsIsStandBy = false;

	/* check the warning status */
	status = can_m0_obj->can_data_byte[2];
	if((status>>6)&0x01)
	{
		param.pcs_system_warning = PCS_WARN_PCS_OT;
		param.pcs_system_pcsIsStandBy = true;
	}

	status = can_m0_obj->can_data_byte[3];
	if((status>>7)&0x01)
	{
		param.pcs_system_warning = PCS_WARN_BAT_OVCHG;
		param.pcs_system_pcsIsStandBy = true;
	}
	if((status>>6)&0x01){param.pcs_system_warning = PCS_WARN_BAT_CHGSUP;}
	if((status>>5)&0x01){param.pcs_system_warning = PCS_WARN_BAT_DSGSUP;}
	if((status>>4)&0x01){param.pcs_system_warning = PCS_WARN_BAT_OVDSG;}
	if((status>>3)&0x01)
	{
		param.pcs_system_warning = PCS_WARN_BAT_OVDSG2;
		param.pcs_system_pcsIsStandBy = true;
	}
	if((status>>2)&0x01)
	{
		param.pcs_system_warning = PCS_WARN_COMM_ERR;
		param.pcs_system_pcsIsStandBy = true;
	}

	return param;
}

DCGRID_STATUS_t 	PCS_GetDCGridStatus(XMC_CAN_MO_t *can_m0_obj)
{
	DCGRID_STATUS_t param;
	uint8_t			status;

	param.dcgrid_system_pcsIsStandBy = false;

	status = can_m0_obj->can_data_byte[0];
	if		((status<=3)||(status==6)||(status==7)) {param.dcgrid_system_status = GRID_STAT_NOT_RUNNING;}
	else if ((status==4)||(status ==5)) 			{param.dcgrid_system_status = GRID_STAT_CHG;}
	else if ((status==8)||(status ==9)) 			{param.dcgrid_system_status = GRID_STAT_DSG;}

	param.dcgrid_system_warning=GRID_WARN_NORMAL;

	/* check the warning status */
	status = can_m0_obj->can_data_byte[2];
	if((status>>7)&0x01){param.dcgrid_system_warning=GRID_WARN_OV;}
	if((status>>6)&0x01){param.dcgrid_system_warning=GRID_WARN_OC;}

	return param;
}

ACINV_STATUS_t 		PCS_GetACInvStatus(XMC_CAN_MO_t *can_m0_obj)
{
	ACINV_STATUS_t param;
	uint8_t		   status;

	status = can_m0_obj->can_data_byte[0];
	if((status<=3)||(status==11)||(status==13))	{param.acinv_system_status = INV_STAT_NOT_RUNNING;}
	else if((status==14)||(status ==15))  		{param.acinv_system_status = INV_STAT_RUNNING;}
	else							 			{param.acinv_system_status = INV_STAT_NONE;}

	param.acinv_system_warning = INV_WARN_NORMAL;
	param.acinv_system_pcsIsStandBy = false;

	/* check the warning status */
	status = can_m0_obj->can_data_byte[2];
	if((status>>7)&0x01)
		{
			param.acinv_system_warning = INV_WARN_MCU_OV;
			param.acinv_system_pcsIsStandBy = true;
		}
	if((status>>3)&0x01){param.acinv_system_warning = INV_WARN_OT;}
	if((status>>2)&0x01){param.acinv_system_warning = INV_WARN_OV;}
	if((status>>1)&0x01){param.acinv_system_warning = INV_WARN_UV;}
	if((status)&0x01){param.acinv_system_warning = INV_WARN_OL;}

	return param;
}

PVCONV_STATUS_t 	PCS_GetPVConvStatus(XMC_CAN_MO_t *can_m0_obj)
{
	PVCONV_STATUS_t param;
	uint8_t		   status;
	status = can_m0_obj->can_data_byte[0];
	if 		(status<=3) 				{param.pvconv_system_status = PV_STAT_NOT_RUNNING;}
	else if ((status==4)||(status ==5))	{param.pvconv_system_status = PV_STAT_RUNNING;}

	param.pvconv_system_warning = PV_WARN_NORMAL;
	param.pvconv_system_pcsIsStandBy = false;

	/* check the warning status */
	status = can_m0_obj->can_data_byte[2];
	if((status>>7)&0x01){param.pvconv_system_warning = PV_WARN_OV;}
	if((status>>6)&0x01){param.pvconv_system_warning = PV_WARN_OC;}

	return param;
}

BATCONV_STATUS_t 	PCS_GetBatConvStatus(XMC_CAN_MO_t *can_m0_obj)
{
	BATCONV_STATUS_t param;
	uint8_t		   status;
	status = can_m0_obj->can_data_byte[0];
	if (status<=3) 						{param.batconv_system_status = BATCONV_STAT_NOT_RUNNING;}
	else if((status==4)||(status ==5))	{param.batconv_system_status = BATCONV_STAT_RUNNING;}

	param.batconv_system_warning = BATCONV_WARN_NORMAL;
	param.batconv_system_pcsIsStandBy = false;

	/* check the warning status */
	status = can_m0_obj->can_data_byte[2];
	if((status>>7)&0x01)
		{
			param.batconv_system_warning = BATCONV_WARN_OV;
			param.batconv_system_pcsIsStandBy = true;
		}
	if((status>>6)&0x01)
		{
			param.batconv_system_warning = BATCONV_WARN_OC;
			param.batconv_system_pcsIsStandBy = true;
		}
	if((status>>5)&0x01)
		{
			param.batconv_system_warning = BATCONV_WARN_DCDC_OV;
			param.batconv_system_pcsIsStandBy = true;
		}
	if((status>>2)&0x01)
		{
			param.batconv_system_warning = BATCONV_WARN_DCDC_OT;
			param.batconv_system_pcsIsStandBy = true;
		}

	return param;
}

PCS_VOLTAGE_PARAM_t	PCS_GetVoltParam(XMC_CAN_MO_t *can_m0_obj)
{
	PCS_VOLTAGE_PARAM_t param;

	param.vdcGrid 	= ((uint16_t)(can_m0_obj->can_data_byte[0])<<8) | can_m0_obj->can_data_byte[1];
	param.vacOut  	= ((uint16_t)(can_m0_obj->can_data_byte[2])<<8) | can_m0_obj->can_data_byte[3];
	param.vPV 		= ((uint16_t)(can_m0_obj->can_data_byte[4])<<8) | can_m0_obj->can_data_byte[5];
	param.vbat  	= ((uint16_t)(can_m0_obj->can_data_byte[6])<<8) | can_m0_obj->can_data_byte[7];

	return param;
}

PCS_CURRENT_PARAM_t	PCS_GetAmpParam(XMC_CAN_MO_t *can_m0_obj)
{
	PCS_CURRENT_PARAM_t param;

	param.idcGrid  = ((int16_t)(can_m0_obj->can_data_byte[0])<<8) | can_m0_obj->can_data_byte[1];
	param.iacOut   = ((uint16_t)(can_m0_obj->can_data_byte[2])<<8) | can_m0_obj->can_data_byte[3];
	param.iPV 		= ((uint16_t)(can_m0_obj->can_data_byte[4])<<8) | can_m0_obj->can_data_byte[5];
	param.ibat  	= ((int16_t)(can_m0_obj->can_data_byte[6])<<8) | can_m0_obj->can_data_byte[7];

	return param;
}

PCS_POWER_PARAM_t 	PCS_GetPowerParam(XMC_CAN_MO_t *can_m0_obj)
{
	PCS_POWER_PARAM_t param;

	param.pdcGrid  = ((int16_t)(can_m0_obj->can_data_byte[0])<<8) | can_m0_obj->can_data_byte[1];
	param.pacOut   = ((uint16_t)(can_m0_obj->can_data_byte[2])<<8) | can_m0_obj->can_data_byte[3];
	param.pPV 	   = ((uint16_t)(can_m0_obj->can_data_byte[4])<<8) | can_m0_obj->can_data_byte[5];
	param.pbat     = ((int16_t)(can_m0_obj->can_data_byte[6])<<8) | can_m0_obj->can_data_byte[7];

	return param;
}

PCS_MISC_PARAM_t 	PCS_GetMiscParam(XMC_CAN_MO_t *can_m0_obj)
{
	PCS_MISC_PARAM_t param;

	param.tempPCS 		= ((uint16_t)(can_m0_obj->can_data_byte[0])<<8) | can_m0_obj->can_data_byte[1];
	param.frequency  	= ((uint16_t)(can_m0_obj->can_data_byte[2])<<8) | can_m0_obj->can_data_byte[3];

	return param;
}

