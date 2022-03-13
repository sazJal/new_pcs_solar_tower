/*
 * esp32_op_conf.c
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#include "esp32_op_conf.h"

XMC_GPIO_CONFIG_t tx_config 	=
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6,
};

XMC_GPIO_CONFIG_t rx_config	=
{
	.mode = XMC_GPIO_MODE_INPUT_PULL_UP,
};

XMC_UART_CH_CONFIG_t uart_config =
{
		.baudrate	= 9600,
		.data_bits	= 8,
		.stop_bits	= 1
};
