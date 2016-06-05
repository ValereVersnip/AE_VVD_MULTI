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
 * @file gas_mq2.c
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
#include "gas_mq2.h"

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
 * Initialize the gas mq2 sensor.
 *
 * @param p_gas gas_mq2 device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GAS_MQ2_Init(gas_mq2_t *p_gas, gas_mq2_config_t *p_config)
{
	status_t status = status_ok;
	/* populate struct */
	p_gas->id				= p_config->id;
	p_gas->p_adc 			= p_config->p_adc;
	p_gas->adcchannel 		= p_config->adcchannel;
	p_gas->alarmtreshold 	= p_config->alarmtreshold;

	p_gas->latestresult = 0;
	p_gas->alarmcounter = 0;

	return status;
}


/**
 * Run0 function for the gas mq2 device.
 *
 * @note this function should be called periodically by higherlevel routines
 * @param p_gas gas_mq2 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GAS_MQ2_Run0(gas_mq2_t *p_gas)
{
	status_t status = status_ok;
	uint8_t newsamples;
	uint8_t i;
	/* first check if there are new samples in the adc ringbuffer */
	newsamples = RingBuffer_GetCount(p_gas->p_adc->p_ringbuffer[p_gas->adcchannel]);
	/* if there are new samples */
	if(newsamples) /* if not zero */
	{
		for(i = 0; i < newsamples; i++)
		{
			/* pop latest sample */
			RingBuffer_Pop(p_gas->p_adc->p_ringbuffer[p_gas->adcchannel], &p_gas->latestresult);
			/* check for treshold */
			if(p_gas->latestresult > p_gas->alarmtreshold)
			{
				/* then increment the alarmcounter */
				p_gas->alarmcounter++;
			}
		}
	}
	return status;
}


/**
 * Get the latest results.
 *
 * @param p_gas gas_mq2 device
 * @param p_latestresult latest analog result (can be NULL if not needed)
 * @param p_alarmcounter latest alarmcounter count (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GAS_MQ2_GetResult(gas_mq2_t *p_gas, uint16_t *p_latestresult, uint16_t *p_alarmcounter)
{
	status_t status = status_ok;
	/* get latest result from the gas device */
	UTILITIES_StoreInPointer(p_latestresult, p_gas->latestresult);
	UTILITIES_StoreInPointer(p_alarmcounter, p_gas->alarmcounter);
	return status;
}


/* End of file gas_mq2.c */
