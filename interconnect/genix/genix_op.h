/*
 * genix_op.h
 *
 *  Created on: Nov 23, 2021
 *      Author: Lenovo
 */

#ifndef INTERCONNECT_GENIX_GENIX_OP_H_
#define INTERCONNECT_GENIX_GENIX_OP_H_


#include "genix_op_conf.h"


typedef struct _GENIX_DATA
{
	uint16_t 	pack_soc;
	uint16_t	pack_voltage;
	uint16_t	pack_max_temp;
	int16_t		pack_current;
} GENIX_DATA_t;

typedef enum GENIX_COMMAND_TYPE
{
	GENIX_SUMMARY 	= 0x02,		// Summary information acquisition request
	GENIX_ALARM   	= 0x03,		// Alarm information
	GENIX_INPUT		= 0x04,		// Drive input state
	GENIX_VOLTAGE	= 0x10,		// Cell voltage
	GENIX_TEMP		= 0x30,		// Temperatures
} GENIX_COMMAND_TYPE_t;

typedef enum GENIX_RCV_STATE
{
	RCV_HEADER_CHECK,
	RCV_DATA_LENGTH,
	RCV_RESPONSE_CODE,
	RCV_COLLECT_DATA,
	RCV_ECC_CHECK,
	RCV_TAIL_CHECK
} GENIX_RCV_STATE_t;

void GENIX_ConnectionInit(void);
void GENIX_SendStream(char *buffer, uint8_t length);
void GENIX_RequestData(GENIX_COMMAND_TYPE_t command);

#endif /* INTERCONNECT_GENIX_GENIX_OP_H_ */
