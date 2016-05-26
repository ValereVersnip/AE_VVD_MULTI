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
 * @file i2c.h
 * @author TimB
 * @date 16-mei-2016
 * @brief	Header file for i2c.c
 *
 */

#ifndef DEF_i2c_H
#define DEF_i2c_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "chip.h"
#include "status.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */

#define I2C_HANDLEMEMORY_SIZE	0x20			/**< size of master handle memory size */
#define I2C_BUFFER_SIZE			100				/**< buffer size */

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */
/**
 * Enumeration for i2c.
 */
typedef enum
{
	i2c_0,				/**< I2C 0 */
	i2c_1,				/**< I2C 1 */
	i2c_2,				/**< I2C 2 */
	i2c_3,				/**< I2C 3 */
	i2c_size			/**< amount of I2C controllers */
}i2c_name_t;


/**
 * Struct for i2c configuration data.
 */
typedef struct i2c_config_t
{
	i2c_name_t i2c;									/**< i2c to initialize */
	uint32_t bitrate;								/**< bitrate eg: 100000, max 400000 --> fast speed, not supported*/
}i2c_config_t;

/**
 * Struct for i2c configuration data.
 */
typedef struct i2c_t
{
	i2c_name_t i2c;									/**< i2c to initialize */
	uint32_t bitrate;								/**< bitrate */
	I2C_HANDLE_T *i2cHandleMaster;					/**< I2C master handle and memory for ROM API */
	uint32_t HandleMemory[I2C_HANDLEMEMORY_SIZE];	/**< use a buffer size larger than the expected return value of i2c_get_mem_size() for the static I2C handle type */
	uint8_t buffer[I2C_BUFFER_SIZE];				/**< databuffer */
}i2c_t;


/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the I2C controller.
 *
 * @param p_i2c i2c device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t I2C_Init(i2c_t *p_i2c, i2c_config_t *p_config);



/**
 * Transmit and Receive data in master mode over specified i2c bus (Polling).
 *
 * This function write and/or read over the specified bus.
 *
 * @param p_i2cDevice pointer to selected i2c bus
 * @param addr slaveaddress (7 LSB) MSB is ignored
 * @param p_txData pointer to send databuffer, NULL if only receive
 * @param txLen amount of bytes to send, 0 if only receive
 * @param p_rxData pointer to receive databuffer, NULL if only send
 * @param rxLen amount of bytes to receive, 0 if only send
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t I2C_Transfer_Blocking(i2c_t *p_i2cDevice, uint8_t addr, uint8_t *p_txData, uint32_t txLen, uint8_t *p_rxData, uint32_t rxLen);


#endif
/* End of file i2c.h */
