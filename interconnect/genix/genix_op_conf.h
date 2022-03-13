/*
 * genix_op_conf.h
 *
 *  Created on: Nov 23, 2021
 *      Author: Lenovo
 */

#ifndef INTERCONNECT_GENIX_GENIX_OP_CONF_H_
#define INTERCONNECT_GENIX_GENIX_OP_CONF_H_

#include "xmc_uart.h"
#include "xmc_gpio.h"
#include "stdio.h"
#include "string.h"
#include "xmc_scu.h"

XMC_GPIO_CONFIG_t tx_genix_config;
XMC_GPIO_CONFIG_t rx_genix_config;

XMC_UART_CH_CONFIG_t uart_genix_config;

#endif /* INTERCONNECT_GENIX_GENIX_OP_CONF_H_ */
