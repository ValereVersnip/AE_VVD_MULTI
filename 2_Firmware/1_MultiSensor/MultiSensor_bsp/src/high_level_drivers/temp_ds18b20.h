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
 * @file temp_ds18b20.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for temp_ds18b20.c
 *
 */

#ifndef DEF_temp_ds18b20_H
#define DEF_temp_ds18b20_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "../low_level_drivers/lowleveldrivers.h"
#include "onewire.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */

#define TEMP_DS18B20_MAXIMUMDATABYTES		9							/**< the maximum amount of databytes that can be read from the device */

/* ROM commands */
#define TEMP_DS18B20_READ_ROM				0x33						/**< READ_ROM COMMAND CODE */
#define TEMP_DS18B20_SKIP_ROM				0xCC						/**< SKIP_ROM COMMAND CODE */

/* PAR COMMANDS */
#define TEMP_DS18B20_CONVERT_T				0x44						/**< CONVERT T COMMAND CODE */
#define TEMP_DS18B20_READ_SCRATCHPAD		0xBE						/**< READ SCRATCHPAD COMMAND CODE */

#define TEMP_DS18B20_CONVERSION				800							/**< Maximum delay time for conversion (in ms) */

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/**
 * Enum for lum_tsl2561 gain.
 */
typedef enum
{
	temp_ds18b20_state_idle,								/**< idle state */
	temp_ds18b20_state_sendreset,							/**< send the reset command */
	temp_ds18b20_state_sendskiprom,							/**< send the skip rom command */
	temp_ds18b20_state_sendconvert,							/**< send the convert t command */
	temp_ds18b20_state_waitforconversion,					/**< wait for the conversion to finish */
	temp_ds18b20_state_waitforoktoreadout,					/**< higherlevel routine should give an explicit OK to read out, this way we can control when the reading out takes place in time */
	temp_ds18b20_state_sendreset2,							/**< send the second reset command */
	temp_ds18b20_state_sendskiprom2,						/**< send second skip rom command */
	temp_ds18b20_state_sendreadscratchpad,					/**< send the read scratchpad command */
	temp_ds18b20_state_readscratchpad,						/**< read the 9 scratchpad bytes */
	temp_ds18b20_state_crc,									/**< do a crc check */
	temp_ds18b20_state_store,								/**< store conversion result */
} temp_ds18b20_state_t;


/**
 * temp_ds18b20 configuration data.
 */
typedef struct temp_ds18b20_config_t
{
	uint8_t id;												/**< id */
	onewire_t *p_onewire;									/**< pointer to onewire device */
}temp_ds18b20_config_t;


/**
 * temp_ds18b20 device.
 */
typedef struct temp_ds18b20_t
{
	uint8_t id;												/**< id */
	onewire_t *p_onewire;									/**< pointer to onewire device */
	uint64_t serialnumber;									/**< unique 48-bit serial number (MSB not used)*/
	int16_t temperature_int;								/**< integer part of temperature  */
	uint16_t temperature_frac;								/**< fractional part of temperature (4DIGITS AFTER COMMA) */
	uint8_t data[TEMP_DS18B20_MAXIMUMDATABYTES];			/**< dataarray for buffering incoming data */
	temp_ds18b20_state_t state;								/**< state for conversion statemachine */
	bool start;												/**< true: start a conversion, false: don't start conversion */
	bool running;											/**< true: conversion is running, false: conversion is not running */
	bool ready;												/**< true: conversion is finished */
	bool oktoreadout;										/**< true: when the physical device has data ready, we can move on to reading out the scratchpad (in the statemachine) */
	uint32_t timestamp;										/**< timestamp for timeout check */
}temp_ds18b20_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */
/**
 * Initialize the temp_ds18b20 device.
 *
 * @note we will get the devices unique 48bit serial code over onewire, and store it in the struct as well.
 * @param p_temp
 * @param p_config
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_Init(temp_ds18b20_t *p_temp, temp_ds18b20_config_t *p_config);

/**
 * Get the serialnumber over onewire.
 *
 * @param p_temp temp_ds18b20 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_GetSerialNumber(temp_ds18b20_t *p_temp);


/**
 * Start a conversion cycle.
 *
 * @param p_temp temp_ds18b20 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_Start(temp_ds18b20_t *p_temp);


/**
 * Get the last converted result.
 *
 * @note this function will immediately reset the ready flag
 * @param p_temp temp_ds18b20 device
 * @param p_temperature_int pointer to integer result (can be NULL if not needed)
 * @param p_temperature_frac pointer to fractional result (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_GetResult(temp_ds18b20_t *p_temp, int16_t *p_temperature_int, uint16_t *p_temperature_fraq);


/**
 * Run0 function for the temp_ds18b20 device.
 *
 * @note this function should be called periodically by higherlevel routines
 * @param p_temp temp_ds18b20 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_Run0(temp_ds18b20_t *p_temp);


/**
 * Get the ready flag.
 *
 * @param p_temp temp ds18b20 device
 * @param p_ready pointer to result
 * @return status_ok
 */
status_t TEMP_DS18B20_GetReadyFlag(temp_ds18b20_t *p_temp, bool *p_ready);


/**
 * Get the running flag.
 *
 * @param p_temp temp ds18b20 device
 * @param p_running pointer to result
 * @return status_ok
 */
status_t TEMP_DS18B20_GetRunningFlag(temp_ds18b20_t *p_temp, bool *p_running);


/**
 * Signal the statemachine, that it is OK to start reading the scratchpad, so we can control when to do this in time.
 * @param p_temp temp ds18b20 device
 * @return status_ok
 */
status_t TEMP_DS18B20_OkToReadOut(temp_ds18b20_t *p_temp);


#endif
/* End of file temp_ds18b20.h */
