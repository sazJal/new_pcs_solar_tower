/*
 * gpio_op_conf.h
 *
 *  Created on: Nov 24, 2021
 *      Author: Lenovo
 */

#ifndef INTERCONNECT_GPIOS_GPIO_OP_CONF_H_
#define INTERCONNECT_GPIOS_GPIO_OP_CONF_H_

#include "xmc_ccu4.h"
#include "xmc_uart.h"
#include "xmc_gpio.h"
#include "xmc_i2c.h"
#include "xmc_eru.h"
#include "xmc_scu.h"

/* LED, Button, and GPIO Configurations */
XMC_GPIO_CONFIG_t led;
XMC_GPIO_CONFIG_t button;
XMC_GPIO_CONFIG_t dipswitch_config;

#endif /* INTERCONNECT_GPIOS_GPIO_OP_CONF_H_ */
