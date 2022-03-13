/*
 * pcs_op_conf.c
 *
 *  Created on: Nov 23, 2021
 *      Author: Lenovo
 */

#include "pcs_op_conf.h"

XMC_GPIO_CONFIG_t pcs_button_config =
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
};

XMC_GPIO_CONFIG_t CAN0_txd_config 	=
{
		.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
//		.mode = XMC_GPIO_MODE_OUTPUT_ALT9,
};

XMC_GPIO_CONFIG_t CAN0_rxd_config	=
{
		.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
//		.mode = XMC_GPIO_MODE_INPUT_PULL_DOWN,
};

/* CAN Bit Time*/
XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t can_baud =
{
		.can_frequency = CAN_FREQ,
		.baudrate	   = CAN_BAUD,
		.sample_point  = 6000,
		.sjw		   = 3
};

/* CAN message = CAN_MO4 */
XMC_CAN_MO_t msgObjToPCS =
{
		.can_mo_ptr 		= CAN_MO4,
		.can_priority 		= XMC_CAN_ARBITRATION_MODE_IDE_DIR_BASED_PRIO_2,
//	  .can_identifier = 0x3ff,
	  .can_id_mask= 0x7ff,
	  .can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	  .can_ide_mask = 1,

		.can_data_length 	= 8,
//		.can_data			= {0x89ABCDEF, 0x1234567},
		.can_mo_type 		= XMC_CAN_MO_TYPE_TRANSMSGOBJ
};

/* CAN message = CAN_MO2 */
XMC_CAN_MO_t msgObjFromPCS =
{
		.can_mo_ptr 		= CAN_MO2,
		.can_priority 		= XMC_CAN_ARBITRATION_MODE_IDE_DIR_BASED_PRIO_2,
		.can_identifier 	= 0x200,
		.can_id_mask		= 0x200,
		.can_id_mode 		= XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
		.can_ide_mask 		= 1,
		.can_data_length 	= 8,
		.can_mo_type 		= XMC_CAN_MO_TYPE_RECMSGOBJ
};


