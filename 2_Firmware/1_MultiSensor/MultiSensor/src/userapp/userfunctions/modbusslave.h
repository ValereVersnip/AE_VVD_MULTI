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
 * @project MultiSensor
 * @file modbusslave.h
 * @author TimB
 * @date 9-jun.-2016
 * @brief	Header file for modbusslave.c
 *
 */

#ifndef DEF_modbusslave_H
#define DEF_modbusslave_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "multisensor_bsp.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */

#define MODBUSSLAVE_FRAMEBUFFER_SIZE 				64					/**< SIZE FOR MODBUS FRAMEBUFFER */
#define MODBUSSLAVE_TIMER_USED						mrt_0				/**< the mrttimer used for delays in the modbus */
#define MODBUSSLAVE_TIMER_PRESCALER					50					/**< the interrupt frequency for the used timer (in uS) */
#define MODBUSSLAVE_USART_USED						usart_0				/**< the usart, this MODBUS uses */


/* modbus_reg_version bitmasks */

/* modbus_reg_status bitmasks */
#define MODBUSSLAVE_STATUS_BIT_ERR					(1 << 0)			/**< Global ERROR bit */
#define MODBUSSLAVE_STATUS_BIT_HUM_ERR				(1 << 1)			/**< Humidity ERROR bit */
#define MODBUSSLAVE_STATUS_BIT_PIR_ERR				(1 << 2)			/**< PIR ERROR bit */
#define MODBUSSLAVE_STATUS_BIT_PRESS_ERR			(1 << 3)			/**< PRESSURE ERROR bit */
#define MODBUSSLAVE_STATUS_BIT_LUM_ERR				(1 << 4)			/**< LUMINOSITY ERROR bit */
#define MODBUSSLAVE_STATUS_BIT_GAS_ERR				(1 << 5)			/**< GAS ERROR bit */
#define MODBUSSLAVE_STATUS_BIT_SOUND_ERR			(1 << 6)			/**< SOUND ERROR bit */
#define MODBUSSLAVE_STATUS_BIT_TEMP_ERR				(1 << 7)			/**< TEMPERATURE ERROR bit */



/* modbus_reg_rgbled_red_green bitmasks */
#define MODBUSSLAVE_RGBLED_RED_GREEN_BIT_RED		(0xFF << 8)			/**< RED mask */
#define MODBUSSLAVE_RGBLED_RED_GREEN_BIT_GREEN		(0xFF << 0)			/**< GREEN mask */

/* modbus_reg_rgbled_blue_dim bitmasks */
#define MODBUSSLAVE_RGBLED_BLUE_DIM_BIT_BLUE		(0xFF << 8)			/**< BLUE mask */
#define MODBUSSLAVE_RGBLED_BLUE_DIM_BIT_DIM			(0xFF << 0)			/**< DIM mask */

/* modbus_reg_rgbled_control bitmasks */
#define MODBUSSLAVE_RGBLED_CONTROL_BIT_BLON			(0x1F << 11)		/**< Blink Ontime mask */
#define MODBUSSLAVE_RGBLED_CONTROL_BIT_BLOFF		(0x1F << 6)			/**< Blink Offtime mask */
#define MODBUSSLAVE_RGBLED_CONTROL_BIT_GLINT		(0x0F << 2)			/**< GlowInterval mask */
#define MODBUSSLAVE_RGBLED_CONTROL_BIT_MODE			(0x03 << 0)			/**< Ledout mode mask */

/* modbus_reg_gas_treshold bitmasks */
#define MODBUSSLAVE_GAS_TRESHOLD_BIT_ALARM			(0x01 << 15)		/**< Alarm mask */
#define MODBUSSLAVE_GAS_TRESHOLD_BIT_TRESHOLD		(0xFFF << 0)		/**< treshold mask */

/* modbus_reg_gas_mv bitmasks */
#define MODBUSSLAVE_GAS_MV_BIT_GAS					(0xFFF << 0)		/**< Gas value mask */

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/**
 * Enumeration for modbusslave local register.
 *
 * @note Allways has to be defined!!!
 */
typedef enum
{
	modbus_reg_version,							/**< version: B11-8 = MAJOR, B7-4 = MINOR, B3-0 = CHANGE */
	modbus_reg_status,							/**<  */
	modbus_reg_rgbled_red_green,
	modbus_reg_rgbled_blue_dim,
	modbus_reg_rgbled_control,
	modbus_reg_buzzer_control,
	modbus_reg_pir_detectcount,
	modbus_reg_pir_detecttime,
	modbus_reg_humidity,
	modbus_reg_humidity_temp,
	modbus_reg_pressure_kpa,
	modbus_reg_pressure_pa,
	modbus_reg_pressure_temp,
	modbus_reg_luminosity_lux,
	modbus_reg_gas_treshold,
	modbus_reg_gas_mv,
	modbus_reg_temp_frac,
	modbus_reg_temp_int,
	modbus_reg_y_5,
	modbus_reg_z_5,
	modbus_reg_x_6,
	modbus_reg_y_6,
	modbus_reg_z_6,
	modbus_reg_x_7,
	modbus_reg_y_7,
	modbus_reg_z_7,
	modbus_reg_control_health,						/**< the master will write a 1 in the respective bitfield for every slave that is completley healthy, a 0 otherwise --> this will be broadcast*/
	modbus_reg_control_count,						/**< the master will increment a counter here */
	modbus_reg_size
}modbusslave_regmap_t;




