/*
 * pcs_op.h
 *
 *  Created on: Nov 23, 2021
 *      Author: Lenovo
 */

#ifndef INTERCONNECT_PCS_PCS_OP_H_
#define INTERCONNECT_PCS_PCS_OP_H_

#include "stdbool.h"
#include "stdlib.h"
#include "pcs_op_conf.h"

/* 	PCS BMS Enumeration 	*/
typedef enum BMC_STATUS
{
	BMU_STOP,
	BMU_PRECHARGE,
	BMU_RUNNING,
	BMU_FAULT
} BMC_STATUS_t;

typedef enum PCS_OP_COMMAND
{
	BMU_PCS_STOP = 0x00,
	BMU_PCS_START = 0x01
} PCS_OP_COMMAND_t;

typedef enum BMC_RELAY_STATUS
{
	RELAY_OPEN,
	RELAY_CLOSE,
} BMC_RELAY_STATUS;

/* 	PCS Operation Enumeration */
typedef enum PCS_STATUS_CODE
{
	PCS_STAT_INITIAL 	= (uint8_t)(0x00),
	PCS_STAT_STANDBY 	= (uint8_t)(0x04),
	PCS_STAT_RUNNING 	= (uint8_t)(0x06),
	PCS_STAT_UNDEFINED 	= (uint8_t)(0xFF)
} PCS_STATUS_CODE_t;

typedef enum PCS_WARNING_CODE
{
	PCS_WARN_NORMAL		= 0x00,
	PCS_WARN_BAT_OVCHG	= 0x01,
	PCS_WARN_BAT_CHGSUP = 0x02,
	PCS_WARN_BAT_DSGSUP = 0x03,
	PCS_WARN_BAT_OVDSG  = 0x04,
	PCS_WARN_BAT_OVDSG2 = 0x05,
	PCS_WARN_PCS_OT		= 0x06
} PCS_WARNING_CODE_t;

typedef enum GRID_STATUS_CODE
{
	GRID_STAT_NOT_RUNNING	= 0x00,
	GRID_STAT_CHG			= 0x04,
	GRID_STAT_DSG			= 0x08
} GRID_STATUS_CODE_t;

typedef enum GRID_WARNING_CODE
{
	GRID_WARN_NORMAL	= 0x00,
	GRID_WARN_OV		= 0x01,
	GRID_WARN_OC		= 0x02
} GRID_WARNING_CODE_t;

typedef enum INV_STATUS_CODE
{
	INV_STAT_NONE 			= 0x04,
	INV_STAT_NOT_RUNNING 	= 0x00,
	INV_STAT_RUNNING 		= 0x0E
} INV_STATUS_CODE_t;

typedef enum INV_WARNING_CODE
{
	INV_WARN_NORMAL	= 0x00,
	INV_WARN_MCU_OV = 0x01,
	INV_WARN_OT		= 0x02,
	INV_WARN_OV		= 0x03,
	INV_WARN_UV		= 0x04,
	INV_WARN_OL		= 0x05
} INV_WARNING_CODE_t;

typedef enum PV_STATUS_CODE
{
	PV_STAT_NOT_RUNNING = 0x00,
	PV_STAT_RUNNING 	= 0x04,
} PV_STATUS_CODE_t;

typedef enum PV_WARNING_CODE
{
	PV_WARN_NORMAL	= 0x00,
	PV_WARN_OV		= 0x01,
	PV_WARN_OC		= 0x02
} PV_WARNING_CODE_t;

typedef enum BATCONV_STATUS_CODE
{
	BATCONV_STAT_NOT_RUNNING	= 0x00,
	BATCONV_STAT_RUNNING		= 0x04,
} BATCONV_STATUS_CODE_t;

typedef enum BATCONV_WARNING_CODE
{
	BATCONV_WARN_NORMAL 	= 0x00,
	BATCONV_WARN_OV			= 0x01,
	BATCONV_WARN_OC			= 0x02,
	BATCONV_WARN_DCDC_OV	= 0x03,
	BATCONV_WARN_DCDC_OT	= 0x04
} BATCONV_WARNING_CODE_t;

/*  BMS Internal Sensor Attributes */
typedef struct BMC_PARAMETER
{
	uint16_t 		soc;
	uint16_t 		vbat;
	uint16_t 		ibat;
	uint16_t 		tempbat;
} BMC_PARAMETER_t;

