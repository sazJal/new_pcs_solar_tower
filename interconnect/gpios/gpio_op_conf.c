/*
 * gpio_op_conf.c
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#include "gpio_op_conf.h"

XMC_GPIO_CONFIG_t led =
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
	.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW
};

XMC_GPIO_CONFIG_t button =
{
	.mode = XMC_GPIO_MODE_INPUT_TRISTATE
};

XMC_GPIO_CONFIG_t dipswitch_config =
{
	.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
};
