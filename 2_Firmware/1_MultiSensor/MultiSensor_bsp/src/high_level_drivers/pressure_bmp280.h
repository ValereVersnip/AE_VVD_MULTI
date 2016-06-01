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
 * @file pressure_bmp280.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for pressure_bmp280.c
 *
 */

#ifndef DEF_pressure_bmp280_H
#define DEF_pressure_bmp280_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "../low_level_drivers/lowleveldrivers.h"
#include "BMP280_driver/bmp280.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */

#define PRESSURE_BMP280_I2C_BUFFER_LEN						8					/**< length for I2C buffer */



/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/**
 * pressure bmp280 configuration struct
 */
typedef struct pressure_bmp280_config_t
{
	uint8_t id;													/**< id for this device */
	i2c_t *p_i2c;												/**< i2c bus to which this device is connected */
	uint8_t address;											/**< 7-bit i2c address (MSB is not used) */







}pressure_bmp280_config_t;


/**
 * pressure bmp280 Struct
 */
typedef struct pressure_bmp280_t
{
	uint8_t id;													/**< id for this device */
	i2c_t *p_i2c;												/**< i2c bus to which this device is connected */
	uint8_t address;											/**< 7-bit i2c address (MSB is not used) */

	struct bmp280_t bmp280;										/**< bosch driver bmp280 device */

	int32_t uncomp_temp;										/**< uncompensated temperature result */
	int32_t uncomp_press;										/**< umcomensated pressure result */
	int32_t comp_temp;											/**< compensated temperature result */
	uint32_t comp_press;										/**< comensated pressure result */

}pressure_bmp280_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the pressure_bmp280.
 *
 * @note We fix the following things in this function:
 * 	I2C mode, NORMAL mode,  BMP280_HIGH_RESOLUTION_MODE, 500MS standby mode.
 * @param p_pressurebmp280 pressure bmp280 device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PRESSURE_BMP280_Init(pressure_bmp280_t *p_pressurebmp280, pressure_bmp280_config_t *p_config);


/**
 * Set the internal static pointer to the specified pressure bmp280 device.
 *
 * This function should be called prior to using any other function!
 * @param p_pressurebmp280 pressure bmp280 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PRESSURE_BMP280_SetPointer(pressure_bmp280_t *p_pressurebmp280);



/**
 * Get the latest temperature and pressure values over I2C.
 *
 * This function will get the latest conversion results over I2C from the BMP280.
 * @param p_pressurebmp280 pressure bmp280 device
 * @param p_uncomp_temp uncompensated temperature result (can be NULL if not needed)
 * @param p_uncomp_press uncompensated pressure result (can be NULL if not needed)
 * @param p_comp_temp compensated temperature result (can be NULL if not needed)
 * @param p_comp_press compensated temperature result (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PRESSURE_BMP280_GetResult(pressure_bmp280_t *p_pressurebmp280, int32_t *p_uncomp_temp, int32_t *p_uncomp_press, int32_t *p_comp_temp, uint32_t *p_comp_press);


#endif
/* End of file pressure_bmp280.h */
