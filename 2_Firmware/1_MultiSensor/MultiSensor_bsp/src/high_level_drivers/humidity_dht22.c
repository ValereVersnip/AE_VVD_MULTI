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
 * @file humidity_dht22.c
 * @author TimB
 * @date 21-mei-2016
 * @brief	Routines for a Humidity DHT22 sensor.
 *
 * API for Humidity/Temperature DHT22 sensor.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "humidity_dht22.h"
#include <string.h>
#include "../library/utilities.h"

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

/**
 * Process the received data.
 *
 * Get the temperature, humidity and crc.
 * Calculate crc.
 * @param p_dht22 dth22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t humidity_dht22_processdata(humidity_dht22_t *p_dht22);


/**
 * Statemachine for dht22 communication.
 * @param p_dht22 dht22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t humidity_dht22_statemachine(humidity_dht22_t *p_dht22);
/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Process the received data.
 *
 * Get the temperature, humidity and crc.
 * Calculate crc.
 * @param p_dht22 dth22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t humidity_dht22_processdata(humidity_dht22_t *p_dht22)
{
	status_t status = status_ok;
	uint8_t i;
	uint8_t bitperiod;
	bool bit;
	uint8_t myCrc;

	/* reset values */
	p_dht22->humidity = 0;
	p_dht22->temperature = 0;
	p_dht22->crc = 0;

	/* we skip the first two samples, because the first one is the fist acknowledge falling edge, and the second is the second acknowledge falling edge */
	for(i = 2; i < HUMIDITY_DHT22_SAMPLEBUFFERSIZE; i++)
	{
		/* first determine if this bit is 1 or 0 */
		bitperiod = p_dht22->samplebuffer[i] - p_dht22->samplebuffer[i-1];
		if(bitperiod == HUMIDITY_DHT22_BITPERIOD_TH)
		{
			bit = bit;
		}

		if(bitperiod >= HUMIDITY_DHT22_BITPERIOD_TH)
		{
			bit = true;
		}
		else
		{
			bit = false;
		}

		/* first 16bits are humidity data*/
		if(i < 2 + 16)
		{
			p_dht22->humidity = UTILITIES_ShiftInRightBit_16bit(p_dht22->humidity, bit);
		}
		/* second 16bits are temperature data*/
		else if(i < 2 + 32)
		{
			p_dht22->temperature = (int16_t)UTILITIES_ShiftInRightBit_16bit((uint16_t)p_dht22->temperature, bit);
		}
		/* last 8bits are crc data*/
		else
		{
			p_dht22->crc = UTILITIES_ShiftInRightBit_16bit(p_dht22->crc, bit);
		}
	}

	/* calculate and verify crc */
	myCrc = (uint8_t)(p_dht22->humidity >> 8) + (uint8_t)(p_dht22->humidity >> 0) + (uint8_t)(p_dht22->temperature >> 8) + (uint8_t)(p_dht22->temperature >>0);
	if(myCrc != p_dht22->crc)
	{
		status = humidity_dht22_crc;
	}

	return status;
}


