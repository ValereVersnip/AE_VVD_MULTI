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
 * @file lum_tsl2561.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for lum_tsl2561.c
 *
 */

#ifndef DEF_lum_tsl2561_H
#define DEF_lum_tsl2561_H

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

#define LUM_TSL2561_VISIBLE 2                   /**< channel 0 - channel 1  */
#define LUM_TSL2561_INFRARED 1                  /**< channel 1              */
#define LUM_TSL2561_FULLSPECTRUM 0              /**< channel 0              */

/* I2C address options*/
#define LUM_TSL2561_ADDR_LOW          (0x29)
#define LUM_TSL2561_ADDR_FLOAT        (0x39)    /**< Default address (pin left floating)*/
#define LUM_TSL2561_ADDR_HIGH         (0x49)

/* Lux calculations differ slightly for CS package*/
//#define TSL2561_PACKAGE_CS
#define LUM_TSL2561_PACKAGE_T_FN_CL

#define LUM_TSL2561_COMMAND_BIT       (0x80)    /**<  Must be 1                                       */
#define LUM_TSL2561_CLEAR_BIT         (0x40)    /**<  Clears any pending interrupt (write 1 to clear) */
#define LUM_TSL2561_WORD_BIT          (0x20)    /**<  1 = read/write word (rather than byte)          */
#define LUM_TSL2561_BLOCK_BIT         (0x10)    /**<  1 = using block read/write                      */

#define LUM_TSL2561_CONTROL_POWERON   (0x03)    /**<                                                  */
#define LUM_TSL2561_CONTROL_POWEROFF  (0x00)    /**<                                                  */

#define LUM_TSL2561_LUX_LUXSCALE      (14)      /**<  Scale by 2^14                                   */
#define LUM_TSL2561_LUX_RATIOSCALE    (9)       /**<  Scale ratio by 2^9                              */
#define LUM_TSL2561_LUX_CHSCALE       (10)      /**<  Scale channel values by 2^10                    */
#define LUM_TSL2561_LUX_CHSCALE_TINT0 (0x7517)  /**<  322/11 * 2^TSL2561_LUX_CHSCALE                  */
#define LUM_TSL2561_LUX_CHSCALE_TINT1 (0x0FE7)  /**<  322/81 * 2^TSL2561_LUX_CHSCALE                  */

/* T, FN and CL package values*/
#define LUM_TSL2561_LUX_K1T           (0x0040)  /**<  0.125 * 2^RATIO_SCALE                           */
#define LUM_TSL2561_LUX_B1T           (0x01f2)  /**<  0.0304 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M1T           (0x01be)  /**<  0.0272 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K2T           (0x0080)  /**<  0.250 * 2^RATIO_SCALE                           */
#define LUM_TSL2561_LUX_B2T           (0x0214)  /**<  0.0325 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M2T           (0x02d1)  /**<  0.0440 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K3T           (0x00c0)  /**<  0.375 * 2^RATIO_SCALE                           */
#define LUM_TSL2561_LUX_B3T           (0x023f)  /**<  0.0351 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M3T           (0x037b)  /**<  0.0544 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K4T           (0x0100)  /**<  0.50 * 2^RATIO_SCALE                            */
#define LUM_TSL2561_LUX_B4T           (0x0270)  /**<  0.0381 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M4T           (0x03fe)  /**<  0.0624 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K5T           (0x0138)  /**<  0.61 * 2^RATIO_SCALE                            */
#define LUM_TSL2561_LUX_B5T           (0x016f)  /**<  0.0224 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M5T           (0x01fc)  /**<  0.0310 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K6T           (0x019a)  /**<  0.80 * 2^RATIO_SCALE                            */
#define LUM_TSL2561_LUX_B6T           (0x00d2)  /**<  0.0128 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M6T           (0x00fb)  /**<  0.0153 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K7T           (0x029a)  /**<  1.3 * 2^RATIO_SCALE                             */
#define LUM_TSL2561_LUX_B7T           (0x0018)  /**<  0.00146 * 2^LUX_SCALE                           */
#define LUM_TSL2561_LUX_M7T           (0x0012)  /**<  0.00112 * 2^LUX_SCALE                           */
#define LUM_TSL2561_LUX_K8T           (0x029a)  /**<  1.3 * 2^RATIO_SCALE                             */
#define LUM_TSL2561_LUX_B8T           (0x0000)  /**<  0.000 * 2^LUX_SCALE                             */
#define LUM_TSL2561_LUX_M8T           (0x0000)  /**<  0.000 * 2^LUX_SCALE                             */

