/*
 * timing_op_conf.c
 *
 *  Created on: Nov 24, 2021
 *      Author: Lenovo
 */

#include "timing_op_conf.h"

XMC_CCU4_SLICE_COMPARE_CONFIG_t SLICE0_config =
{
  .timer_mode 			= (uint32_t) XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
  .monoshot 			= (uint32_t) false,
  .shadow_xfer_clear 	= (uint32_t) 0,
  .dither_timer_period 	= (uint32_t) 0,
  .dither_duty_cycle 	= (uint32_t) 0,
  .prescaler_mode 		= (uint32_t) XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
  .mcm_enable 			= (uint32_t) 0,
  .prescaler_initval   	= (uint32_t) XMC_CCU4_SLICE_PRESCALER_32,
  .float_limit			= (uint32_t) 0,
  .dither_limit			= (uint32_t) 0,
  .passive_level 		= (uint32_t) XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
  .timer_concatenation	= (uint32_t) 0
};

XMC_CCU4_SLICE_COMPARE_CONFIG_t SLICE1_config =
{
  .timer_mode 			= (uint32_t) XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
  .monoshot 			= (uint32_t) false,
  .shadow_xfer_clear 	= (uint32_t) 0,
  .dither_timer_period 	= (uint32_t) 0,
  .dither_duty_cycle 	= (uint32_t) 0,
  .prescaler_mode 		= (uint32_t) XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
  .mcm_enable 			= (uint32_t) 0,
  .prescaler_initval   	= (uint32_t) XMC_CCU4_SLICE_PRESCALER_1024,
  .float_limit			= (uint32_t) 0,
  .dither_limit			= (uint32_t) 0,
  .passive_level 		= (uint32_t) XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
  .timer_concatenation	= (uint32_t) 0
};

XMC_CCU4_SLICE_COMPARE_CONFIG_t SLICE2_config =
{
  .timer_mode 			= (uint32_t) XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
  .monoshot 			= (uint32_t) false,
  .shadow_xfer_clear 	= (uint32_t) 0,
  .dither_timer_period 	= (uint32_t) 0,
  .dither_duty_cycle 	= (uint32_t) 0,
  .prescaler_mode 		= (uint32_t) XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
  .mcm_enable 			= (uint32_t) 0,
  .prescaler_initval   	= (uint32_t) XMC_CCU4_SLICE_PRESCALER_8192,
  .float_limit			= (uint32_t) 0,
  .dither_limit			= (uint32_t) 0,
  .passive_level 		= (uint32_t) XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
  .timer_concatenation	= (uint32_t) 0
};