/**
 * Statemachine for dht22 communication.
 * @param p_dht22 dht22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t humidity_dht22_statemachine(humidity_dht22_t *p_dht22)
{
	status_t status = status_ok;
	uint32_t currenttime;

	switch(p_dht22->state)
	{
	/* IDLE STATE */
	case humidity_dht22_state_idle:
		if(p_dht22->start)
		{
			/* immdiately turn off start, and set the running flag */
			p_dht22->start = false;
			p_dht22->running = true;
			/* reset parameters */
			p_dht22->ready = false;
			memset(p_dht22->samplebuffer, 0, sizeof(p_dht22->samplebuffer));
			p_dht22->crc 			= 0;
			p_dht22->humidity 		= 0;
			p_dht22->temperature 	= 0;
			p_dht22->samplecount 	= 0;

			/* put the dataline as output, and put low */
			status = GPIO_Set_PinDir(p_dht22->gpioport, p_dht22->gpiopin, outputlow);
			if(status == status_ok)
			{
				/* take timestamp */
				SYSTICK_GetTicks(&p_dht22->timestamp);
				/* next state is start */
				p_dht22->state = humidity_dht22_state_start;
			}
		}
		break;

	/* START STATE */
	case humidity_dht22_state_start:
		/* get currenttime */
		SYSTICK_GetTicks(&currenttime);
		/* if the time for start pulse has finished */
		if(currenttime - p_dht22->timestamp > HUMIDITY_DHT22_TIME_STARTPULSE)
		{
			/* take timestamp */
			SYSTICK_GetTicks(&p_dht22->timestamp);

			if(status == status_ok)
			{
				/* next state get samples */
				p_dht22->state = humidity_dht22_state_samples;
				/* reset sample count */
				p_dht22->samplecount = 0;

				/* enable the 10us mrt and its interrupts */
				status = MRT_EnableTimer(p_dht22->mrt, true);
			}
			if(status == status_ok)
			{
				/* enable the pininterrupt routine */
				status = GPIO_PinInterrupt_Enable(p_dht22->gpiopin, true);
				/* set the data pin as input */
				status = GPIO_Set_PinDir(p_dht22->gpioport, p_dht22->gpiopin, input);
			}
		}
		break;


	/* SAMPLE STATE */
	case humidity_dht22_state_samples:
		/* get current time */
		SYSTICK_GetTicks(&currenttime);
		/* wait until 42 samples have been read */
		if(p_dht22->samplecount >= HUMIDITY_DHT22_SAMPLEBUFFERSIZE)
		{
			/* immediately stop 10us mrt */
			status = MRT_EnableTimer(p_dht22->mrt, false);
			if(status == status_ok)
			{
				/* and immediately stop the pininterrupt */
				/* enable the pininterrupt routine */
				status = GPIO_PinInterrupt_Enable(p_dht22->gpiopin, false);
			}
			if(status == status_ok)
			{
				/* go process the received data */
				p_dht22->state = humidity_dht22_state_processdata;
			}
		}
		/* if we run over time out */
		else if(currenttime - p_dht22->timestamp >= HUMIDITY_DHT22_TIME_OUT)
		{
			status = humidity_dht22_timeout;
		}
		break;



	/* PROCESS DATA STATE */
	case humidity_dht22_state_processdata:
		status = humidity_dht22_processdata(p_dht22); 	/* process the data */
		p_dht22->running = false;						/* always reset running flag */
		p_dht22->state = humidity_dht22_state_idle;		/* next state always idle */
		SYSTICK_GetTicks(&p_dht22->waitstamp);
		if(status == status_ok)
		{
			p_dht22->ready = true;		/* ready only set, if crc is correct */
		}
		break;

	}

	/* if status is not ok, reset everything, and go to idle */
	if(status != status_ok)
	{
		SYSTICK_GetTicks(&p_dht22->waitstamp);	     	/* always get new wait timestamp */
		p_dht22->running = false;						/* always reset running flag */
		p_dht22->state = humidity_dht22_state_idle;		/* next state always idle */
		p_dht22->ready = false;							/* ready only set, if crc is correct */
		p_dht22->start = false;
		memset(p_dht22->samplebuffer, 0, sizeof(p_dht22->samplebuffer));
		p_dht22->crc 			= 0;
		p_dht22->humidity 		= 0;
		p_dht22->temperature 	= 0;
		p_dht22->samplecount 	= 0;
	}
	return status;
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */


