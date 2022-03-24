/*
 * operation.h
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#ifndef OPERATION_OPERATION_H_
#define OPERATION_OPERATION_H_

#include "variables.h"

typedef enum OP_STATE
{
	OP_POWERUP,		// Initialize remote device and handshaking
	OP_TURN_ON,
	OP_RUNNING,
	OP_STANDBY,
	OP_ERROR,
	OP_SHUTDOWN
} OP_STATE_t;

typedef struct OP_CTRL_FLAG
{
	bool isPerforming_Precharge;
	bool isStandby;
	bool isAuto_mode; // not used
	bool isBypass_power_mode;
	bool isForced_shutting_down; // not used
	bool isChecking_peripheral; // not used
} OP_CTRL_FLAG_t;

OP_STATE_t mcu_state;
OP_CTRL_FLAG_t controlFlag;

void OP_SetupConnection(OP_STATE_t *state, MISC_SYSTICK_PARAM_t *sysTickParam,
						OP_CTRL_FLAG_t *ctrlFlag, float pvVoltage, PCS_BMC_STATUS_t *pcsBmsStatus, PCS_STATUS_t *pcsStatus);
void OP_TurnOnPCS(OP_STATE_t *state, MISC_SYSTICK_PARAM_t *sysTickParam,
						OP_CTRL_FLAG_t *ctrlFlag, PCS_BMC_STATUS_t *pcsBmsStatus, PCS_STATUS_t *pcsStatus);
void OP_CheckingPCSStatus(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						  MISC_SYSTICK_PARAM_t *sysTickParam,
						  OP_CTRL_FLAG_t *ctrlFlag);
void OP_PutSystemStandby(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						 OP_CTRL_FLAG_t *ctrlFlag,
						 PCS_BMC_STATUS_t *pcsBmsStatus);
void OP_HandlingPCSWarning(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						   MISC_SYSTICK_PARAM_t *sysTickParam,
						   OP_CTRL_FLAG_t *ctrlFlag, BMC_PARAMETER_t *bmsParam,
						   PCS_BMC_STATUS_t *pcsBmsStatus);
void OP_PutSystemShutDown(OP_STATE_t *state, MISC_SYSTICK_PARAM_t *sysTickParam, OP_CTRL_FLAG_t *ctrlFlag,
						PCS_BMC_STATUS_t *pcsBmsStatus);
void OP_CheckingMode(OP_CTRL_FLAG_t *ctrlFlag);
void OP_CheckingError(OP_CTRL_FLAG_t *ctrlFlag, PCS_STATUS_t *pcsStatus,
					  DCGRID_STATUS_t *gridStatus, ACINV_STATUS_t *invStatus,
					  PVCONV_STATUS_t *pvStatus, BATCONV_STATUS_t *BatStatus);
#endif /* OPERATION_OPERATION_H_ */
