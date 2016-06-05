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
 * @file pir_hcsr501.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for pir_hcsr501.c
 *
 */

#ifndef DEF_pir_hcsr501_H
#define DEF_pir_hcsr501_H

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
 * Pir Hcsr501 configuration data.
 */
typedef struct pir_hcsr501_config_t
{
	uint8_t id;								/**< Id for this device */
	uint8_t port;							/**< gpio port for pir datapin */
	uint8_t pin;							/**< gpio pin for pir datapin */
}pir_hcsr501_config_t;


/**
 * Pir Hcsr501 device.
 */
typedef struct pir_hcsr501_t
{
	uint8_t id;								/**< Id for this device */
	uint8_t port;							/**< gpio port for pir datapin */
	uint8_t pin;							/**< gpio pin for pir datapin */

	uint16_t detectcount;					/**< keeps track of amount of motion detects */
	uint16_t detecttime;					/**< detection time (in s) */
}pir_hcsr501_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the PIR hcs501 sensor.
 *
 * @param p_pirDevice pirdevice
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PIR_HCSR501_Init(pir_hcsr501_t *p_pirDevice, pir_hcsr501_config_t *p_config);


/**
 * Rising edge data routine for datapin.
 *
 * @note This function should be called from falling edge pin interrupt routine.
 * @param pir_hcsr501_t pir device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PIR_HCSR501_RisingEdgeRoutine(pir_hcsr501_t *p_pirDevice);


/**
 * Get detectresults.
 *
 * @param p_pirDevice pirdevice
 * @param p_detectcount detectcount
 * @param p_detecttime lastdetecttime
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PIR_HCSR501_GetResults(pir_hcsr501_t *p_pirDevice, uint16_t *p_detectcount, uint16_t *p_detecttime);


#endif
/* End of file pir_hcsr501.h */