/*CS package values*/
#define LUM_TSL2561_LUX_K1C           (0x0043)  /**<  0.130 * 2^RATIO_SCALE                           */
#define LUM_TSL2561_LUX_B1C           (0x0204)  /**<  0.0315 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M1C           (0x01ad)  /**<  0.0262 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K2C           (0x0085)  /**<  0.260 * 2^RATIO_SCALE                           */
#define LUM_TSL2561_LUX_B2C           (0x0228)  /**<  0.0337 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M2C           (0x02c1)  /**<  0.0430 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K3C           (0x00c8)  /**<  0.390 * 2^RATIO_SCALE                           */
#define LUM_TSL2561_LUX_B3C           (0x0253)  /**<  0.0363 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M3C           (0x0363)  /**<  0.0529 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K4C           (0x010a)  /**<  0.520 * 2^RATIO_SCALE                           */
#define LUM_TSL2561_LUX_B4C           (0x0282)  /**<  0.0392 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M4C           (0x03df)  /**<  0.0605 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K5C           (0x014d)  /**<  0.65 * 2^RATIO_SCALE                            */
#define LUM_TSL2561_LUX_B5C           (0x0177)  /**<  0.0229 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M5C           (0x01dd)  /**<  0.0291 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K6C           (0x019a)  /**<  0.80 * 2^RATIO_SCALE                            */
#define LUM_TSL2561_LUX_B6C           (0x0101)  /**<  0.0157 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_M6C           (0x0127)  /**<  0.0180 * 2^LUX_SCALE                            */
#define LUM_TSL2561_LUX_K7C           (0x029a)  /**<  1.3 * 2^RATIO_SCALE                             */
#define LUM_TSL2561_LUX_B7C           (0x0037)  /**<  0.00338 * 2^LUX_SCALE                           */
#define LUM_TSL2561_LUX_M7C           (0x002b)  /**<  0.00260 * 2^LUX_SCALE                           */
#define LUM_TSL2561_LUX_K8C           (0x029a)  /**<  1.3 * 2^RATIO_SCALE                             */
#define LUM_TSL2561_LUX_B8C           (0x0000)  /**<  0.000 * 2^LUX_SCALE                             */
#define LUM_TSL2561_LUX_M8C           (0x0000)  /**<  0.000 * 2^LUX_SCALE                             */

/*Auto-gain thresholds*/
#define LUM_TSL2561_AGC_THI_13MS      (4850)    /**<  Max value at Ti 13ms = 5047                     */
#define LUM_TSL2561_AGC_TLO_13MS      (100)
#define LUM_TSL2561_AGC_THI_101MS     (36000)   /**<  Max value at Ti 101ms = 37177                   */
#define LUM_TSL2561_AGC_TLO_101MS     (200)
#define LUM_TSL2561_AGC_THI_402MS     (63000)   /**<  Max value at Ti 402ms = 65535                   */
#define LUM_TSL2561_AGC_TLO_402MS     (500)

/* Clipping thresholds*/
#define LUM_TSL2561_CLIPPING_13MS     (4900)
#define LUM_TSL2561_CLIPPING_101MS    (37000)
#define LUM_TSL2561_CLIPPING_402MS    (65000)

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */


/**
 * enum for lum_tsl2561 registeraddresses.
 */
typedef enum
{
  LUM_TSL2561_REGISTER_CONTROL          = 0x00,      /**< LUM_TSL2561_REGISTER_CONTROL            */
  LUM_TSL2561_REGISTER_TIMING           = 0x01,      /**< LUM_TSL2561_REGISTER_TIMING             */
  LUM_TSL2561_REGISTER_THRESHHOLDL_LOW  = 0x02,      /**< LUM_TSL2561_REGISTER_THRESHHOLDL_LOW    */
  LUM_TSL2561_REGISTER_THRESHHOLDL_HIGH = 0x03,      /**< LUM_TSL2561_REGISTER_THRESHHOLDL_HIGH   */
  LUM_TSL2561_REGISTER_THRESHHOLDH_LOW  = 0x04,      /**< LUM_TSL2561_REGISTER_THRESHHOLDH_LOW    */
  LUM_TSL2561_REGISTER_THRESHHOLDH_HIGH = 0x05,      /**< LUM_TSL2561_REGISTER_THRESHHOLDH_HIGH   */
  LUM_TSL2561_REGISTER_INTERRUPT        = 0x06,      /**< LUM_TSL2561_REGISTER_INTERRUPT          */
  LUM_TSL2561_REGISTER_CRC              = 0x08,      /**< LUM_TSL2561_REGISTER_CRC                */
  LUM_TSL2561_REGISTER_ID               = 0x0A,      /**< LUM_TSL2561_REGISTER_ID                 */
  LUM_TSL2561_REGISTER_CHAN0_LOW        = 0x0C,      /**< LUM_TSL2561_REGISTER_CHAN0_LOW          */
  LUM_TSL2561_REGISTER_CHAN0_HIGH       = 0x0D,      /**< LUM_TSL2561_REGISTER_CHAN0_HIGH         */
  LUM_TSL2561_REGISTER_CHAN1_LOW        = 0x0E,      /**< LUM_TSL2561_REGISTER_CHAN1_LOW          */
  LUM_TSL2561_REGISTER_CHAN1_HIGH       = 0x0F       /**< LUM_TSL2561_REGISTER_CHAN1_HIGH         */
}lum_tsl2561_reg_t;

