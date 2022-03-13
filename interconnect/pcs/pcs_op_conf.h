/*
 * pcs_op_conf.h
 *
 *  Created on: Nov 23, 2021
 *      Author: Lenovo
 */

#ifndef INTERCONNECT_PCS_PCS_OP_CONF_H_
#define INTERCONNECT_PCS_PCS_OP_CONF_H_

#include "xmc_ccu4.h"
#include "xmc_uart.h"
#include "xmc_gpio.h"
#include "xmc_i2c.h"
#include "xmc_eru.h"
#include "xmc_scu.h"
#include "xmc_can.h"

#define CAN_FREQ	11000000UL
#define CAN_BAUD	500000UL

#define CAN0_TXD	P0_5
#define CAN0_RXD	P0_4

XMC_GPIO_CONFIG_t pcs_button_config;
XMC_GPIO_CONFIG_t CAN0_txd_config;
XMC_GPIO_CONFIG_t CAN0_rxd_config;
XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t can_baud;
XMC_CAN_MO_t msgObjToPCS;
XMC_CAN_MO_t msgObjFromPCS;

#endif /* INTERCONNECT_PCS_PCS_OP_CONF_H_ */
