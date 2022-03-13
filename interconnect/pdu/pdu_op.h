/*
 * pdu_op.h
 *
 *  Created on: Dec 14, 2021
 *      Author: Lenovo
 */

#ifndef INTERCONNECT_PDU_PDU_OP_H_
#define INTERCONNECT_PDU_PDU_OP_H_

#include "pdu_op_conf.h"

typedef enum PDU_RELAY_NAME
{
	PDU_RELAY_KBAT,
	PDU_RELAY_KMP,
	PDU_RELAY_KMF,
	PDU_RELAY_KDSG
} PDU_RELAY_NAME_t;

typedef enum PDU_RELAY_STAT
{
	PDU_RELAY_OPEN,
	PDU_RELAY_CLOSE
} PDU_RELAY_STAT_t;

typedef struct PDU_SENSOR_RAW
{
	uint16_t rawPVPT;
	uint16_t rawBTPT;
	uint16_t rawHVPT;
	uint16_t rawDCCT;
} PDU_SENSOR_RAW_t;

typedef struct PDU_SENSOR_AVG
{
	bool 		isValid;
	bool 		isBeingProcessed;
	uint8_t 	count;
	uint32_t	sumAvgPVPT;
	uint32_t	sumAvgBTPT;
	uint32_t	sumAvgHVPT;
	uint32_t	sumAvgDCCT;
} PDU_SENSOR_AVG_t;

typedef struct PDU_SENSOR
{
	float mPVPT;
	float mBTPT;
	float mHVPT;
	float mDCCT;
} PDU_SENSOR_t;


void PDU_SensorADC_Init(void);
void PDU_Relay_Init(void);
void PDU_Switch_Relay(PDU_RELAY_NAME_t relay_name, PDU_RELAY_STAT_t level);
void PDU_ADC_Start(void);

#endif /* INTERCONNECT_PDU_PDU_OP_H_ */