/**
 * Enum for lum_tsl2561 integrationtime.
 */
typedef enum
{
  LUM_TSL2561_INTEGRATIONTIME_13MS      = 0x00,      /**< 13.7ms */
  LUM_TSL2561_INTEGRATIONTIME_101MS     = 0x01,      /**< 101ms  */
  LUM_TSL2561_INTEGRATIONTIME_402MS     = 0x02       /**< 402ms */
} lum_tsl2561_integrationtime_t;

/**
 * Enum for lum_tsl2561 gain.
 */
typedef enum
{
  LUM_TSL2561_GAIN_1X                   = 0x00,      /**< no gain */
  LUM_TSL2561_GAIN_16X                  = 0x10,      /**< 16x gain */
} lum_tsl2561_gain_t;


/**
 * lum_tsl2561 configuration struct
 */
typedef struct lum_tsl2561_config_t
{
	uint8_t id;													/**< id for this device */
	i2c_t *p_i2c;												/**< i2c bus to which this device is connected */
	uint8_t address;											/**< 7-bit i2c address (MSB is not used) */
	lum_tsl2561_integrationtime_t integration_time;				/**< integration time */
	lum_tsl2561_gain_t gain;									/**< gain */
}lum_tsl2561_config_t;


/**
 * lum_tsl2561 Struct
 */
typedef struct lum_tsl2561_t
{
	uint8_t id;													/**< id for this device */
	i2c_t *p_i2c;												/**< i2c bus to which this device is connected */
	uint8_t address;											/**< 7-bit i2c address (MSB is not used) */
	lum_tsl2561_integrationtime_t integration_time;				/**< integration time */
	lum_tsl2561_gain_t gain;									/**< gain */

	uint16_t fullspectrum;										/**< last result for fullspectrum */
	uint16_t infrared;											/**< last result for infrared */
	uint32_t lux;												/**< last calculated LUX result */
}lum_tsl2561_t;


/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the tsl2561 device.
 *
 * @note we will turn it on forever, and let it run forever... it consumes less than 0.5mA --> for my application this is not a big deal.
 * @param p_tsl2561 tsl2561 device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_Init(lum_tsl2561_t *p_tsl2561, lum_tsl2561_config_t *p_config);



/**
 * Set the integrationtime.
 *
 * @note LUM_TSL2561_INTEGRATIONTIME_402MS is slower but more precise, LUM_TSL2561_INTEGRATIONTIME_13MS is very fast but not so precise
 * @param p_tsl2561 tsl2561 device
 * @param integrationtime LUM_TSL2561_INTEGRATIONTIME_402MS or LUM_TSL2561_INTEGRATIONTIME_101MS or LUM_TSL2561_INTEGRATIONTIME_13MS
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_SetIntegrationTime(lum_tsl2561_t *p_tsl2561, lum_tsl2561_integrationtime_t integrationtime);


/**
 * Set the gain.
 *
 * @note Use 16X gain to get more precision in dark ambients, or enable auto gain with LUM_TSL2561_SenseLight()
 * @param p_tsl2561
 * @param gain LUM_TSL2561_GAIN_1X or LUM_TSL2561_GAIN_16X
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_SetGain(lum_tsl2561_t *p_tsl2561, lum_tsl2561_gain_t gain);



/**
 * Wake up the TSL2561 by setting the control bit.
 *
 * @param p_tsl2561 tsl2561 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_On(lum_tsl2561_t *p_tsl2561);



/**
 * Turn the TSL2561 in power saving mode by clearing the control bit.
 *
 * @param p_tsl2561 tsl2561 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_Off(lum_tsl2561_t *p_tsl2561);



/**
 * Sense the ambient light.
 *
 * This function will get the latest raw fullspectrum and infrared value over i2c, and calculate the LUX from it.
 * It also provides the posibility to try the autogain.
 *
 * @note the result will be saved in the device struct, result pointers added for convenience, they can be made NULL if not needed.
 * @param p_tsl2561 tsl2561 device
 * @param p_fullspectrum quantity og light at full spectrum (including infrared) (can be NULL if not needed)
 * @param p_infrared quantity of infrared light (can be NULL if not needed)
 * @param p_lux lux result (can be NULL if not needed)
 * @param autogain false: single sensor reading is done, with gain and integration time previously selected. true: automatic gain adjustment algorithm is used.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_SenseLight(lum_tsl2561_t *p_tsl2561, uint16_t *p_fullspectrum, uint16_t *p_infrared, uint32_t *p_lux, bool autogain);


#endif
/* End of file lum_tsl2561.h */
