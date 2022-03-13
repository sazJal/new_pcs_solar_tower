/*
 * pdu_op_conf.c
 *
 *  Created on: Dec 14, 2021
 *      Author: Lenovo
 */

#include "pdu_op_conf.h"

/* VADC Configuration */

/* Initialization data of VADC Global resources */
XMC_VADC_GLOBAL_CONFIG_t g_global_handle =
{
		.disable_sleep_mode_control = false,

		.clock_config =
			{
					.analog_clock_divider	= 3,
					.msb_conversion_clock	= 0,
					.arbiter_clock_divider	= 1
			},

		.class0 =
			{
					.conversion_mode_standard		= XMC_VADC_CONVMODE_12BIT,
					.sample_time_std_conv    		= 3U,
					.conversion_mode_emux			= XMC_VADC_CONVMODE_12BIT,
					.sampling_phase_emux_channel  	= 3U
			},

		.class1 =
			{
					.conversion_mode_standard		= XMC_VADC_CONVMODE_12BIT,
					.sample_time_std_conv			= 3U,
					.conversion_mode_emux			= XMC_VADC_CONVMODE_12BIT,
					.sampling_phase_emux_channel	= 3U
			},

		.data_reduction_control = 0,
		.wait_for_read_mode     = true,
		.event_gen_enable       = false,
		.boundary0              = 0,
		.boundary1              = 0
};

XMC_VADC_GROUP_CONFIG_t g_group_handle =
{
		.class0 =
			{
					.conversion_mode_standard= XMC_VADC_CONVMODE_12BIT,
					.sample_time_std_conv= 3U,
					.conversion_mode_emux= XMC_VADC_CONVMODE_12BIT,
					.sampling_phase_emux_channel= 3U
			},

		.class1 =
			{
					.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT,
					.sample_time_std_conv = 3U,
					.conversion_mode_emux= XMC_VADC_CONVMODE_12BIT,
					.sampling_phase_emux_channel= 3U
			},

		.arbitration_round_length = 0x0U,
		.arbiter_mode = XMC_VADC_GROUP_ARBMODE_ALWAYS,
		.boundary0 = 0, /* Boundary-0 */
		.boundary1 = 0, /* Boundary-1 */

		.emux_config =
			{
					.emux_mode = XMC_VADC_GROUP_EMUXMODE_SWCTRL,
					.stce_usage = 0,
					.emux_coding = XMC_VADC_GROUP_EMUXCODE_BINARY,
					.starting_external_channel = 0,
					.connected_channel = 0
			}
};

XMC_VADC_QUEUE_CONFIG_t g_queue_handle =
{
  .req_src_priority = (uint8_t)3, /* Highest Priority = 3, Lowest = 0 */
  .conv_start_mode= XMC_VADC_STARTMODE_WFS,
#if VADC_HARDWARE_TRIGGER
  .external_trigger = (bool) true, /* External Hardware trigger enabled*/
  .trigger_signal = XMC_CCU_40_SR2,
  .trigger_edge = XMC_VADC_TRIGGER_EDGE_RISING,
#else
  .external_trigger = (bool) false, /* software trigger enabled*/
  .trigger_signal   = 0,
  .trigger_edge     = XMC_VADC_TRIGGER_EDGE_NONE,
#endif
  .gate_signal= XMC_VADC_REQ_GT_A, // not used
  .timer_mode = (bool) false, /* No timer mode */
};

XMC_VADC_QUEUE_ENTRY_t g_queue_entry[VADC_QUEUE_MAX] =
{
		//Queue 0
		{
		 .channel_num		 = 4,	  /* Set Channel 4 (P2_9) */
		 .refill_needed 	 = true,  /* Refill is needed */
		 .generate_interrupt = false,
		 .external_trigger 	 = true   /* External trigger is required */
		},

		/*
		 *  even when the trigger is not needed we need to mention it as true so that we will be able to control the execution
		 *  of the queue. if WFR mode is enabled then the next conversion of the same queue entry will stall the entire
		 *  execution.
		 *  Since there is a small delay for inserting the entry-1, entry-0 will go for conversion again this has
		 *  wait for read mode enabled in the ISR thus it will stall the ADC completely
		 */

		//Queue 1
		{
		 .channel_num		 = 5,	  /* Set Channel 5 (P2_3) */
		 .refill_needed 	 = true,  /* Refill is needed */
		 .generate_interrupt = false,
		 .external_trigger 	 = false  /* in WFR mode, will be stalled waiting the previous queue */
		},

		//Queue 2
		{
		 .channel_num		 = 6,	  /* Set Channel 6 (P2_4)*/
		 .refill_needed 	 = true,  /* Refill is needed */
		 .generate_interrupt = false,
		 .external_trigger 	 = false  /* in WFR mode, will be stalled waiting the previous queue */
		},

		//Queue 3
		{
		 .channel_num		 = 7,	  /* Set Channel 7 (P2_5)*/
		 .refill_needed 	 = true,  /* Refill is needed */
		 .generate_interrupt = true,  /* Generate Service Request for interrupt */
		 .external_trigger 	 = false  /* in WFR mode, will be stalled waiting the previous queue */
		},
};

