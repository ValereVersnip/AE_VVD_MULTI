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
 * @file highleveldrivers.h
 * @author TimB
 * @date 20-mei-2016
 * @brief	Header file for highleveldrivers.c
 *
 *  This header makes all High level device structs external, so they can be accessed by higherlevel routines.
 *	This header also makes initilization, runtimefunctions public.
 */

#ifndef DEF_highleveldrivers_H
#define DEF_highleveldrivers_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "../low_level_drivers/lowleveldrivers.h"
#include "address.h"
#include "buzzer.h"
#include "gas_mq2.h"
#include "humidity_dht22.h"
#include "lum_tsl2561.h"
#include "microphone.h"
#include "pir_hcsr501.h"
#include "pressure_bmp280.h"
#include "rgbled.h"
#include "temp_ds18b20.h"
#include "color.h"


/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */

/*
 * ***********************************************************************************************************************************************
 * external device declaration
 * ***********************************************************************************************************************************************
 */

/* rgbled devices */
extern rgbled_t RgbLed_D2;					/**< RGBLED D2 */

/* address device */
extern address_t Address;					/**< Address */

/* humidity dht22 device */
extern humidity_dht22_t HumidityDht22_U5;	/**< Humidity DHT22 senor U5 */

/* pressure bmp280 device */
extern pressure_bmp280_t PressureBmp280_M5;	/**< Pressure BMP280 sensor M5 */

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize all highlevel drivers.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HIGHLEVELDRIVERS_Init();


/**
 * highleveldrivers Run0 Function.
 *
 * @note This function should be called periodically by higher level routines.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HIGHLEVELDRIVERS_Run0();

#endif
/* End of file highleveldrivers.h */
