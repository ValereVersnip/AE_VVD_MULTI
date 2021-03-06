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
 * @file gpio.c
 * @author TimB
 * @date 16-mei-2016
 * @brief	Low level routines for gpio access.
 *
 * Gpio initialization and low level routines.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "gpio.h"
#include "lowleveldriversconfig.h"

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

static uint8_t GpioInterruptChannel[GPIO_PIN_AMOUNT];												/**< we will map the gpio to the pininterrupt channel, interrupts are enabled */

static void (*p_PinIntrHandler[8])(void) = LOWLEVELDRIVERSCONFIG_GPIO_INTERRUPTHANDLERS;			/**< array with the pointers to the high level pin interrupt routines */

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

/**
 * Handle interrupt form pinint channel 0
 */
void PININT0_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH0);
	if(p_PinIntrHandler[0] != NULL)
	{
		(*p_PinIntrHandler[0])();
	}
}

/**
 * Handle interrupt form pinint channel 1
 */
void PININT1_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH1);
	if(p_PinIntrHandler[1] != NULL)
	{
		(*p_PinIntrHandler[1])();
	}
}

/**
 * Handle interrupt form pinint channel 2
 */
void PININT2_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH2);
	if(p_PinIntrHandler[2] != NULL)
	{
		(*p_PinIntrHandler[2])();
	}
}

/**
 * Handle interrupt form pinint channel 3
 */
void PININT3_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH3);
	if(p_PinIntrHandler[3] != NULL)
	{
		(*p_PinIntrHandler[3])();
	}
}

/**
 * Handle interrupt form pinint channel 4
 */
void PININT4_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH4);
	if(p_PinIntrHandler[4] != NULL)
	{
		(*p_PinIntrHandler[4])();
	}
}

/**
 * Handle interrupt form pinint channel 5
 */
void PININT5_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH5);
	if(p_PinIntrHandler[5] != NULL)
	{
		(*p_PinIntrHandler[5])();
	}
}

/**
 * Handle interrupt form pinint channel 6
 */
void PININT6_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH6);
	if(p_PinIntrHandler[6] != NULL)
	{
		(*p_PinIntrHandler[6])();
	}
}

/**
 * Handle interrupt form pinint channel 7
 */
void PININT7_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH7);
	if(p_PinIntrHandler[7] != NULL)
	{
		(*p_PinIntrHandler[7])();
	}
}

/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize all Gpio pins.
 *
 * If a pin is not in this define, it will be configured as input.
 * @param gpioParams array with gpio params
 * @param gpioParamsLen length of the gpioparams array
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Init(const gpio_pin_t gpioParams[], uint8_t gpioParamsLen)
{
	uint8_t i;
	uint8_t usedInterrupts = 0;	/* holds the used interrupts */

	/* Initialize GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* set direction, and level if output for al gpio pins */
	for(i = 0; i < gpioParamsLen; i++)
	{
		if(gpioParams[i].dir == input)
		{
			Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, gpioParams[i].port, gpioParams[i].pin);
		}
		else
		{
			Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, gpioParams[i].port, gpioParams[i].pin);
			if(gpioParams[i].dir == outputhigh) /* set the startup output state */
			{
				Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpioParams[i].port, gpioParams[i].pin, true);
			}
			else
			{
				Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpioParams[i].port, gpioParams[i].pin, false);
			}
		}

		/* set interrupt if needed */
		/* set the interupt */
		if(gpioParams[i].irq != none)
		{

			/* Configure interrupt channel  for the GPIO pin in SysCon block */
			Chip_SYSCTL_SetPinInterrupt(gpioParams[i].irqchannel, gpioParams[i].pin);


			switch(gpioParams[i].irq)
			{
			case levelhigh:
				Chip_PININT_SetPinModeLevel(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				break;

			case levellow:
				Chip_PININT_SetPinModeLevel(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				break;

			case edgehigh:
				Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				break;

			case edgelow:
				Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				break;

			case edgeboth:
				Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				break;

			default:
				break;
			}

			/* map the interrupt channel to this pin */
			GpioInterruptChannel[i] = gpioParams[i].irqchannel;

			usedInterrupts |= 0x01 << gpioParams[i].irqchannel;
		}
	}
	/* Enable the interrupts */
	if(usedInterrupts)
	{
		for(i = 0; i < 8; i++)
		{
			if(0x01 & (usedInterrupts >> i) ) /* if the pininterrupt channel is used */
			{
				/* Enable interrupt in the NVIC */
				Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(gpioParams[i].irqchannel));
				NVIC_ClearPendingIRQ(PININT0_IRQn + i);
				NVIC_EnableIRQ(PININT0_IRQn + i);
			}
		}
	}
	return status_ok;
}


/**
 * DeInitialize the GPIO.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_DeInit()
{
	/* DeInitializes GPIO */
	Chip_GPIO_DeInit(LPC_GPIO_PORT);
	return status_ok;
}



/**
 * Toggle GPIO pin.
 *
 * @param port GPIO port
 * @param pin GPIO pin
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Toggle_Pin(uint8_t port, uint8_t pin)
{
	Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, port, pin);
	return status_ok;
}


/**
 * Set GPIO pin.
 *
 * @param port GPIO port
 * @param pin GPIO pin
 * @param set true: high, false: low
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Set_Pin(uint8_t port, uint8_t pin, bool set)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, port, pin, set);
	return status_ok;
}

/**
 * Get GPIO Pin.
 *
 * @param port GPIO port
 * @param pin GPIO pin
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Get_Pin(uint8_t port, uint8_t pin, bool *p_result)
{
	*p_result = Chip_GPIO_GetPinState(LPC_GPIO_PORT, port, pin);
	return status_ok;
}

/**
 * Set GPIO Pin Direction.
 *
 * If output: this function will also set the appropriate level.
 * @param port GPIO port
 * @param pin GPIO pin
 * @param dir true: output,
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Set_PinDir(uint8_t port, uint8_t pin, gpio_direction_t dir)
{
	if(dir == input)
	{
		Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, port, pin, false);	/* false for input */
	}
	else
	{
		Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, port, pin, true);	/* true for output */
		if(dir == outputhigh)
		{
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, port, pin, true);
		}
		else
		{
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, port, pin, false);
		}
	}
	return status_ok;
}


/**
 * Enable/Disable the pin interrupt.
 * @param pin pin
 * @param enable true: enable interrupt, false: disable interrupt
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_PinInterrupt_Enable(uint8_t pin, bool enable)
{
	status_t status = status_ok;
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GpioInterruptChannel[pin]));
	NVIC_ClearPendingIRQ(PININT0_IRQn + GpioInterruptChannel[pin]);
	if(enable)
	{
		NVIC_EnableIRQ(PININT0_IRQn + GpioInterruptChannel[pin]);
	}
	else
	{
		NVIC_DisableIRQ(PININT0_IRQn + GpioInterruptChannel[pin]);
	}
	return status;
}


/* End of file gpio.c */
