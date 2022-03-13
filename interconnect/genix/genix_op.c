/*
 * genix_op.c
 *
 *  Created on: Nov 23, 2021
 *      Author: Lenovo
 */

#include "genix_op.h"

void GENIX_ConnectionInit(void)
{
    XMC_GPIO_Init(P0_2, &tx_genix_config);
    XMC_GPIO_Init(P0_3, &rx_genix_config);

   	XMC_USIC_CH_Enable(XMC_USIC1_CH0);
	XMC_UART_CH_Init(XMC_USIC1_CH0, &uart_genix_config);
	XMC_UART_CH_SetInputSource(XMC_USIC1_CH0, XMC_UART_CH_INPUT_RXD, USIC1_C0_DX0_P0_3);

	XMC_UART_CH_EnableEvent(XMC_USIC1_CH0,XMC_UART_CH_EVENT_STANDARD_RECEIVE); // PSR.RFF
	XMC_UART_CH_SelectInterruptNodePointer(XMC_USIC1_CH0, XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE, 3);
	XMC_SCU_SetInterruptControl(IRQ12_IRQn, XMC_SCU_IRQCTRL_USIC1_SR3_IRQ12);
	NVIC_SetPriority(IRQ12_IRQn, 10U);
	NVIC_EnableIRQ(IRQ12_IRQn);

	XMC_UART_CH_Start(XMC_USIC1_CH0); // Sets the USIC channel operation mode to UART mode
}

void GENIX_SendStream(char *buffer, uint8_t length)
{
	uint8_t count = 0;
	while(count < length)
	{
		while(XMC_USIC_CH_GetTransmitBufferStatus(XMC_USIC1_CH0) == XMC_USIC_CH_TBUF_STATUS_BUSY){}
		XMC_UART_CH_ClearStatusFlag(XMC_USIC1_CH0, (uint32_t)XMC_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

		XMC_USIC1_CH0->TBUF[0U] = buffer[count];

		count++;
	 }
}

void GENIX_RequestData(GENIX_COMMAND_TYPE_t command)
{
	uint8_t ecc;
	uint8_t buffer[13];

	buffer[0] = 0xCC;
	buffer[1] = 0xDD;
	buffer[2] = 0x0A;
	buffer[3] = command;
	buffer[4] = 0x00;
	buffer[5] = 0x00;
	buffer[6] = 0x00;
	buffer[7] = 0x00;
	buffer[8] = 0x00;
	buffer[9] = 0x00;

	ecc = 0x00;
	for(uint8_t i = 0; i<10; i++)
	{
		ecc = ecc+buffer[i];
	}

	ecc = 0x1D;
	buffer[10] = ecc;
	buffer[11] = 0x0D;
	buffer[12] = 0x0A;

	GENIX_SendStream((char*)&buffer, 13);
}

