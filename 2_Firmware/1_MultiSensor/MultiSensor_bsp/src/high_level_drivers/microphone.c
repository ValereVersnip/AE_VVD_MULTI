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
 * @file microphone.c
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
#include "microphone.h"
#include <string.h>

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
 * Increment and wrap pointers to the samplebuffer.
 *
 * @param p_microphone microphone device.
 * @param p_p_pointer pointer to be incremented or wrapped around.
 * @param incamount amount of spaces to increment
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t microphone_incpointer(microphone_t *p_microphone, uint16_t **p_p_pointer, uint8_t incamount)
{
	status_t status = status_ok;
	uint32_t space;
	/* check if we have to wrap around or not */
	space = (uint32_t)(&p_microphone->samplebuffer[MICROPHONE_SAMPLEBUFFER_SIZE-1] - *p_p_pointer);

	/* if there is enough space left, increment the pointer with incamount */
	if(space >= incamount)
	{
		*p_p_pointer += incamount;
	}
	/* else we have to wrap around and then some */
	else
	{
		*p_p_pointer = (uint16_t*)(&p_microphone->samplebuffer[0] + (incamount - space - 1));
	}
	return status;
}


/**
 * Check if we cross the treshold for the specified amount of time.
 * If so increment the timetresholdcounter.
 *
 * @param p_microphone microphone device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t microphone_tresholdcheck(microphone_t *p_microphone)
{
	status_t status = status_ok;
	uint8_t i;

	for(i = 0; i < p_microphone->tresh_overrun; i++)
	{
		/* if the sample is higher than the treshold, increment the abovetreshcount */
		if(*p_microphone->p_read_tresh > p_microphone->treshold)
		{
			p_microphone->abovetreshcount++;
		}
		/* if it is below or equal, make the count zero again */
		else
		{
			p_microphone->abovetreshcount = 0;
		}

		/* next check if we have more samples above treshold than tresholdtime */
		if(p_microphone->abovetreshcount > (p_microphone->tresholdtime / p_microphone->adcperiod))
		{
			/* then increment the timetresholdcount */
			p_microphone->tresholddetectioncount++;
			/* and make the abovetreshcount 0 again */
			p_microphone->abovetreshcount = 0;
		}
		/* don't forget to decrement the tresh overrun count */
		p_microphone->tresh_overrun--;
		/* and increment/wrap the read tresh pointer */
		microphone_incpointer(p_microphone, &p_microphone->p_read_tresh, 1);
	}
	return status;
}




/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */


/**
 * Initialize the microphone.
 * @param p_microphone microphone device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MICROPHONE_Init(microphone_t *p_microphone, microphone_config_t *p_config)
{
	status_t status = status_ok;
	/* populate the struct */
	p_microphone->id 					= p_config->id;
	p_microphone->p_adc 				= p_config->p_adc;
	p_microphone->adcchannel 			= p_config->adcchannel;
	p_microphone->adcperiod				= p_config->adcperiod;
	p_microphone->treshold 				= p_config->treshold;
	p_microphone->tresholdtime 			= p_config->tresholdtime;

	p_microphone->tresholddetectioncount= 0;

	memset(p_microphone->samplebuffer,0,MICROPHONE_SAMPLEBUFFER_SIZE);
	p_microphone->p_write				= p_microphone->samplebuffer;
	p_microphone->tresh_overrun			= 0;
	p_microphone->abovetreshcount		= 0;
	p_microphone->latestresult			= 0;

	return status;
}



/**
 * Run0 function for Microphone.
 *
 * This function will gather new adc samples, if available.
 * Depending on how many, the timetreshold algorithms will be run.
 *
 * @note this function should be called periodically by higherlevel routines.
 * @param p_microphone
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MICROPHONE_Run0(microphone_t *p_microphone)
{
	status_t status = status_ok;
	uint8_t newsamples;
	uint8_t i;
	uint8_t check;
	uint32_t average;
	uint16_t new;

	/* first check if there are new samples in the adc ringbuffer */
	newsamples = RingBuffer_GetCount(p_microphone->p_adc->p_ringbuffer[p_microphone->adcchannel]);
	/* if there are new samples */
	if(newsamples >= MICROPHONE_AVERAGE_SAMPLES) /* if enough samples are available for averaging */
	{
		average = 0;
		for(i=0; i < MICROPHONE_AVERAGE_SAMPLES; i++)
		{
			/* get new sample */
			check = RingBuffer_Pop(p_microphone->p_adc->p_ringbuffer[p_microphone->adcchannel], &new);
			/* check if we indd popped 1 sample, otherwise break and raise error */
			if(check != 1)
			{
				status = microphone_pop;
				break;
			}
			/* and accumulate */
			average += (uint32_t)new;
		}

		if(status == status_ok)
		{
			/* and then divide to get the average */
			average /= MICROPHONE_AVERAGE_SAMPLES;

			/* first increment the overrun counters */
			p_microphone->tresh_overrun++;

			/* if these counters become bigger than the samplebuffer size, we have an overrun! */
			if(p_microphone->tresh_overrun > MICROPHONE_SAMPLEBUFFER_SIZE)
			{
				status = microphone_bufferoverflow;
			}
		}

		if(status == status_ok)
		{
			/* write the average sample to the samplebuffer */
			*p_microphone->p_write = average;

			/* and also, always store in latest average sample sample */
			p_microphone->latestresult = *p_microphone->p_write;

			/* increment or wrap around the writepointer */
			microphone_incpointer(p_microphone, &p_microphone->p_write, 1);
		}


		if(status == status_ok)
		{
			/* if ok, do the treshold check */
			status = microphone_tresholdcheck(p_microphone);
		}
	}
	return status;
}


/**
 * Get the last threshold and clap detection counters.
 *
 * @param p_microphone microphone device
 * @param p_tresholddetectioncount pointer to tresholddetection count (can be NULL if not needed)
 * @param p_latestresult pointer to latest sound value (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MICROPHONE_GetResults(microphone_t *p_microphone, uint16_t *p_tresholddetectioncount, uint16_t *p_latestresult)
{
	status_t status = status_ok;
	UTILITIES_StoreInPointer(p_tresholddetectioncount, p_microphone->tresholddetectioncount);
	UTILITIES_StoreInPointer(p_latestresult, p_microphone->latestresult);
	return status;
}



/**
 * Set the microphone treshold value.
 *
 * @param p_microphone microphone device
 * @param treshold treshold
 * @param tresholdtime tresholdtime
 * @return status_ok
 */
status_t MICROPHONE_SetTreshold(microphone_t *p_microphone, uint16_t treshold, uint16_t tresholdtime)
{
	p_microphone->treshold = treshold;
	p_microphone->tresholdtime = tresholdtime;
	return status_ok;
}


/* End of file microphone.c */
