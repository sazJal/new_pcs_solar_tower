/*
 * variables.c
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */


#include "variables.h"

PCS_VOLTAGE_PARAM_t	pcsVoltages;
PCS_CURRENT_PARAM_t	pcsCurrents;
PCS_POWER_PARAM_t 	pcsPowers;
PCS_MISC_PARAM_t 	pcsMiscs;

PCS_STATUS_t 		pcsSystemStatus;
DCGRID_STATUS_t 	pcsDCgridStatus;
ACINV_STATUS_t 		pcsACinvStatus;
PVCONV_STATUS_t 	pcsPVconvStatus;
BATCONV_STATUS_t 	pcsBatconvStatus;

/* ESP32 Data */
ESP_Data_Group_t 	data_group;
uint8_t *buffptr;
uint8_t espBuffer[80];

MISC_SYSTICK_PARAM_t sysTickParamHandle;

/* ADC Sensor Variables */
PDU_SENSOR_RAW_t pduSensorRaw;
PDU_SENSOR_AVG_t pduSensorAvg;
PDU_SENSOR_t 	 pduSensor;

/* Genix Data */
GENIX_DATA_t genix_data;
