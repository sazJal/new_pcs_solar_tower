/*
 * main.c
 *
 *  Created on: 2022 Mar 03 10:25:10
 *  Author: Lenovo
 */



#include <xmc_common.h>
#include "variables.h"
#include "operation.h"

#define COUNT_MAX 1000
uint16_t count = 0;

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. 
 */

void SysTick_Handler(void)
{

}

int main(void)
{
  /* Create 1 kHz tick and Initialize systick handler variable */
  sysTickParamHandle.enable 	= false;
  sysTickParamHandle.isTimeOut 	= false;
  sysTickParamHandle.count		= 0;
  SysTick_Config(SystemCoreClock / 1000);

  /* Initialize Button, LED, and Dipswitches */
  GPIO_Init();

  /* Initialize ESP32 */
  ESP32_ConnectionInit();

  /* Initialization for PCS Communication */
  PCS_ConnectionInit();

  /* Initialization for Battery Communication */
  GENIX_ConnectionInit();

  /* Initialization for Power Distribution Control Peripheral */
  PDU_Relay_Init();
  PDU_SensorADC_Init();

  /* Setup Timer */
  TIMING_TimerInit();

  /* PCS Command Initialization */
  mcu_state = OP_SETUP;

  controlFlag.isAuto_mode 			 = false;
  controlFlag.isBypass_power_mode 	 = false;
  controlFlag.isChecking_peripheral  = false;
  controlFlag.isForced_shutting_down = false;
  controlFlag.isPerforming_Precharge = false;
  controlFlag.isStandby 			 = false;

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
	  /* Continuously Checking Mode and Error */
  	  OP_CheckingError(&controlFlag, &pcsSystemStatus, &pcsDCgridStatus,
  			  	  	   &pcsACinvStatus, &pcsPVconvStatus, &pcsBatconvStatus);
  	  OP_CheckingMode(&controlFlag);

  	  /* Run operational state */
  	  switch (mcu_state)
  	  {
  	  	  case OP_SETUP 	:
  	  		  mcu_state	= OP_SETUP;

			  /* Set LED Indicator for this state */
	  		  XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_LOW);
	  		  XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_HIGH);
	  		  XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_HIGH);

			  /* Perform actions for this state */
			  OP_SetupConnection(&mcu_state, &sysTickParamHandle, &controlFlag,
					  	  	  	  pduSensor.mPVPT, &pcsBmsStatus, &pcsSystemStatus);
			  break;
  	  	  case OP_TURN_ON 	:
  	  		  mcu_state	= OP_TURN_ON;

  	  		  /* Set LED Indicator for this state */
	  		  XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_HIGH);
	  		  XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_LOW);
	  		  XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_HIGH);

	  		  /* Perform actions for this state */
	  		  OP_TurnOnPCS(&mcu_state, &sysTickParamHandle, &controlFlag, &pcsBmsStatus,
	  				  	   &pcsSystemStatus);
	  		  break;
  	  	  case OP_RUNNING 	:
  	  		  mcu_state	= OP_RUNNING;

			  /* Set LED Indicator for this state */
	  		  XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_LOW);
	  		  XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_LOW);
	  		  XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_HIGH);

			  /* Perform actions for this state */
			  OP_CheckingPCSStatus(&mcu_state, &pcsSystemStatus, &sysTickParamHandle,
								   &controlFlag);
			  break;
		  case OP_STANDBY	:
			  mcu_state	= OP_STANDBY;

			  /* Set LED Indicator for this state */
	  		  XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_HIGH);
	  		  XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_HIGH);
	  		  XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_LOW);

			  /* Perform actions for this state */
			  OP_PutSystemStandby(&mcu_state, &pcsSystemStatus, &controlFlag,
					  	  	  	  &pcsBmsStatus);
			  break;
		  case OP_ERROR		:
			  mcu_state	= OP_ERROR;

			  /* Set LED Indicator for this state */
	  		  XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_LOW);
	  		  XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_HIGH);
	  		  XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_LOW);

			  /* Perform actions for this state */
			  OP_HandlingPCSWarning(&mcu_state, &pcsSystemStatus, &sysTickParamHandle,
									&controlFlag, &bmsSensor, &pcsBmsStatus);
			  break;
		  case OP_SHUTDOWN	:
			  mcu_state 	= OP_SHUTDOWN;

			  /* Set LED Indicator for this state */
	  		  XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_LOW);
	  		  XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_LOW);
	  		  XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_LOW);

			  /* Perform actions for this state */
			  OP_PutSystemShutDown(&mcu_state, &sysTickParamHandle);
			  break;
		  default			: break;
	  }
  }
}
