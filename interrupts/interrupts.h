/*
 * interrupts.h
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#ifndef INTERRUPTS_INTERRUPTS_H_
#define INTERRUPTS_INTERRUPTS_H_

#include "variables.h"

#define esp32uart_intrpt_handler 		IRQ18_Handler
#define one_sec_intrpt_handler 			IRQ6_Handler
#define adc_intrpt_handler  			IRQ19_Handler
#define one_msec_intrpt_handler			IRQ23_Handler
#define hundred_msec_intrpt_handler 	IRQ24_Handler
#define five_hundreds_intrpt_handler 	IRQ21_Handler
#define pcsCAN_intrpt_handler  			IRQ7_Handler
#define genixrs485_intrpt_handler 		IRQ12_Handler

#endif /* INTERRUPTS_INTERRUPTS_H_ */