/**
 * Structure for the modbus slave configuration.
 */
typedef struct modbusslave_config_t
{
	long baud;														/**< baudrate for modbus, this should be the same rate as the physical UART uses */
	unsigned char slaveID;											/**< slave ID */
	unsigned char TxEnablePort;										/**< TX Enable PIO port */
	unsigned char TxEnablePin;										/**< TX Enable PIO pin */
}modbusslave_config_t;


/**
 * 1D Modbus Slave register Array
 */
typedef uint16_t modbusslave_regarray_t[modbus_reg_size];


/**
 * Structure for the modbus slave.
 */
typedef struct modbusslave_t
{
	unsigned char TxEnablePort;										/**< TX Enable PIO port */
	unsigned char TxEnablePin;										/**< TX Enable PIO pin */

	unsigned char frame[MODBUSSLAVE_FRAMEBUFFER_SIZE]; 				/**< frame[] is used to receive and transmit packages.
	                                                                     The maximum number of bytes in a modbus packet is 256 bytes
	                                                                     This is limited to the serial buffer of 64 bytes */
	unsigned int T1_5; 												/**< inter character time out in microseconds */
	unsigned int T3_5; 												/**< frame delay */

	modbusslave_regarray_t register_array;							/**< slave register array */

	unsigned char broadcastFlag;									/**< broadcast flag */
	unsigned char slaveID;											/**< slave ID */
	unsigned char function;											/**< function */
	unsigned int errorCount;										/**< errorcount */

	bool justresponded;												/**< flag to indicate if we just responded to a modrequest for this ID.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 we will use this flag , so we can do blocking things right after this flag
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 this is the best timespot to do blocking things! */
}modbusslave_t;




/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Modbusslave Init
 * @param p_modbusslave modbusslave device
 * @param p_config configuration data
 */
void MODBUSSLAVE_Init(modbusslave_t *p_modbusslave, modbusslave_config_t *p_config);



/**
 * Modbusslave Run function
 * @param p_modbusslave modbusslave device
 * @return errorcount
 */
int MODBUSSLAVE_Run0(modbusslave_t *p_modbusslave);


/**
 * Get modbus slave register.
 * @param p_modbusslave modbus slave device
 * @param reg register
 * @return uint16 data
 */
uint16_t MODBUSSLAVE_GetRegister(modbusslave_t *p_modbusslave, uint8_t reg);


/**
 * Set modbus slave register
 * @param p_modbusslave modbus slave device
 * @param reg register
 * @param data data to set
 */
void MODBUSSLAVE_SetRegister(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t data);


/**
 * Get the pointer to the modbusslave registerarray.
 * @param p_modbusslave modbusslave device
 * @return pointer to modbusslave registerarray.
 */
modbusslave_regarray_t *MODBUSSLAVE_GetRegisterArray(modbusslave_t *p_modbusslave);


/**
 * Get the justresponded flag.
 *
 * @note this will immediately reset the justresponded flag in the modbus device struct.
 * @param p_modbusslave modbus device
 * @return true: just responded, false: not just responded
 */
bool MODBUSSLAVE_GetSetJustResponded(modbusslave_t *p_modbusslave);


/**
 * Set the bits specified in mask, in the specified register.
 *
 * @param p_modbusslave modbusslave device
 * @param reg register
 * @param mask mask
 */
void MODBUSSLAVE_DiscreteSet(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t mask);


/**
 * Clear the bits specified in mask, in the specified register.
 *
 * @param p_modbusslave modbusslave device
 * @param reg register
 * @param mask mask
 */
void MODBUSSLAVE_DiscreteClear(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t mask);


/**
 * Write a field in the modbus register.
 *
 * @note this function will check if the value will fit in the amount of mask spaces, and if there even is a mask.
 * @param p_modbusslave modbusslave device
 * @param reg register
 * @param fieldmask fielsmask (EG: 0x0F70)
 * @param fieldvalue value (eg: 15);
 * @return true: if succeeded, false: if somehting is wrong
 */
bool MODBUSSLAVE_WriteField(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t fieldmask, uint16_t fieldvalue);


/**
 * Read a field in the modbus register.
 *
 * @param p_modbusslave modbusslave device
 * @param reg register
 * @param fieldmask fieldmask (eg: 0x0F70)
 * @return the field value (if no valid mask --> 0)
 */
uint16_t MODBUSSLAVE_ReadField(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t fieldmask);


#endif
/* End of file modbusslave.h */
