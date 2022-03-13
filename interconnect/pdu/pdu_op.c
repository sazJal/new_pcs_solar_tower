/*
 * pdu_op.c
 *
 *  Created on: Dec 14, 2021
 *      Author: Lenovo
 */

#include "pdu_op.h"

void PDU_SensorADC_Init(void)
{
	XMC_GPIO_Init(P2_9,&adc_io);
	XMC_GPIO_Init(P2_3,&adc_io);
	XMC_GPIO_Init(P2_4,&adc_io);
	XMC_GPIO_Init(P2_5,&adc_io);

	XMC_VADC_GLOBAL_Init(VADC, &g_global_handle);
	/* Configure a conversion kernel */
	XMC_VADC_GROUP_Init(VADC_G1, &g_group_handle);

	/* Enable the analog converters */
	XMC_VADC_GROUP_SetPowerMode(VADC_G1, XMC_VADC_GROUP_POWERMODE_NORMAL);

	XMC_VADC_GLOBAL_StartupCalibration(VADC);		//XMC1400 EE step will not work with this API here

	/* Configure the queue request source of the aforesaid conversion kernel */
	XMC_VADC_GROUP_QueueInit(VADC_G1, &g_queue_handle);

	XMC_VADC_GROUP_QueueInsertChannel(VADC_G1, g_queue_entry[0]);	// Queue no.0
	XMC_VADC_GROUP_QueueInsertChannel(VADC_G1, g_queue_entry[1]);	// Queue no.1
	XMC_VADC_GROUP_QueueInsertChannel(VADC_G1, g_queue_entry[2]);	// Queue no.2
	XMC_VADC_GROUP_QueueInsertChannel(VADC_G1, g_queue_entry[3]);	// Queue no.3

	XMC_VADC_GROUP_ChannelInit(VADC_G1,4, &g_channel_handle[0]);	// G1 ch 4 P2_9
	XMC_VADC_GROUP_ChannelInit(VADC_G1,5, &g_channel_handle[1]);	// G1 ch 5 P2_3
	XMC_VADC_GROUP_ChannelInit(VADC_G1,6, &g_channel_handle[2]);	// G1 ch 6 P2_4
	XMC_VADC_GROUP_ChannelInit(VADC_G1,7, &g_channel_handle[3]);	// G1 ch 7 P2_5

	XMC_VADC_GROUP_ResultInit(VADC_G1, 0, &g_result_handle[0]);
	XMC_VADC_GROUP_ResultInit(VADC_G1, 1, &g_result_handle[1]);
	XMC_VADC_GROUP_ResultInit(VADC_G1, 2, &g_result_handle[2]);
	XMC_VADC_GROUP_ResultInit(VADC_G1, 3, &g_result_handle[3]);

	/* Enable event at g_queue_entry[i].generate_interrupt */
//	XMC_VADC_GROUP_QueueSetReqSrcEventInterruptNode(VADC_G1, XMC_VADC_SR_GROUP_SR1);
//	NVIC_SetPriority(IRQ20_IRQn, IRQ20_IRQn_11);			// Table 4-3 Interrupt Node assignment
//	NVIC_EnableIRQ(IRQ20_IRQn);

	/* Enable event at g_channel_handle[i].event_gen_criteria */
//	XMC_VADC_GROUP_ChannelSetEventInterruptNode(VADC_G1, 7, XMC_VADC_SR_GROUP_SR1); // For Channel 7
//	NVIC_SetPriority(IRQ20_IRQn, IRQ20_IRQn_11);			// Table 4-3 Interrupt Node assignment
//	NVIC_EnableIRQ(IRQ20_IRQn);

	/* Enable event at check g_result_handle[i].event_gen_enable */
	XMC_VADC_GROUP_SetResultInterruptNode(VADC_G1, VADC_RES_MAX-1, XMC_VADC_SR_GROUP_SR0);
	NVIC_SetPriority(IRQ19_IRQn, 17U);
	NVIC_EnableIRQ(IRQ19_IRQn);
}

void PDU_Relay_Init(void)
{
	XMC_GPIO_Init(P1_4, &relay_KBAT);
	XMC_GPIO_Init(P1_5, &relay_KMP);
	XMC_GPIO_Init(P1_6, &relay_KMF);
	XMC_GPIO_Init(P1_7, &relay_KDSG);
}

void PDU_Switch_Relay(PDU_RELAY_NAME_t relay_name, PDU_RELAY_STAT_t level)
{
	switch(relay_name)
	{
		case PDU_RELAY_KBAT : level?(XMC_GPIO_SetOutputLevel(P1_4, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
								    (XMC_GPIO_SetOutputLevel(P1_4, XMC_GPIO_OUTPUT_LEVEL_LOW));
						  	  break;
		case PDU_RELAY_KMP 	: level?(XMC_GPIO_SetOutputLevel(P1_5, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
									(XMC_GPIO_SetOutputLevel(P1_5, XMC_GPIO_OUTPUT_LEVEL_LOW));
							  break;
		case PDU_RELAY_KMF 	: level?(XMC_GPIO_SetOutputLevel(P1_6, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
									(XMC_GPIO_SetOutputLevel(P1_6, XMC_GPIO_OUTPUT_LEVEL_LOW));
							  break;
		case PDU_RELAY_KDSG	: level?(XMC_GPIO_SetOutputLevel(P1_7, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
									(XMC_GPIO_SetOutputLevel(P1_7, XMC_GPIO_OUTPUT_LEVEL_LOW));
							  break;
		default				: break;
	}
}

void PDU_ADC_Start(void)
{
	XMC_VADC_GROUP_QueueTriggerConversion(VADC_G1);
}

