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
 * @file onewire.c
 * @author TimB
 * @date 26-jun.-2016
 * @brief	Brief.
 *
 * Description
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "onewire.h"


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

static uint32_t A = ((ONEWIRE_TIMING_A * 1000)/ONEWIRE_TIMING_TICK);		/**< Amount of ticks needed for A */
static uint32_t B = ((ONEWIRE_TIMING_B * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for B */
static uint32_t C = ((ONEWIRE_TIMING_C * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for C */
static uint32_t D = ((ONEWIRE_TIMING_D * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for D */
static uint32_t E = ((ONEWIRE_TIMING_E * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for E */
static uint32_t F = ((ONEWIRE_TIMING_F * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for F */
static uint32_t G = ((ONEWIRE_TIMING_G * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for G */
static uint32_t H = ((ONEWIRE_TIMING_H * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for H */
static uint32_t I = ((ONEWIRE_TIMING_I * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for I */
static uint32_t J = ((ONEWIRE_TIMING_J * 1000)/ONEWIRE_TIMING_TICK);      /**< Amount of ticks needed for J */







/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */
/**
 * INLINE function for creating 0.216 microsecond delay.
 * @param delay delay (in 0.216us ticks)
 */
inline static void onewire_delay(uint32_t delay);

/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * INLINE function for creating 0.216 microsecond delay.
 * @param delay delay (in 0.216us ticks)
 */
inline static void onewire_delay(uint32_t delay)
{
	uint32_t i;
	for(i = 0; i < delay; i++)
	{
		asm("");	/* add this so nothing gets optimized away */
	}
}






/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the onewire.
 * @param p_onewire onewire device
 * @param p_config configuration data
 * @return status_ok
 */
status_t ONEWIRE_Init(onewire_t *p_onewire, onewire_config_t *p_config)
{
	status_t status = status_ok;

	/* populate the struct */
	p_onewire->id			= p_config->id;
	p_onewire->p_lpc_gpio	= p_config->p_lpc_gpio;
	p_onewire->gpioport		= p_config->gpioport;
	p_onewire->gpiopin		= p_config->gpiopin;

	return status;
}



/**
 * Generate a 1-wire reset.
 * @param p_onewire onewire device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ONEWIRE_reset(onewire_t *p_onewire)
{
	status_t status = status_ok;
	bool presence;

	/* disable all interrupts, otherwise routine will fail */
	__disable_irq();

	/* set the pin to high */
	Chip_GPIO_SetPinState(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin, true);
	/* then set it as output */
	Chip_GPIO_SetPinDIROutput(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);

	/* G delay */
	onewire_delay(G);
	/* drive DQ low */
	Chip_GPIO_SetPinState(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin, false);
	/* H delay */
	onewire_delay(H);
	/* set DQ to input (release the bus) */
	Chip_GPIO_SetPinDIRInput(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);
	/* I delay */
	onewire_delay(I);
	/* sample the input for presence detection */
	presence = Chip_GPIO_ReadPortBit(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);
	/* J delay */
	onewire_delay(J);

	/* enable interrupts again */
	__enable_irq();

	/* If presense is false, we have detected a device, otherwise error */
	if(presence)
	{
		status = onewire_presence;
	}
	return status;
}

/**
 * Send a 1-wire write bit. Provide 10us recovery time.
 * @param p_onewire onewire device
 * @param bit true: write '1', false": write '0'
 * @return status_ok;
 */
status_t ONEWIRE_writebit(onewire_t *p_onewire, bool bit)
{
	/* disable all interrupts, otherwise routine will fail */
	__disable_irq();


	/* set dq as output */
	Chip_GPIO_SetPinDIROutput(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);

	/* if we have to write a '1' */
	if(bit)
	{
		/* drive DQ low */
		Chip_GPIO_SetPinState(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin, false);
		/* A delay */
		onewire_delay(A);
		/* set DQ to input (release the bus) */
		Chip_GPIO_SetPinDIRInput(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);
		/* B delay  (complete the timeslot and 10us recovery)*/
		onewire_delay(B);
	}
	/* if we have to write a '0' */
	else
	{
		/* drive DQ low */
		Chip_GPIO_SetPinState(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin, false);
		/* C delay */
		onewire_delay(C);
		/* set DQ to input (release the bus) */
		Chip_GPIO_SetPinDIRInput(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);
		/* D delay */
		onewire_delay(D);
	}

	/* enable interrupts again */
	__enable_irq();
	return status_ok;
}

/**
 * Read a bit from the 1wire bus. Provide 10us recovery time.
 * @param p_onewire onewire device
 * @param p_bit pointer to result
 * @return status_ok
 */
status_t ONEWIRE_readbit(onewire_t *p_onewire, bool *p_bit)
{
	/* disable all interrupts, otherwise routine will fail */
	__disable_irq();

	/* set dq as output */
	Chip_GPIO_SetPinDIROutput(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);

	/* drive DQ low */
	Chip_GPIO_SetPinState(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin, false);
	/* A delay */
	onewire_delay(A);
	/* set DQ to input (release the bus) */
	Chip_GPIO_SetPinDIRInput(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);
	/* E delay  */
	onewire_delay(E);
	/* sample the bit */
	*p_bit = Chip_GPIO_ReadPortBit(p_onewire->p_lpc_gpio, p_onewire->gpioport, p_onewire->gpiopin);
	/* F delay (complete the timeslot and provide 10us recovery time) */
	onewire_delay(F);

	/* enable interrupts again */
	__enable_irq();

	return status_ok;
}


/**
 * Write 1 byte over the 1wire bus.
 * @param p_onewire onewire device
 * @param data data byte (8bits) to write
 * @return status_ok
 */
status_t ONEWIRE_writebyte(onewire_t *p_onewire, uint8_t data)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		ONEWIRE_writebit(p_onewire, data & 0x01);	/* returns ok */
		data >>= 1;
	}
	return status_ok;
}


/**
 * Read 1 byte over the 1 wire bus.
 * @param p_onewire onewire device
 * @param p_data pointer to result
 * @return status_ok
 */
status_t ONEWIRE_readbyte(onewire_t *p_onewire, uint8_t *p_data)
{
	uint8_t i;
	uint8_t data = 0;
	bool bit;

	for(i = 0; i < 8; i++)
	{
		data >>= 1;
		ONEWIRE_readbit(p_onewire, &bit);
		if(bit)
		{
			data |= 0x80;
		}
	}
	*p_data = data;
	return status_ok;
}


/* End of file onewire.c */
