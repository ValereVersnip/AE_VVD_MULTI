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
 * @file buzzer.c
 * @author TimB
 * @date 21-mei-2016
 * @brief	Brief.
 *
 * Description
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "buzzer.h"

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


/**
 * statemachine for the buzzer.
 * @param p_buzzerDevice buzzer device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t buzzer_statemachine(buzzer_t *p_buzzerDevice)
{
	status_t status = status_ok;
	uint32_t currentTime;
	switch(p_buzzerDevice->buzzerstate)
	{
	case buzzer_state_idle:
		if(p_buzzerDevice->buzzpending)
		{
			p_buzzerDevice->buzzpending = false;
			SYSTICK_GetTicks(&p_buzzerDevice->timestamp);	/* allways ok */
			status = BUZZER_Start(p_buzzerDevice, true);
			p_buzzerDevice->buzzerstate = buzzer_state_buzz;
		}
		break;

	case buzzer_state_buzz:
		if(!p_buzzerDevice->state)	/* if we are not buzzing anymore (stopped by user) */
		{
			p_buzzerDevice->buzzerstate = buzzer_state_idle;
		}
		else
		{
			SYSTICK_GetTicks(&currentTime);	/* allways ok */
			if(currentTime - p_buzzerDevice->timestamp > p_buzzerDevice->pulsewidth)
			{
				status = BUZZER_Start(p_buzzerDevice, false);
				p_buzzerDevice->buzzerstate = buzzer_state_idle;
			}
		}
		break;
	}
	return status;
}

/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize buzzer device.
 * @param p_buzzerDevice
 * @param p_buzzerConfig
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_Init(buzzer_t *p_buzzerDevice, buzzer_config_t *p_buzzerConfig)
{
	status_t status = status_ok;

	p_buzzerDevice->id = p_buzzerConfig->id;
	p_buzzerDevice->port = p_buzzerConfig->port;
	p_buzzerDevice->pin = p_buzzerConfig->pin;


	if(status == status_ok)
	{
		status = BUZZER_Start(p_buzzerDevice, false);
	}
	if(status == status_ok)
	{
		p_buzzerDevice->init = true;
	}
	return status;
}


/**
 * DeInitialize buzzer device.
 * @param p_buzzerDevice
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_DeInit(buzzer_t *p_buzzerDevice)
{
	status_t status = status_ok;
	if(p_buzzerDevice->init)
	{
		status = BUZZER_Start(p_buzzerDevice, false);
		p_buzzerDevice->init = false;
	}
	return status;
}

/**
 * Start or Stop the buzzer.
 * @param p_buzzerDevice buzzer device
 * @param start true: start buzzer, false: stop buzzer
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_Start(buzzer_t *p_buzzerDevice, bool start)
{
	status_t status = status_ok;

	status = GPIO_Set_Pin(p_buzzerDevice->port, p_buzzerDevice->pin, start);

	if(status == status_ok)
	{
		p_buzzerDevice->state = start;
		status = status_ok;
	}
	return status;
}


/**
 * start a timed buzz operation.
 *
 * This will set the buzzpending flag for the statemachine.
 * And set the time (in ms) for the pulsewidth
 * @param p_buzzerDevice buzzer device
 * @param time time to buzz in ms
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_Buzz(buzzer_t *p_buzzerDevice, uint32_t time)
{
	status_t status = status_ok;
	if(status == status_ok)
	{
		p_buzzerDevice->pulsewidth = time;
		p_buzzerDevice->buzzpending = true;
	}
	return status;
}



/**
 * Run function for the buzzer device.
 *
 * This function shoulc be called periodically by higher level functions.
 * @param p_buzzerDevice buzzer device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_Run0(buzzer_t *p_buzzerDevice)
{
	status_t status = status_ok;
	if(p_buzzerDevice->init)
	{
		status = buzzer_statemachine(p_buzzerDevice);
	}
	return status;
}




/* End of file buzzer.c */
