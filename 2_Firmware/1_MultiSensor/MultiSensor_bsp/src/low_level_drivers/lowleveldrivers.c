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
 * @file lowleveldrivers.c
 * @author TimB
 * @date 16-mei-2016
 * @brief	Lowleveldrivers device collection, initialization and run functions.
 *
 * All low level drivers device structs are declared here.
 * These are initialized here.
 * All low level drivers run functions are grouped here, into 1 lowleveldrivers run function, which should be called periodically by higher level routines.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "lowleveldrivers.h"
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

/* i2c devices */
i2c_t I2C0;					/**< I2C 0 Device */

/* pwm SCTs */
pwm_t Pwm;					/**< Pwm device */

/* adc device */
adc_t Adc;					/**< Adc device */

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
 * Initialize the Systick Timer.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t initialize_systick()
{
	status_t status = status_ok;
	systick_config_t sysTickConfig;
	sysTickConfig.p_handler = LOWLEVELDRIVERSCONFIG_SYSTICK_INTRHANDLER;
	sysTickConfig.rate = LOWLEVELDRIVERSCONFIG_SYSTICK_RATE;
	status = SYSTICK_Init(&sysTickConfig);

	return status;
}

/**
 * Initialize the mrttimer.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t initialize_mrt()
{
	status_t status = status_ok;
	mrt_config_t mrtConfig;
	mrtConfig.enable[0] 					= LOWLEVELDRIVERSCONFIG_MRT_0_ENABLE;
	mrtConfig.enable[1] 					= LOWLEVELDRIVERSCONFIG_MRT_1_ENABLE;
	mrtConfig.enable[2] 					= LOWLEVELDRIVERSCONFIG_MRT_2_ENABLE;
	mrtConfig.enable[3] 					= LOWLEVELDRIVERSCONFIG_MRT_3_ENABLE;

	mrtConfig.enablematchinterrupt[0] 		= LOWLEVELDRIVERSCONFIG_MRT_0_ENABLEINTERRUPT;
	mrtConfig.enablematchinterrupt[1] 		= LOWLEVELDRIVERSCONFIG_MRT_1_ENABLEINTERRUPT;
	mrtConfig.enablematchinterrupt[2] 		= LOWLEVELDRIVERSCONFIG_MRT_2_ENABLEINTERRUPT;
	mrtConfig.enablematchinterrupt[3] 		= LOWLEVELDRIVERSCONFIG_MRT_3_ENABLEINTERRUPT;

	mrtConfig.interruptfreq[0] 				= LOWLEVELDRIVERSCONFIG_MRT_0_INTERRUPTFREQ;
	mrtConfig.interruptfreq[1] 				= LOWLEVELDRIVERSCONFIG_MRT_1_INTERRUPTFREQ;
	mrtConfig.interruptfreq[2] 				= LOWLEVELDRIVERSCONFIG_MRT_2_INTERRUPTFREQ;
	mrtConfig.interruptfreq[3] 				= LOWLEVELDRIVERSCONFIG_MRT_3_INTERRUPTFREQ;

	mrtConfig.p_handler[0] 					= LOWLEVELDRIVERSCONFIG_MRT_0_INTRHANDLER;
	mrtConfig.p_handler[1] 					= LOWLEVELDRIVERSCONFIG_MRT_1_INTRHANDLER;
	mrtConfig.p_handler[2] 					= LOWLEVELDRIVERSCONFIG_MRT_2_INTRHANDLER;
	mrtConfig.p_handler[3] 					= LOWLEVELDRIVERSCONFIG_MRT_3_INTRHANDLER;


	status = MRT_Init(&mrtConfig);

	return status;
}

/**
 * Initialize the GPIO.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t initialize_gpio()
{
	status_t status = status_ok;

	const gpio_pin_t GpioParams[] = LOWLEVELDRIVERSCONFIG_GPIO_PARAMS;							/**< array with gpio initialization data (direction of pins) */
	uint8_t gpioParamsLen = sizeof(GpioParams)/sizeof(GpioParams[0]);
	status = GPIO_Init(GpioParams, gpioParamsLen);

	return status;
}


