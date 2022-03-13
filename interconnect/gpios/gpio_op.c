/*
 * gpio_op.c
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#include "gpio_op.h"

void GPIO_Init(void)
{
	/* Initialize LEDs */
	XMC_GPIO_Init(P2_11, &led);
	XMC_GPIO_Init(P2_12, &led);
	XMC_GPIO_Init(P2_13, &led);

	/* Initialize DIP Switches */
	XMC_GPIO_Init(P4_2,&dipswitch_config);
	XMC_GPIO_Init(P4_3,&dipswitch_config);
	XMC_GPIO_Init(P4_4,&dipswitch_config);
	XMC_GPIO_Init(P4_5,&dipswitch_config);

	/* Initialize Start Button */
	XMC_GPIO_Init(P2_2, &button);

	/* Set all LED off */
	XMC_GPIO_SetOutputHigh(P2_11);
	XMC_GPIO_SetOutputHigh(P2_12);
	XMC_GPIO_SetOutputHigh(P2_13);


}