/*	PCS BMS Attributes */
typedef struct PCS_BMC_STATUS
{
	BMC_STATUS_t   		bmc_status;
	PCS_OP_COMMAND_t	bmc_pcs_command;
	BMC_RELAY_STATUS 	bmc_main_relay;
	BMC_RELAY_STATUS 	bmc_precharge_relay;
} PCS_BMC_STATUS_t;

/* PCS Internal Sensor Attributes */
typedef struct PCS_VOLTAGE_PARAM
{
	uint16_t vdcGrid;
	uint16_t vacOut;
	uint16_t vPV;
	uint16_t vbat;
} PCS_VOLTAGE_PARAM_t;

typedef struct PCS_CURRENT_PARAM
{
	int16_t idcGrid;
	int16_t iacOut;
	int16_t iPV;
	int16_t ibat;
} PCS_CURRENT_PARAM_t;

typedef struct PCS_POWER_PARAM
{
	uint16_t pdcGrid;
	uint16_t pacOut;
	uint16_t pPV;
	uint16_t pbat;
} PCS_POWER_PARAM_t;

typedef struct PCS_MISC_PARAM
{
	uint16_t tempPCS;
	uint16_t frequency;
} PCS_MISC_PARAM_t;

typedef struct PCS_STATUS
{
	PCS_STATUS_CODE_t  pcs_system_status;
	bool			   pcs_precharge_IsComplete;
	PCS_WARNING_CODE_t pcs_system_warning;
	bool	 		   pcs_system_fault;
	bool			   pcs_system_pcsIsStandBy;
} PCS_STATUS_t;

typedef struct DCGRID_STATUS
{
	GRID_STATUS_CODE_t  dcgrid_system_status;
	GRID_WARNING_CODE_t dcgrid_system_warning;
	bool				dcgrid_system_fault;
	bool			    dcgrid_system_pcsIsStandBy;
} DCGRID_STATUS_t;

typedef struct ACINV_STATUS
{
	INV_STATUS_CODE_t   acinv_system_status;
	INV_WARNING_CODE_t  acinv_system_warning;
	bool				acinv_system_fault;
	bool			    acinv_system_pcsIsStandBy;
} ACINV_STATUS_t;

typedef struct PVCONV_STATUS
{
	PV_STATUS_CODE_t   pvconv_system_status;
	PV_WARNING_CODE_t  pvconv_system_warning;
	bool			   pvconv_system_fault;
	bool			   pvconv_system_pcsIsStandBy;
} PVCONV_STATUS_t;

typedef struct BATCONV_STATUS
{
	BATCONV_STATUS_CODE_t   batconv_system_status;
	BATCONV_WARNING_CODE_t  batconv_system_warning;
	bool				    batconv_system_fault;
	bool			    	batconv_system_pcsIsStandBy;
} BATCONV_STATUS_t;

/**************************************************/

void PCS_ConnectionInit(void);
void PCS_BMUTransmitToPCS(uint32_t id, PCS_BMC_STATUS_t *bmuStatus, BMC_PARAMETER_t *bmuParam);

PCS_STATUS_t 		PCS_GetPCSStatus(XMC_CAN_MO_t *can_m0_obj);
PCS_VOLTAGE_PARAM_t	PCS_GetVoltParam(XMC_CAN_MO_t *can_m0_obj);
PCS_CURRENT_PARAM_t	PCS_GetAmpParam(XMC_CAN_MO_t *can_m0_obj);
PCS_POWER_PARAM_t 	PCS_GetPowerParam(XMC_CAN_MO_t *can_m0_obj);
PCS_MISC_PARAM_t 	PCS_GetMiscParam(XMC_CAN_MO_t *can_m0_obj);

DCGRID_STATUS_t 	PCS_GetDCGridStatus(XMC_CAN_MO_t *can_m0_obj);
ACINV_STATUS_t 		PCS_GetACInvStatus(XMC_CAN_MO_t *can_m0_obj);
PVCONV_STATUS_t 	PCS_GetPVConvStatus(XMC_CAN_MO_t *can_m0_obj);
BATCONV_STATUS_t 	PCS_GetBatConvStatus(XMC_CAN_MO_t *can_m0_obj);


#endif /* INTERCONNECT_PCS_PCS_OP_H_ */
