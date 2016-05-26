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
 * @file humidity_dht22.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for humidity_dht22.c
 *
 */

#ifndef DEF_humidity_dht22_H
#define DEF_humidity_dht22_H

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

#define HUMIDITY_DHT22_SAMPLEBUFFERSIZE				42				/**< we have 42 falling edges in one dht22 data transfer */
#define HUMIDITY_DHT22_TIME_STARTPULSE				2				/**< the startpulse width (in ms) */
#define HUMIDITY_DHT22_WAIT_TIME					2000			/**< wait time between two conversions (in ms) */
#define HUMIDITY_DHT22_TIME_OUT						100				/**< timeout after conversion is started (in ms) */
#define HUMIDITY_DHT22_BITPERIOD_TH					9				/**< The threshold for determining 1 or 0 by bit period (in multiples of 10us) */




/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/**
 * Enumeration for humidity dht22 statemachine.
 */
typedef enum
{
	humidity_dht22_state_idle,								/**< idle state */
	humidity_dht22_state_start,								/**< start state */
	humidity_dht22_state_samples,							/**< get samples state */
	humidity_dht22_state_processdata,						/**< process received data state */
}humidity_dht22_state_t;


/**
 * Struct for humidity dht22 configuration data.
 */
typedef struct humidity_dht22_config_t
{
	uint8_t id;												/**< id */
	uint8_t gpioport;										/**< gpioport for data line */
	uint8_t gpiopin;										/**< gpiopin for data line */
	mrt_name_t mrt;											/**< multirate channel which will be setup for 10uS interrupts, at which we sample incoming data */
}humidity_dht22_config_t;

/**
 * Struct humidity dht22.
 */
typedef struct humidity_dht22_t
{
	uint8_t id;												/**< id */
	uint8_t gpioport;										/**< gpioport for data line */
	uint8_t gpiopin;										/**< gpiopin for data line */
	mrt_name_t mrt;											/**< multirate channel which will be setup for 10uS interrupts, at which point we sample incoming data */

	bool start;												/**< true: start a conversion, false: don't start conversion */
	bool running;											/**< true: conversion is running, false: conversion is not running */
	bool ready;												/**< true: conversion is finished */
	uint16_t samplecount;									/**< keeps track of how many samples we have */
	uint32_t samplebuffer[HUMIDITY_DHT22_SAMPLEBUFFERSIZE];	/**< buffer for us timing samples */
	uint8_t crc;											/**< buffer for crc */
	uint16_t humidity;										/**< humidity (example: 0x0292 = 658 --> Humidity = 65.8%) */
	int16_t temperature;									/**< temperature (example: 0x010D = 269 --> 26.9C
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 or negative(MSB is 1): 0x8065 --> 0x0065 = 101 --> -10.1C*/
	humidity_dht22_state_t state;							/**< state */
	uint32_t timestamp;										/**< timestamp for timedriven functions */
	uint32_t waitstamp;										/**< timestamp to keep track when we can start a new conversion (we need to leave 2s between conversions) */
}humidity_dht22_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the humidity dht22 sensor.
 * @param p_dht22 dht22 device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_Init(humidity_dht22_t *p_dht22, humidity_dht22_config_t *p_config);


/**
 * Run0 function for humidity dht22 sensor.
 *
 * @note This function should be called periodically from higher level routines.
 * @param p_dht22 dht22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_Run0(humidity_dht22_t *p_dht22);



/**
 * Start a conversion.
 *
 * This function will check if a conversion is already running, and will raise an error.
 * @param p_dht22 dht22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_Start(humidity_dht22_t *p_dht22);



/**
 * Falling edge data routine for reading in data into samplebuffer.
 *
 * This function should be called from falling edge pin interrupt routine.
 * @param p_dht22 dht22 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_FallingEdgeRoutine(humidity_dht22_t *p_dht22);


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
status_t HUMIDITY_DHT22_GetResults(humidity_dht22_t *p_dht22, uint16_t *p_humidity, int16_t *p_temperature);


/**
 * Get the ready flag.
 * @param p_dht22 dht22 device
 * @param p_ready pointer to readyflag
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HUMIDITY_DHT22_GetReady(humidity_dht22_t *p_dht22, bool *p_ready);

#endif
/* End of file humidity_dht22.h */
