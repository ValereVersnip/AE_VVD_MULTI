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
 * @file pir_hcsr501.c
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
#include "pir_hcsr501.h"

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
 * Initialize the PIR hcs501 sensor.
 *
 * @param p_pirDevice pirdevice
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PIR_HCSR501_Init(pir_hcsr501_t *p_pirDevice, pir_hcsr501_config_t *p_config)
{
	status_t status = status_ok;
	/* populate struct */
	p_pirDevice->id = p_config->id;
	p_pirDevice->port = p_config->port;
	p_pirDevice->pin = p_config->pin;

	p_pirDevice->detectcount = 0;
	p_pirDevice->detecttime = 0;

	return status;
}


/**
 * Rising edge data routine for datapin.
 *
 * @note This function should be called from falling edge pin interrupt routine.
 * @param pir_hcsr501_t pir device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PIR_HCSR501_RisingEdgeRoutine(pir_hcsr501_t *p_pirDevice)
{
	status_t status = status_ok;
	uint32_t currentTime;
	/* get ms timestamp  */
	SYSTICK_GetTicks(&currentTime);
	/* we divide by 1000, because we want second ticks */
	p_pirDevice->detecttime = (uint16_t)(currentTime/1000);
	/* increment the samplecount */
	p_pirDevice->detectcount++;
	return status;
}

/**
 * Get detectresults.
 *
 * @param p_pirDevice pirdevice
 * @param p_detectcount detectcount
 * @param p_detecttime lastdetecttime
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PIR_HCSR501_GetResults(pir_hcsr501_t *p_pirDevice, uint16_t *p_detectcount, uint16_t *p_detecttime)
{
	status_t status = status_ok;
	if(status == status_ok)
	{
		UTILITIES_StoreInPointer(p_detectcount, p_pirDevice->detectcount);
		UTILITIES_StoreInPointer(p_detecttime, p_pirDevice->detecttime);
	}
	return status;
}



/* End of file pir_hcsr501.c */