/**
 * Initialize the I2C.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t initialize_i2c()
{
	status_t status = status_ok;
	i2c_config_t i2cConfig;

	i2cConfig.i2c	  = i2c_0;
	i2cConfig.bitrate = LOWLEVELDRIVERSCONFIG_I2C0_BITRATE;
	status = I2C_Init(&I2C0, &i2cConfig);


	return status;
}


/**
 * Initialize the usart.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t initialize_usart()
{
	status_t status = status_ok;
	usart_config_t usartConfig;

	usartConfig.enable[0] 		= false;
	usartConfig.enable[1] 		= false;
	usartConfig.enable[2] 		= false;

#ifdef LOWLEVELDRIVERSCONFIG_USART0_USE
	usartConfig.enable[0] 		= true;
	usartConfig.baud[0]			= LOWLEVELDRIVERSCONFIG_USART0_BAUDRATE;
	usartConfig.parity[0]		= LOWLEVELDRIVERSCONFIG_USART0_PARITY;
	usartConfig.databits[0]		= LOWLEVELDRIVERSCONFIG_USART0_DATABITS;
	usartConfig.stopbits[0]		= LOWLEVELDRIVERSCONFIG_USART0_STOPBITS;
#endif
#ifdef LOWLEVELDRIVERSCONFIG_USART1_USE
	usartConfig.enable[1] 		= true;
	usartConfig.baud[1]			= LOWLEVELDRIVERSCONFIG_USART1_BAUDRATE;
	usartConfig.parity[1]		= LOWLEVELDRIVERSCONFIG_USART1_PARITY;
	usartConfig.databits[1]		= LOWLEVELDRIVERSCONFIG_USART1_DATABITS;
	usartConfig.stopbits[1]		= LOWLEVELDRIVERSCONFIG_USART1_STOPBITS;
#endif
#ifdef LOWLEVELDRIVERSCONFIG_USART2_USE
	usartConfig.enable[2] 		= true;
	usartConfig.baud[2]			= LOWLEVELDRIVERSCONFIG_USART2_BAUDRATE;
	usartConfig.parity[2]		= LOWLEVELDRIVERSCONFIG_USART2_PARITY;
	usartConfig.databits[2]		= LOWLEVELDRIVERSCONFIG_USART2_DATABITS;
	usartConfig.stopbits[2]		= LOWLEVELDRIVERSCONFIG_USART2_STOPBITS;
#endif


	status = USART_Init(&usartConfig);

	return status;
}


/**
 * Initialize the PWM.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t initialize_pwm()
{
	status_t status = status_ok;
	pwm_config_t pwmConfig;
	uint8_t i;
	bool usechannel[PWM_AMOUNTOFCHANNELS] = LOWLEVELDRIVERSCONFIG_PWM_USECHANNEL;

	pwmConfig.frequency = LOWLEVELDRIVERSCONFIG_PWM_FREQUENCY;

	for(i = 0; i < PWM_AMOUNTOFCHANNELS; i++)
	{
		pwmConfig.dutycycle[i] 	= LOWLEVELDRIVERSCONFIG_PWM_DUTYCYCLE;
		pwmConfig.usechannel[i] = usechannel[i];
	}

	status = PWM_Init(&Pwm, &pwmConfig);

	return status;
}


/**
 * Initialize the ADC.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t initialize_adc()
{
	status_t status = status_ok;
	adc_config_t config;

	config.usechannel[0] = LOWLEVELDRIVERSCONFIG_ADC0_USE;
	config.usechannel[1] = LOWLEVELDRIVERSCONFIG_ADC1_USE;
	config.usechannel[2] = LOWLEVELDRIVERSCONFIG_ADC2_USE;
	config.usechannel[3] = LOWLEVELDRIVERSCONFIG_ADC3_USE;
	config.usechannel[4] = LOWLEVELDRIVERSCONFIG_ADC4_USE;
	config.usechannel[5] = LOWLEVELDRIVERSCONFIG_ADC5_USE;
	config.usechannel[6] = LOWLEVELDRIVERSCONFIG_ADC6_USE;
	config.usechannel[7] = LOWLEVELDRIVERSCONFIG_ADC7_USE;
	config.usechannel[8] = LOWLEVELDRIVERSCONFIG_ADC8_USE;
	config.usechannel[9] = LOWLEVELDRIVERSCONFIG_ADC9_USE;
	config.usechannel[10] = LOWLEVELDRIVERSCONFIG_ADC10_USE;
	config.usechannel[11] = LOWLEVELDRIVERSCONFIG_ADC11_USE;


	status = ADC_Init(&Adc, &config);

	return status;
}

/**
 * Initialize the WDT.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t initialize_wdt()
{
	status_t status = status_ok;

	status = WDT_Init(LOWLEVELDRIVERSCONFIG_WDT_TIMEOUT);

	return status;
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize all lowlevel drivers.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LOWLEVELDRIVERS_Init()
{
	status_t status = status_ok;

	/* before any, Initialize the PINMUXER */
	PINMUX_Init();

	/* Initialize Systick Timer */
	if(status == status_ok)
	{
		status = initialize_systick();
	}

	/* Initialize mrt */
	if(status == status_ok)
	{
		status = initialize_mrt();
	}

	/* Initialize GPIO */
	if(status == status_ok)
	{
		status = initialize_gpio();
	}

	/* Initialize I2C */
	if(status == status_ok)
	{
		status = initialize_i2c();
	}

	/* Initialize USART */
	if(status == status_ok)
	{
		status = initialize_usart();
	}

	/* Initialize PWM */
	if(status == status_ok)
	{
		status = initialize_pwm();
	}

	/* Initialize ADC */
	if(status == status_ok)
	{
		status = initialize_adc();
	}

	/* Initialize WDT */
	if(status == status_ok)
	{
		status = initialize_wdt();
	}


	return status;
}


/**
 * lowleveldrivers Run0 Function.
 *
 * @note This function should be called periodically by higher level routines.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LOWLEVELDRIVERS_Run0()
{
	status_t status = status_ok;

	/* ADC Run0 */
	if(status == status_ok)
	{
		status = ADC_Run0(&Adc);
		/* sometimes (once in 20 minutes or so...) the adc reports data non valid, then we don't copy it to the struct, but we proceed anyway */
		if(status == adc_datanotvalid)
		{
			status = status_ok;
		}
	}


	/* WDT kick here */
	if(status == status_ok)
	{
		status = WDT_Kick();
	}

	return status;
}


/**
 * This function will be called when the MRT1 interrupts (overruns).
 *
 * At specified frequency (MRT1) we will start an ADC conversion.
 */
void LOWLEVELDRIVERS_MRT1_HANDLER()
{
	ADC_StartSequenceA(&Adc);
}





/* End of file lowleveldrivers.c */
