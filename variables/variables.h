/*
 * variables.h
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#ifndef VARIABLES_VARIABLES_H_
#define VARIABLES_VARIABLES_H_

#include "esp32_op.h"
#include "pcs_op.h"
#include "genix_op.h"
#include "gpio_op.h"
#include "pdu_op.h"
#include "timing_op.h"

typedef struct MISC_SYSTICK_PARAM
{
	bool		enable;
	uint16_t	count;
	bool		isTimeOut;
} MISC_SYSTICK_PARAM_t;

MISC_SYSTICK_PARAM_t sysTickParamHandle;

#define BATTERY_NUM_75 1

/* PCS BMC Parameter */
PCS_BMC_STATUS_t pcsBmsStatus;
BMC_PARAMETER_t	 bmsSensor;

/* PCS Variables */
PCS_VOLTAGE_PARAM_t	pcsVoltages;
PCS_CURRENT_PARAM_t	pcsCurrents;
PCS_POWER_PARAM_t 	pcsPowers;
PCS_MISC_PARAM_t 	pcsMiscs;

PCS_STATUS_t 		pcsSystemStatus;
DCGRID_STATUS_t 	pcsDCgridStatus;
ACINV_STATUS_t 		pcsACinvStatus;
PVCONV_STATUS_t 	pcsPVconvStatus;
BATCONV_STATUS_t 	pcsBatconvStatus;

/* ESP32 Variables */
ESP_Data_Group_t 	data_group;
uint8_t *buffptr;
uint8_t espBuffer[80];

/* ADC Sensor Variables */
PDU_SENSOR_RAW_t pduSensorRaw;
PDU_SENSOR_AVG_t pduSensorAvg;
PDU_SENSOR_t 	 pduSensor;

/* Genix Data */
GENIX_DATA_t genix_data;


#endif /* VARIABLES_VARIABLES_H_ */
