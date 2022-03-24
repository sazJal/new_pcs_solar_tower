/*
 * esp32_op.h
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#ifndef ESP32_ESP32_OP_H_
#define ESP32_ESP32_OP_H_

#include "esp32_op_conf.h"

#define SoF 0x05
#define EoF 0x0A
#define ESP_DATA_LENGTH 75

#define	CRC_POLY_16			0xA001
#define	CRC_START_16		0x0000

typedef enum ESP_RECEIVE_STATE
{
	ESP_SOF,
	ESP_LENGTH,
	ESP_DATA,
	ESP_CRC8,
	ESP_EOF
} ESP_RECEIVE_STATE_t;

typedef struct ESP_Data_Group
{
	/* pcs converter system */
	uint8_t pcs_system;
	uint8_t dcgrid_system;
	uint8_t inv_system;
	uint8_t pvconv_system;
	uint8_t batconv_system;

	/* pcs sensors */
	float 	 vDC_grid;
	float 	 vAC_inv;
	float 	 vPv_conv;
	float 	 vBat_conv;
	float 	 iDC_grid;
	float 	 iAC_inv;
	float 	 iPv_conv;
	float 	 iBat_conv;
	int16_t  pDC_grid;
	uint16_t pAC_inv;
	uint16_t pPv_conv;
	int16_t  pBat_conv;
	float 	 temp_sensor;
	float 	 frequency;

	/* bmc status */
	uint8_t  bmc_system;
	uint8_t  bmc_operation_state;

	/* battery information */
	uint8_t  soC;
	uint16_t vBat_genix;
	float 	 iBat_genix;
	uint8_t  temp_genix;

	/* real time clock */
	uint16_t year;
	uint8_t	 month;
	uint8_t  date;
	uint8_t  hour;
	uint8_t  minute;
	uint8_t  second;
} ESP_Data_Group_t;

void ESP32_ConnectionInit(void);
void ESP32_SendStream(char *buffer, uint8_t length);
uint8_t *ESP32_Map_Data(ESP_Data_Group_t *dataGroup);
void ESP32_CRC16_table_init(void);
uint16_t ESP32_CRC16_Operation(const unsigned char *input_str, unsigned char num_bytes);
uint8_t ESP32_CRC8_maxim(uint8_t *data, int length);

#endif /* ESP32_ESP32_OP_H_ */
