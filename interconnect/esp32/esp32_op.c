/*
 * esp32_op.c
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#include "esp32_op.h"

static bool      crc_tab16_init = false;
static uint16_t  crc_tab16[256];

void ESP32_ConnectionInit(void)
{
    XMC_GPIO_Init(P2_0, &tx_config);
    XMC_GPIO_Init(P2_1, &rx_config);

   	XMC_USIC_CH_Enable(XMC_USIC0_CH0);
	XMC_UART_CH_Init(XMC_USIC0_CH0, &uart_config);
	XMC_UART_CH_SetInputSource(XMC_USIC0_CH0, XMC_UART_CH_INPUT_RXD, USIC0_C0_DX0_P2_1);

	XMC_UART_CH_EnableEvent(XMC_USIC0_CH0,XMC_UART_CH_EVENT_STANDARD_RECEIVE); // PSR.RFF
	XMC_UART_CH_SelectInterruptNodePointer(XMC_USIC0_CH0, XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE, 3);
	XMC_SCU_SetInterruptControl(IRQ18_IRQn, XMC_SCU_IRQCTRL_USIC0_SR3_IRQ18);
	NVIC_SetPriority(IRQ18_IRQn, 10U);
	NVIC_EnableIRQ(IRQ18_IRQn);

	XMC_UART_CH_Start(XMC_USIC0_CH0);
}

void ESP32_SendStream(char *buffer, uint8_t length)
{
	uint8_t count = 0;
	while(count < length)
	{
		while(XMC_USIC_CH_GetTransmitBufferStatus(XMC_USIC0_CH0) == XMC_USIC_CH_TBUF_STATUS_BUSY){}
		XMC_UART_CH_ClearStatusFlag(XMC_USIC0_CH0, (uint32_t)XMC_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

		XMC_USIC0_CH0->TBUF[0U] = buffer[count];

		count++;
	 }
}

uint8_t *ESP32_Map_Data(ESP_Data_Group_t *dataGroup)
{
	static uint8_t buffer[80];
	uint8_t offset = 3;
	uint16_t crc;
	buffer[0] = SoF;
	buffer[1] = 0x02;
	buffer[2] = 69;
	/* pcs converter system */
	buffer[offset+0] = dataGroup->pcs_system;
	buffer[offset+1] = dataGroup->dcgrid_system;
	buffer[offset+2] = dataGroup->inv_system;
	buffer[offset+3] = dataGroup->pvconv_system;
	buffer[offset+4] = dataGroup->batconv_system;

	/* pcs sensors */
	buffer[offset+5]  = ((unsigned char*)&dataGroup->vDC_grid)[0];
	buffer[offset+6]  = ((unsigned char*)&dataGroup->vDC_grid)[1];
	buffer[offset+7]  = ((unsigned char*)&dataGroup->vDC_grid)[2];
	buffer[offset+8]  = ((unsigned char*)&dataGroup->vDC_grid)[3];
	buffer[offset+9]  = ((unsigned char*)&dataGroup->vAC_inv)[0];
	buffer[offset+10] = ((unsigned char*)&dataGroup->vAC_inv)[1];
	buffer[offset+11] = ((unsigned char*)&dataGroup->vAC_inv)[2];
	buffer[offset+12] = ((unsigned char*)&dataGroup->vAC_inv)[3];
	buffer[offset+13] = ((unsigned char*)&dataGroup->vPv_conv)[0];
	buffer[offset+14] = ((unsigned char*)&dataGroup->vPv_conv)[1];
	buffer[offset+15] = ((unsigned char*)&dataGroup->vPv_conv)[2];
	buffer[offset+16] = ((unsigned char*)&dataGroup->vPv_conv)[3];
	buffer[offset+17] = ((unsigned char*)&dataGroup->vBat_conv)[0];
	buffer[offset+18] = ((unsigned char*)&dataGroup->vBat_conv)[1];
	buffer[offset+19] = ((unsigned char*)&dataGroup->vBat_conv)[2];
	buffer[offset+20] = ((unsigned char*)&dataGroup->vBat_conv)[3];
	buffer[offset+21] = ((unsigned char*)&dataGroup->iDC_grid)[0];
	buffer[offset+22] = ((unsigned char*)&dataGroup->iDC_grid)[1];
	buffer[offset+23] = ((unsigned char*)&dataGroup->iDC_grid)[2];
	buffer[offset+24] = ((unsigned char*)&dataGroup->iDC_grid)[3];
	buffer[offset+25] = ((unsigned char*)&dataGroup->iAC_inv)[0];
	buffer[offset+26] = ((unsigned char*)&dataGroup->iAC_inv)[1];
	buffer[offset+27] = ((unsigned char*)&dataGroup->iAC_inv)[2];
	buffer[offset+28] = ((unsigned char*)&dataGroup->iAC_inv)[3];
	buffer[offset+29] = ((unsigned char*)&dataGroup->iPv_conv)[0];
	buffer[offset+30] = ((unsigned char*)&dataGroup->iPv_conv)[1];
	buffer[offset+31] = ((unsigned char*)&dataGroup->iPv_conv)[2];
	buffer[offset+32] = ((unsigned char*)&dataGroup->iPv_conv)[3];
	buffer[offset+33] = ((unsigned char*)&dataGroup->iBat_conv)[0];
	buffer[offset+34] = ((unsigned char*)&dataGroup->iBat_conv)[1];
	buffer[offset+35] = ((unsigned char*)&dataGroup->iBat_conv)[2];
	buffer[offset+36] = ((unsigned char*)&dataGroup->iBat_conv)[3];
	buffer[offset+37] = (dataGroup->pDC_grid) >> 8;
	buffer[offset+38] = (dataGroup->pDC_grid);
	buffer[offset+39] = (dataGroup->pAC_inv) >> 8;
	buffer[offset+40] = (dataGroup->pAC_inv);
	buffer[offset+41] = (dataGroup->pPv_conv) >> 8;
	buffer[offset+42] = (dataGroup->pPv_conv);
	buffer[offset+43] = (dataGroup->pBat_conv) >> 8;
	buffer[offset+44] = (dataGroup->pBat_conv);
	buffer[offset+45] = ((unsigned char*)&dataGroup->temp_sensor)[0];
	buffer[offset+46] = ((unsigned char*)&dataGroup->temp_sensor)[1];
	buffer[offset+47] = ((unsigned char*)&dataGroup->temp_sensor)[2];
	buffer[offset+48] = ((unsigned char*)&dataGroup->temp_sensor)[3];
	buffer[offset+49] = ((unsigned char*)&dataGroup->frequency)[0];
	buffer[offset+50] = ((unsigned char*)&dataGroup->frequency)[1];
	buffer[offset+51] = ((unsigned char*)&dataGroup->frequency)[2];
	buffer[offset+52] = ((unsigned char*)&dataGroup->frequency)[3];
	/* bmc status */
	buffer[offset+53] = dataGroup->bmc_system;

	/* battery information */
	buffer[offset+54] = dataGroup->soC;
	buffer[offset+55] = (dataGroup->vBat_genix) >> 8;
	buffer[offset+56] = (dataGroup->vBat_genix);
	buffer[offset+57] = ((unsigned char*)&dataGroup->iBat_genix)[0];
	buffer[offset+58] = ((unsigned char*)&dataGroup->iBat_genix)[1];
	buffer[offset+59] = ((unsigned char*)&dataGroup->iBat_genix)[2];
	buffer[offset+60] = ((unsigned char*)&dataGroup->iBat_genix)[3];
	buffer[offset+61] = dataGroup->temp_genix;

	/* rtc information */
	buffer[offset+62] = dataGroup->year >> 8;
	buffer[offset+63] = dataGroup->year;
	buffer[offset+64] = dataGroup->month;
	buffer[offset+65] = dataGroup->date;
	buffer[offset+66] = dataGroup->hour;
