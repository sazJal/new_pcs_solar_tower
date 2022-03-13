/*
 * pdu_op_conf.h
 *
 *  Created on: Dec 14, 2021
 *      Author: Lenovo
 */

#ifndef INTERCONNECT_PDU_PDU_OP_CONF_H_
#define INTERCONNECT_PDU_PDU_OP_CONF_H_

#include "xmc_vadc.h"
#include "xmc_gpio.h"

#define VADC_QUEUE_MAX 4
#define VADC_CH_MAX    4
#define VADC_RES_MAX   4

#define VADC_HARDWARE_TRIGGER 0

/* VADC Configuration */

/* Initialization data of VADC Global resources */
XMC_VADC_GLOBAL_CONFIG_t g_global_handle;
XMC_VADC_GROUP_CONFIG_t g_group_handle;
XMC_VADC_QUEUE_CONFIG_t g_queue_handle;
XMC_VADC_QUEUE_ENTRY_t g_queue_entry[VADC_QUEUE_MAX];
XMC_VADC_CHANNEL_CONFIG_t g_channel_handle [VADC_CH_MAX];
XMC_VADC_RESULT_CONFIG_t g_result_handle [VADC_RES_MAX];

XMC_GPIO_CONFIG_t adc_io;

XMC_GPIO_CONFIG_t relay_KMP;
XMC_GPIO_CONFIG_t relay_KMF;
XMC_GPIO_CONFIG_t relay_KDSG;
XMC_GPIO_CONFIG_t relay_KBAT;

#endif /* INTERCONNECT_PDU_PDU_OP_CONF_H_ */