XMC_VADC_CHANNEL_CONFIG_t g_channel_handle [VADC_CH_MAX] =
{
		// Channel 4
		{
		  .channel_priority              = 1U, // in bool format Only non priority channels can be converted by Background Request Source
		  .input_class                   = XMC_VADC_CHANNEL_CONV_GROUP_CLASS1,
		  .alias_channel                 = (uint8_t)-1, //  Force the value to \b(int8_t)-1 to bypass alias feature.
		  .bfl                           = 0,
		  .event_gen_criteria            = XMC_VADC_CHANNEL_EVGEN_NEVER,  // event is not generated
		  .alternate_reference           = XMC_VADC_CHANNEL_REF_INTREF,
		  .result_reg_number             = (uint8_t) 0,						// Store in Result register 0
		  .sync_conversion  			 = false,                          /* Sync Feature disabled*/
		  .result_alignment              = XMC_VADC_RESULT_ALIGN_RIGHT,
		  .use_global_result             = false,
		  .broken_wire_detect_channel    = false,
		  .broken_wire_detect            = false
		},

		// Channel 5
		{
		  .channel_priority              = 1U, // in bool format Only non priority channels can be converted by Background Request Source
		  .input_class                   = XMC_VADC_CHANNEL_CONV_GROUP_CLASS1,
		  .alias_channel                 = (uint8_t)-1, //  Force the value to \b(int8_t)-1 to bypass alias feature.
		  .bfl                           = 0,
		  .event_gen_criteria            = XMC_VADC_CHANNEL_EVGEN_NEVER,  // event is not generated
		  .alternate_reference           = XMC_VADC_CHANNEL_REF_INTREF,
		  .result_reg_number             = (uint8_t) 1,						// Store in Result register 0
		  .sync_conversion  			 = false,                          /* Sync Feature disabled*/
		  .result_alignment              = XMC_VADC_RESULT_ALIGN_RIGHT,
		  .use_global_result             = false,
		  .broken_wire_detect_channel    = false,
		  .broken_wire_detect            = false
		},

		// Channel 6
		{
		  .channel_priority              = 1U, // in bool format Only non priority channels can be converted by Background Request Source
		  .input_class                   = XMC_VADC_CHANNEL_CONV_GROUP_CLASS1,
		  .alias_channel                 = (uint8_t)-1, //  Force the value to \b(int8_t)-1 to bypass alias feature.
		  .bfl                           = 0,
		  .event_gen_criteria            = XMC_VADC_CHANNEL_EVGEN_NEVER,	// event is not generated
		  .alternate_reference           = XMC_VADC_CHANNEL_REF_INTREF,
		  .result_reg_number             = (uint8_t) 2,								// Store in Result register 1
		  .sync_conversion  			 = false,                          /* Sync Feature disabled*/
		  .result_alignment              = XMC_VADC_RESULT_ALIGN_RIGHT,
		  .use_global_result             = false,
		  .broken_wire_detect_channel    = false,
		  .broken_wire_detect            = false
		},

		// Channel 7
		{
		  .channel_priority              = 1U, // in bool format Only non priority channels can be converted by Background Request Source
		  .input_class                   = XMC_VADC_CHANNEL_CONV_GROUP_CLASS1,
		  .alias_channel                 = (uint8_t)-1, //  Force the value to \b(int8_t)-1 to bypass alias feature.
		  .bfl                           = 0,
		  .event_gen_criteria            = XMC_VADC_CHANNEL_EVGEN_ALWAYS,  	// event is generated
		  .alternate_reference           = XMC_VADC_CHANNEL_REF_INTREF,
		  .result_reg_number             = (uint8_t) 3,								// Store in Result register 3
		  .sync_conversion  			 = false,                          /* Sync Feature disabled*/
		  .result_alignment              = XMC_VADC_RESULT_ALIGN_RIGHT,
		  .use_global_result             = false,
		  .broken_wire_detect_channel    = false,
		  .broken_wire_detect            = false
		}
};

XMC_VADC_RESULT_CONFIG_t g_result_handle [VADC_RES_MAX] =
{
		// Result register 0
		{
			.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
			.data_reduction_control = 0,
			.part_of_fifo           = false, /* No FIFO */
			.wait_for_read_mode     = true, /* WFS */
			.event_gen_enable       = false  /* No result event */
		},

		// Result register 1
		{
			.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
			.data_reduction_control = 0,
			.part_of_fifo           = false, /* No FIFO */
			.wait_for_read_mode     = true, /* WFS */
			.event_gen_enable       = false  /* No result event */
		},

		// Result register 2
		{
			.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
			.data_reduction_control = 0,
			.part_of_fifo           = false, /* No FIFO */
			.wait_for_read_mode     = true, /* WFS */
			.event_gen_enable       = false  /* No result event */
		},

		// Result register 3
		{
			.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
			.data_reduction_control = 0,
			.part_of_fifo           = false, /* No FIFO */
			.wait_for_read_mode     = true, /* WFS */
			.event_gen_enable       = true  /* Generated result event */
		}
};

XMC_GPIO_CONFIG_t adc_io =
{
		.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
};

XMC_GPIO_CONFIG_t relay_KMP =
{
		.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW
};

XMC_GPIO_CONFIG_t relay_KMF =
{
		.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW
};

XMC_GPIO_CONFIG_t relay_KDSG =
{
		.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW
};

XMC_GPIO_CONFIG_t relay_KBAT =
{
		.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW
};

