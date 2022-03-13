/*
 * genix_op_conf.c
 *
 *  Created on: Nov 23, 2021
 *      Author: Lenovo
 */

#include "genix_op_conf.h"

XMC_GPIO_CONFIG_t tx_genix_config 	=
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
};

XMC_GPIO_CONFIG_t rx_genix_config	=
{
	.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
};

XMC_UART_CH_CONFIG_t uart_genix_config =
{
		.baudrate	= 9600,
		.data_bits	= 8,
		.stop_bits	= 1
};
