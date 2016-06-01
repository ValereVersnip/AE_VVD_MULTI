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
 * @file highleveldriversconfig.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Configuration parameters for all highleveldriver structs.
 *
 */

#ifndef DEF_highleveldriversconfig_H
#define DEF_highleveldriversconfig_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */

/*
 * RGBLED startup values
 * ***********************************************************
 */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_ID							0					/**< RGBLED D2 Id for this rgbled */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_PWM						&Pwm				/**< RGBLED D2 Pointer to pwm device which operates this rgbled */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_REDCHANNEL					1					/**< RGBLED D2 Pwm channel for RED led */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GREENCHANNEL				2					/**< RGBLED D2 Pwm channel for GREEN led */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_BLUECHANNEL				3					/**< RGBLED D2 Pwm channel for BLUE led */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_LEDOUT						rgbled_off			/**< RGBLED D2 startup ledout value */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_BLINKONTIME				200					/**< RGBLED D2 blinking on time (in ms) */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_BLINKOFFTIME				1000				/**< RGBLED D2 blinking off time (in ms) */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GLOWDIMMIN					0					/**< RGBLED D2 minimum glow pwm dim value (in percent 0 - 100) */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GLOWDIMMAX					10					/**< RGBLED D2 maximum glow pwm dim value (in percent 0 - 100)*/
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GLOWINTERVAL				40					/**< RGBLED D2 interval between glowsteps (in ms)*/
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GLOWSTEP					1					/**< RGBLED D2 glowstep (in pwm value) */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_COLOR						COLOR_RGB_ROSYBROWN	/**< RGBLED D2 startup color for led */
#define HIGHLEVELDRIVERSCONFIG_RGBLED_D2_DIM						10					/**< RGBLED D2 global dimming value (for blinking and just on) (in percent 0 - 100) */



/*
 * ADDRESS startup values
 * ***********************************************************
 */
#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT							0					/**< All adress pins are on port 0 */

#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_0						14					/**< Gpio Pin for address bit 0 */
#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_1						23                  /**< Gpio Pin for address bit 1 */
#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_2						22                  /**< Gpio Pin for address bit 2 */
#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_3						21                  /**< Gpio Pin for address bit 3 */
#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_4						20                  /**< Gpio Pin for address bit 4 */
#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_5						19                  /**< Gpio Pin for address bit 5 */
#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_6						18                  /**< Gpio Pin for address bit 6 */
#define HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_7						17                  /**< Gpio Pin for address bit 7 */


/*
 * HUMIDITY DHT22 startup values
 * ***********************************************************
 */
#define HIGHLEVELDRIVERSCONFIG_HUMIDITYDHT22_U5_ID					0					/**< id */
#define HIGHLEVELDRIVERSCONFIG_HUMIDITYDHT22_U5_GPIOPORT			0					/**< gpioport for data line */
#define HIGHLEVELDRIVERSCONFIG_HUMIDITYDHT22_U5_GPIOPIN				13					/**< gpiopin for data line */
#define HIGHLEVELDRIVERSCONFIG_HUMIDITYDHT22_U5_MRT					mrt_2				/**< multirate channel which will be setup for 5uS interrupts, at which we sample incoming data */



/*
 * PRESSURE BMP280 startup values
 * ***********************************************************
 */
#define HIGHLEVELDRIVERSCONFIG_PRESSUREBMP280_M5_ID					0					/**< id */
#define HIGHLEVELDRIVERSCONFIG_PRESSUREBMP280_M5_I2C				&I2C0				/**< i2c bus to which this device is connected */
#define HIGHLEVELDRIVERSCONFIG_PRESSUREBMP280_M5_ADDRESS			0x76				/**< 7-bit i2c address (MSB is not used) */




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



#endif
/* End of file highleveldriversconfig.h */