//	buffer[offset+67] = dataGroup->minute;
	buffer[offset+67] = dataGroup->bmc_operation_state;
	buffer[offset+68] = dataGroup->second;

	crc	= ESP32_CRC16_Operation((unsigned char *) &buffer[offset], buffer[2]);
	buffer[offset+69] = crc >> 8;
	buffer[offset+70] = crc;
	buffer[offset+71] = EoF;

	return buffer;
}

void ESP32_CRC16_table_init(void)
{
	uint16_t i;
	uint16_t j;
	uint16_t crc;
	uint16_t c;

	for (i=0; i<256; i++) {

		crc = 0;
		c   = i;

		for (j=0; j<8; j++) {

			if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ CRC_POLY_16;
			else                      crc =   crc >> 1;

			c = c >> 1;
		}

		crc_tab16[i] = crc;
	}

	crc_tab16_init = true;
}

uint16_t ESP32_CRC16_Operation(const unsigned char *input_str, unsigned char num_bytes)
{
	uint16_t crc;
	uint16_t tmp;
	uint16_t short_c;
	const unsigned char *ptr;
	unsigned char a;

	if (!crc_tab16_init) ESP32_CRC16_table_init();

	crc = CRC_START_16;
	ptr = input_str;

	if ( ptr != NULL ) for (a=0; a<num_bytes; a++) {

		short_c = 0x00ff & (uint16_t) *ptr;
		tmp     =  crc       ^ short_c;
		crc     = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];

		ptr++;
	}

	return crc;
}

uint8_t ESP32_CRC8_maxim(uint8_t *data, int length)
{
   uint8_t crc = 0x00;
   uint8_t extract;
   uint8_t sum;
   for(int i=0;i<length;i++)
   {
      extract = *data;
      for (uint8_t tempI = 8; tempI; tempI--)
      {
         sum = (crc ^ extract) & 0x01;
         crc >>= 1;
         if (sum)
            crc ^= 0x8C;
         extract >>= 1;
      }
      data++;
   }
   return crc;
}

