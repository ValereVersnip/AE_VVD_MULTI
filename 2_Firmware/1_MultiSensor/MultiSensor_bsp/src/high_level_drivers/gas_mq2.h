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
 * @file gas_mq2.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for gas_mq2.c
 *
 */

#ifndef DEF_gas_mq2_H
#define DEF_gas_mq2_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "../low_level_drivers/lowleveldrivers.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/**
 * Struct gas_mq2 configuration data.
 */
typedef struct gas_mq2_config_t
{
	uint8_t id;													/**< Id for this device */
	adc_t *p_adc;												/**< adc device */
	uint8_t adcchannel;											/**< adc channel */
	uint16_t alarmtreshold;										/**< treshold for alarm counter */
}gas_mq2_config_t;



/**
 * Struct gas_mq2 device.
 */
typedef struct gas_mq2_t
{
	uint8_t id;													/**< Id for this device */
	adc_t *p_adc;												/**< adc device */
	uint8_t adcchannel;											/**< adc channel */
	uint16_t alarmtreshold;										/**< treshold for alarm counter */

	uint16_t latestresult;										/**< latest gas result (in mv) */
	uint16_t alarmcounter;										/**< keeps track of how many samples were above the treshold value */
}gas_mq2_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the gas mq2 sensor.
 *
 * @param p_gas gas_mq2 device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GAS_MQ2_Init(gas_mq2_t *p_gas, gas_mq2_config_t *p_config);

/**
 * Run0 function for the gas mq2 device.
 *
 * @note this function should be called periodically by higherlevel routines
 * @param p_gas gas_mq2 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GAS_MQ2_Run0(gas_mq2_t *p_gas);

/**
 * Get the latest results.
 *
 * @param p_gas gas_mq2 device
 * @param p_latestresult latest analog result (can be NULL if not needed)
 * @param p_alarmcounter latest alarmcounter count (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GAS_MQ2_GetResult(gas_mq2_t *p_gas, uint16_t *p_latestresult, uint16_t *p_alarmcounter);


/**
 * Set alarm treshold value.
 * @param p_gas gas_mq2 device
 * @param treshold alarmtreshold
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GAS_MQ2_SetTreshold(gas_mq2_t *p_gas, uint16_t treshold);


#endif
/* End of file gas_mq2.h */
