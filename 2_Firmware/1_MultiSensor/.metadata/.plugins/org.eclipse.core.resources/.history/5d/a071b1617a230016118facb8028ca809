 /*            #                                                   
             #@#@                                                   
          @@@#@@@                                                   
          @ `@@                                                     
          @@                                                        
          @@@                                                       
      .    @@                                                       
     @@@@. @@    @@          @@        @@,@;       @@  :@@@@@@@     
    @@@@@@@'@   @@@          #@`      '@  @@       @@  :@   .@@@.   
    @@@@@@@ @:  @@@           @@      @@  @@      `@:  :@`     @@   
    @@@@@@@@@@@@@@@           @@      @+  .@'     @@   :@`      @@  
    @@@@@@@@@@@@@@@           +@`    +@    @@     @@   :@`      @@  
    @@@@@@@@@@@@@@@            @@    @@    @@    `@.   :@`      @@  
    @@@@@@@@@@@@@@@            @@    @'     @#   @@    :@`      #@  
    @@@@@@@@@@@@@@@,           ;@.  +@      @@   @@    :@`      @@  
   @@@@@@@@@@@@@@@@@            @@  @@      #@  `@`    :@`      @@  
   @@@@@@@@@@@@@@@@@@           @@  @:       @# @@     :@`      @@  
  '@@@@@@@@@@@@@@@@@@           ,@,+@        @@ @@     :@`     ;@+  
  '@@@@+@@@@@@@@@@@@@            @@@@        +@`@      :@`    ,@@   
   @@@  @@@@@@ @@@@@#            @@@.         @@@      :@@@@@@@@    
        @@@@@@  :+:              `@@          @@+      `@@@@@@      
        @@@@@@                                                      
        @@@@@@                                                      
 */

/**
 * Valere Versnip Design 
 *
 * @project MultiSensor_bsp
 * @file pinmux.c
 * @author TimB
 * @date 16-mei-2016
 * @brief	Pinmuxing for LPC824.
 *
 * Because the LPC824 has very flexible pinmuxing (hw and sw) we created this file.
 * It sets the pinmuxing for the Multisensor Project.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "pinmux.h"
#include "chip.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Local Members
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Configure the pinmuxing for the MultiSensor board.
 *
 * only the special functions are set here, default the pins are gpio.
 */
void PINMUX_Init()
{
	/* Enable the clock to the Switch Matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);

	/* DISconnect the XTALIN and XTALOUT signals to port pins(P0.8, P0.9), as these will be used for gpio purposes */
	/* these are fixed location pins */
	Chip_SWM_FixedPinEnable(SWM_FIXED_XTALIN, false);
	Chip_SWM_FixedPinEnable(SWM_FIXED_XTALOUT, false);

	/* Connect the I2C_SDA0 and I2C_SCL0 signals to port pins(P0.10, P0.11) */
	/* these are fixed location pins */
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SDA);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SCL);

	/* Connect the ADC0 (P0.7) and ADC1 (P0.6) */
	/* these are fixed location pins */
	Chip_SWM_EnableFixedPin(SWM_FIXED_ADC0);
	Chip_SWM_EnableFixedPin(SWM_FIXED_ADC1);

	/* Connect the USART0 TX & RX signals to port pins(P0.16, P0.26) */
	/* these are movable location pins */
	Chip_SWM_MovablePinAssign(SWM_U0_TXD_O, 16);
	Chip_SWM_MovablePinAssign(SWM_U0_RXD_I, 26);

	/* Connect the USART1 RX signal to port pins(P0.25) */
	/* these are movable location pins */
	Chip_SWM_MovablePinAssign(SWM_U1_RXD_I, 25);

	/* Connect the SCT OUTPUT 0 (PWM)  signal to port pin(P0.0) --> this is the buzzer*/
	/* these are movable location pins */
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT0_O, 0);

	/* Connect the SCT OUTPUT 1,2,3 (PWM : RGB)  signal to port pin(P0.1, P0.9, P0.8) --> this is the RGB led*/
	/* these are movable location pins */
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT1_O, 1);
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT2_O, 9);
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT3_O, 8);

	/* make sure to disable fixed pin functionality for HUMIDITY dht22 data pin */
	Chip_SWM_DisableFixedPin(SWM_FIXED_ADC10);
	//Chip_SWM_MovablePinAssign(SWM_SCT_IN1_I, 13);


	/* Disable the clock to the Switch Matrix to save power */
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}


/* End of file pinmux.c */