/**
 * Initialize the humidity dht22 sensor.
 *
 * This function will also setup the edge pin interrupt for the DHT22 data pin.
 * @param p_dht22 dht22 device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_Init(humidity_dht22_t *p_dht22, humidity_dht22_config_t *p_config)
{
	status_t status = status_ok;

	/* populate struct */
	p_dht22->id 			= p_config->id;
	p_dht22->gpioport 		= p_config->gpioport;
	p_dht22->gpiopin 		= p_config->gpiopin;
	p_dht22->mrt 			= p_config->mrt;

	/* set to default */
	p_dht22->start			= false;
	p_dht22->running 		= false;
	p_dht22->ready 			= false;
	memset(p_dht22->samplebuffer, 0, sizeof(p_dht22->samplebuffer));
	p_dht22->crc 			= 0;
	p_dht22->humidity 		= 0;
	p_dht22->temperature 	= 0;
	p_dht22->samplecount 	= 0;
	p_dht22->state 			= humidity_dht22_state_idle;

	SYSTICK_GetTicks(&p_dht22->timestamp);
	p_dht22->waitstamp 		= p_dht22->timestamp;

	/* disable the pininterrupt for datapin, we will only enable this when we are reading from dht22 */
	status = GPIO_PinInterrupt_Enable(p_dht22->gpiopin, false);

	return status;
}


/**
 * Run0 function for humidity dht22 sensor.
 *
 * @note This function should be called periodically from higher level routines.
 * @param p_dht22 dht22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_Run0(humidity_dht22_t *p_dht22)
{
	status_t status = status_ok;
	status = humidity_dht22_statemachine(p_dht22);
	return status;
}


/**
 * Start a conversion.
 *
 * This function will check if a conversion is already running, and will raise an error.
 * @param p_dht22 dht22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_Start(humidity_dht22_t *p_dht22)
{
	status_t status = status_ok;
	uint32_t currentTime;
	/* check if a converision is not already started */
	if(p_dht22->running)
	{
		status = humidity_dht22_running;
	}
	/* check if the timeout has passed */
	SYSTICK_GetTicks(&currentTime);
	if(currentTime - p_dht22->waitstamp < HUMIDITY_DHT22_WAIT_TIME)
	{
		status = humidity_dht22_toofast;
	}
	if(status == status_ok)
	{
		p_dht22->start = true;
	}
	return status;
}


/**
 * Falling edge data routine for reading in data into samplebuffer.
 *
 * This function should be called from falling edge pin interrupt routine.
 * @param p_dht22 dht22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_FallingEdgeRoutine(humidity_dht22_t *p_dht22)
{
	status_t status = status_ok;
	/* get 10us timestamp  */
	MRT_GetTicks(p_dht22->mrt, &p_dht22->samplebuffer[p_dht22->samplecount]);
	/* increment the samplecount */
	p_dht22->samplecount++;
	return status;
}


/**
 * Get the latest conversion results.
 *
 * This function will also clear the ready flag.
 *
 * @note Users should use this function to get the data, as this function will also clear the ready flag.
 * @param p_dht22 dht22 device
 * @param p_humidity pointer to humidity result
 * @param p_temperature pointer to temperature result
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_GetResults(humidity_dht22_t *p_dht22, uint16_t *p_humidity, int16_t *p_temperature)
{
	status_t status = status_ok;
	if(!p_dht22->ready)
	{
		status = humidity_dht22_notready;
	}
	if(status == status_ok)
	{
		*p_humidity = p_dht22->humidity;
		*p_temperature = p_dht22->temperature;
		/* also reset the ready flag */
		p_dht22->ready = false;
	}
	return status;
}


/**
 * Get the ready flag.
 * @param p_dht22 dht22 device
 * @param p_ready pointer to readyflag
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_GetReady(humidity_dht22_t *p_dht22, bool *p_ready)
{
	status_t status = status_ok;
	*p_ready = p_dht22->ready;
	return status;
}


/**
 * Get the running flag.
 * @param p_dht22 dht22 device
 * @param p_running pointer to running
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_GetRunning(humidity_dht22_t *p_dht22, bool *p_running)
{
	status_t status = status_ok;
	*p_running = p_dht22->running;
	return status;
}


/* End of file humidity_dht22.c */
