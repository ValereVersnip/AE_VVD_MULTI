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
	uint8_t space;
	/* check if we have to wrap around or not */
	space = &p_microphone->samplebuffer[MICROPHONE_SAMPLEBUFFER_SIZE] - *p_p_pointer;

	/* if there is enough space left, increment the pointer with incamount */
	if(space >= incamount)
	{
		*p_p_pointer += incamount;
	}
	/* else we have to wrap around and then some */
	else
	{
		*p_p_pointer = &p_microphone->samplebuffer[0] + (incamount - space);
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


/**
 * Clapdetection algorithm.
 *
 * Check in a 5 sample window for one of these profiles:
 * L X H X L
 * X L H X L
 * X L H L X
 * L X H L X
 *
 * If one of these profiles is detected, we detect 1 clap.
 * subsequent claps need to be detected within the maximum time specified, otherwise ind. claps will be ignored again.
 * @param p_microphone microphone device.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t microphone_clapcheck(microphone_t *p_microphone)
{
	status_t status = status_ok;
	uint8_t i;
	uint16_t *p_temppointer;
	uint16_t clapbuffer[5];

	/* check if we have more than 5 samples for clap checking */
	while(p_microphone->clap_overrun >= 5)
	{
		/* set temppointer to the first sample */
		p_temppointer = p_microphone->p_read_clap;
		/* get samples in clapbuffer */
		for(i = 0; i < 5; i++)
		{
			clapbuffer[i] = *p_temppointer;
			microphone_incpointer(p_microphone, &p_temppointer,1);
		}
		/* check the clapbuffer for a clap */
		if
		(
			(
			( clapbuffer[2] > clapbuffer[0] && clapbuffer[2] - clapbuffer[0] > p_microphone->claptresh) ||
			( clapbuffer[2] > clapbuffer[1] && clapbuffer[2] - clapbuffer[1] > p_microphone->claptresh)
			)
			&&
			(
			( clapbuffer[2] > clapbuffer[4] && clapbuffer[2] - clapbuffer[4] > p_microphone->claptresh) ||
			( clapbuffer[2] > clapbuffer[3] && clapbuffer[2] - clapbuffer[3] > p_microphone->claptresh)
			)
		)
		{
			/* if we detect a clap, increment the clappers */
			p_microphone->clappers++;
			/* then immediately skip 5 samples */
			microphone_incpointer(p_microphone, &p_microphone->p_read_clap, 1);
			/* don't forget to decrement the clap overrun counter */
			p_microphone->clap_overrun -= 1;
			/* load the claptimeout counter */
			p_microphone->claptimeout = p_microphone->maxtimebetweenclaps/p_microphone->adcperiod;

			/* check if we have reached the needed clapamount for detection */
			if(p_microphone->clappers >= p_microphone->clapamount)
			{
				p_microphone->clapdetectioncount++;
				p_microphone->clappers = 0;
			}
		}
		/* if no clap is detected */
		else
		{
			/* check if we are not timed out */
			if(p_microphone->claptimeout)
			{
				/* decrement the claptimeout */
				p_microphone->claptimeout--;
				/* if the claptimeout reaches zero again, reset the clappers */
				if(p_microphone->claptimeout == 0)
				{
					p_microphone->clappers = 0;
				}
			}
			/* increment the clapsample pointer by 1 */
			microphone_incpointer(p_microphone, &p_microphone->p_read_clap, 1);
			/* don't forget to decrement the clap overrun counter */
			p_microphone->clap_overrun--;
		}
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
	p_microphone->maxtimebetweenclaps	= p_config->maxtimebetweenclaps;
	p_microphone->claptresh 			= p_config->claptresh;
	p_microphone->clapamount 			= p_config->clapamount;

	p_microphone->tresholddetectioncount= 0;
	p_microphone->clapdetectioncount	= 0;
	memset(p_microphone->samplebuffer,0,MICROPHONE_SAMPLEBUFFER_SIZE);
	p_microphone->p_write				= p_microphone->samplebuffer;
	p_microphone->p_read_clap			= p_microphone->samplebuffer;
	p_microphone->p_read_tresh			= p_microphone->samplebuffer;
	p_microphone->tresh_overrun			= 0;
	p_microphone->clap_overrun			= 0;
	p_microphone->abovetreshcount		= 0;
	p_microphone->clappers				= 0;
	p_microphone->claptimeout			= 0;

	return status;
}



/**
 * Run0 function for Microphone.
 *
 * This function will gather new adc samples, if available.
 * Depending on how many, the timetreshold and clap detection algorithms will be run.
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
	/* first check if there are new samples in the adc ringbuffer */
	newsamples = RingBuffer_GetCount(p_microphone->p_adc->p_ringbuffer[p_microphone->adcchannel]);
	/* if there are new samples */
	if(newsamples) /* if not zero */
	{
		/* first increment the overrun counters */
		p_microphone->tresh_overrun += newsamples;
		p_microphone->clap_overrun += newsamples;

		/* if these counters become bigger than the samplebuffer size, we have an overrun! */
		if(p_microphone->tresh_overrun > MICROPHONE_SAMPLEBUFFER_SIZE || p_microphone->clap_overrun > MICROPHONE_SAMPLEBUFFER_SIZE)
		{
			status = microphone_bufferoverflow;
		}

		if(status == status_ok)
		{
			/* if ok, move the new samples to the samplebuffer */
			for(i = 0; i < newsamples; i++)
			{
				check = RingBuffer_Pop(p_microphone->p_adc->p_ringbuffer[p_microphone->adcchannel], p_microphone->p_write);
				/* check if we indd popped 1 sample, otherwise break and raise error */
				if(check != 1)
				{
					status = microphone_pop;
					break;
				}
				/* increment or wrap around the writepointer */
				microphone_incpointer(p_microphone, &p_microphone->p_write, 1);
			}
		}

		if(status == status_ok)
		{
			/* if ok, do the treshold check */
			status = microphone_tresholdcheck(p_microphone);
		}

		if(status == status_ok)
		{
			/* if ok, do the clap check */
			status = microphone_clapcheck(p_microphone);
		}
	}
	return status;
}


/**
 * Get the last threshold and clap detection counters.
 *
 * @param p_microphone microphone device
 * @param p_tresholddetectioncount pointer to tresholddetection count (can be NULL if not needed)
 * @param p_clapdetectioncount pointer to clapdetection count (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MICROPHONE_GetResults(microphone_t *p_microphone, uint16_t *p_tresholddetectioncount, uint16_t *p_clapdetectioncount)
{
	status_t status = status_ok;
	UTILITIES_StoreInPointer(p_tresholddetectioncount, p_microphone->tresholddetectioncount);
	UTILITIES_StoreInPointer(p_clapdetectioncount, p_microphone->clapdetectioncount);
	return status;
}

/* End of file microphone.c */
