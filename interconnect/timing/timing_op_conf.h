/*
 * timing_op_conf.h
 *
 *  Created on: Nov 24, 2021
 *      Author: Lenovo
 */

#ifndef TOOLS_TIMING_TIMING_OP_CONF_H_
#define TOOLS_TIMING_TIMING_OP_CONF_H_

#include "xmc_ccu4.h"
#include "xmc_uart.h"
#include "xmc_gpio.h"
#include "xmc_i2c.h"
#include "xmc_eru.h"
#include "xmc_scu.h"

XMC_CCU4_SLICE_COMPARE_CONFIG_t SLICE0_config;
XMC_CCU4_SLICE_COMPARE_CONFIG_t SLICE1_config;
XMC_CCU4_SLICE_COMPARE_CONFIG_t SLICE2_config;

#endif /* TOOLS_TIMING_TIMING_OP_CONF_H_ */
