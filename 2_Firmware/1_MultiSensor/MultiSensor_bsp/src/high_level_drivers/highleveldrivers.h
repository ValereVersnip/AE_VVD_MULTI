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
#include "onewire.h"


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

/* lum tsl2561 device */
extern lum_tsl2561_t LumTsl2561_M4;			/**< Illumination TSL2561 sensor M4 */

/* buzzer device */
extern buzzer_t Buzzer_SPK1;				/**< Buzzer SPK1 */

/* PIR hcsr501 device */
extern pir_hcsr501_t PirHcsr501_M1;			/**< PIR Hcsr501 sensor M1 */

/* microphone device */
extern microphone_t Microphone_M6;			/**< Microphone M6 */

/* gas mq2 device */
extern gas_mq2_t GasMq2_M2;					/**< Gas mq2 sensor M2 */

/* onewire device */
extern onewire_t OneWire;					/**< Onewire device */

/* temp ds18b20 device */
extern temp_ds18b20_t Tempds18b20_U4;			/**< Temperature DS18B20 sensor U4 */

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
