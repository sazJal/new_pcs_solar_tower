/*
 * esp32_op_conf.h
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#ifndef ESP32_ESP32_OP_CONF_H_
#define ESP32_ESP32_OP_CONF_H_

#include "xmc_gpio.h"
#include "xmc_uart.h"
#include "xmc_scu.h"
#include "xmc_rtc.h"
#include "xmc_eru.h"
#include "xmc_i2c.h"

XMC_GPIO_CONFIG_t tx_config;
XMC_GPIO_CONFIG_t rx_config;

XMC_UART_CH_CONFIG_t uart_config;

#endif /* ESP32_ESP32_OP_CONF_H_ */
