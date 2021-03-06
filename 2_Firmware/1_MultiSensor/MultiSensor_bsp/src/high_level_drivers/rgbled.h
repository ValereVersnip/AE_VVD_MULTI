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
 * @file rgbled.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for rgbled.c
 *
 */

#ifndef DEF_rgbled_H
#define DEF_rgbled_H

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
 * enum for rgbled ledout
 */
typedef enum
{
	rgbled_off,										/**< rgbled off */
	rgbled_on,										/**< rgbled on */
	rgbled_glow,									/**< rgbled glowing */
	rgbled_blink,									/**< rgbled blinking */
}rgbled_ledout_t;

/**
 * enum for rgbled state. needed for timing functions
 */
typedef enum
{
	rgbled_state_idle,								/**< rgbled state idle */
	rgbled_state_blinkon,							/**< rgbled state blinkon */
	rgbled_state_blinkoff,							/**< rgbled state blinkoff */
	rgbled_state_glowup,							/**< we are glowing up */
	rgbled_state_glowdown,							/**< we are glowing down */
}rgbled_state_t;


/**
 * rgb led configuration struct.
 */
typedef struct rgbled_config_t
{
	uint8_t id;										/**< Id for this rgbled */
	pwm_t *p_pwm;									/**< Pointer to pwm device which operates this rgbled */
	uint8_t red_channel;							/**< Pwm channel for RED led */
	uint8_t green_channel;							/**< Pwm channel for GREEN led */
	uint8_t blue_channel;							/**< Pwm channel for BLUE led */
	rgbled_ledout_t ledout;							/**< startup ledout value */
	uint16_t blinkontime;							/**< blinking on time (in ms) */
	uint16_t blinkofftime;							/**< blinking off time (in ms) */
	uint8_t glow_dim_min;							/**< minimum glow pwm dim value (in percent 0 - 100) */
	uint8_t glow_dim_max;							/**< maximum glow pwm dim value (in percent 0 - 100)*/
	uint16_t glowinterval;							/**< interval between glowsteps (in ms)*/
	uint8_t glowstep;								/**< glowstep (in pwm value) */
	uint32_t color;									/**< startup color for led */
	uint8_t dim;									/**< global dimming value (for blinking and just on) (in percent 0 - 100) */
}rgbled_config_t;


/**
 * rgbled device.
 */
typedef struct rgbled_t
{
	uint8_t id;										/**< Id for this rgbled */
	pwm_t *p_pwm;									/**< Pointer to pwm device which operates this rgbled */
	uint8_t red_channel;							/**< Pwm channel for RED led */
	uint8_t green_channel;							/**< Pwm channel for GREEN led */
	uint8_t blue_channel;							/**< Pwm channel for BLUE led */
	rgbled_ledout_t ledout;							/**< startup ledout value */
	uint16_t blinkontime;							/**< blinking on time (in ms) */
	uint16_t blinkofftime;							/**< blinking off time (in ms) */
	uint8_t glow_dim_min;							/**< minimum glow pwm dim value (in percent 0 - 100) */
	uint8_t glow_dim_max;							/**< maximum glow pwm dim value (in percent 0 - 100)*/
	uint16_t glowinterval;							/**< interval between glowsteps (in ms)*/
	uint8_t glowstep;								/**< glowstep (in pwm value) */
	uint32_t color;									/**< startup color for led */
	uint8_t dim;									/**< global dimming value (for blinking and just on) (in percent 0 - 100) */
	uint32_t timestamp;								/**< timestamp for various timing functions */
	uint8_t currentdim;								/**< current dimming value (for glowing functions) */
	rgbled_state_t state;							/**< led state for timing functions */
}rgbled_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * RGBLED Init.
 *
 * @param p_rgbled rgbled device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_Init(rgbled_t *p_rgbled, rgbled_config_t *p_config);


/**
 * Run0 function for RGBLED.
 *
 * @note This function should be called periodically from higher level routines.
 * @param p_rgbled rgbled device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_Run0(rgbled_t *p_rgbled);


/**
 * Update the rgbled color and dim value.
 *
 * This function sets the rgbled color by setting the pwm.
 * @param p_rgbled rgbled device
 * @param color wanted color (24Bits RGB)
 * @param dimming dimming value 0 - 100
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_Update(rgbled_t *p_rgbled, uint32_t color, uint8_t dimming);


/**
 * Turn off the led.
 *
 * This will in fact set RGB value to 0, and dimming also to 0.
 * @param p_rgbled rgbled device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_LedOff(rgbled_t *p_rgbled);


/**
 * Turn on the led.
 *
 * The color and dim value (if not -1) will also be updated.
 * If these values are -1, the current color and dim value will be used
 * @param p_rgbled rgbled device
 * @param color -1: don't update, else the wanted color
 * @param dim -1: don't update, else wanted dim value (0-100)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_LedOn(rgbled_t *p_rgbled, int32_t color, int16_t dim);


/**
 * Set the led to blinking.
 *
 * The color,dim, blinkon, blinkoff value (if not -1) will also be updated.
 * If these values are -1, the current color and dim and blink timings value will be used
 * @param p_rgbled rgbled device
 * @param color -1: don't update, else the wanted color
 * @param dim -1: don't update, else wanted dim value (0-100)
 * @param blinkon -1: don't update, else wanted blinkon time (in ms)
 * @param blinkoff -1: don't update, else wanted blinkoff time (in ms)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_LedBlink(rgbled_t *p_rgbled, int32_t color, int16_t dim, int32_t blinkon, int32_t blinkoff);

/**
 * Set the led to glowing.
 *
 * The color,dim, glow_dim_min, glow_dim_max, glowtime value (if not -1) will also be updated.
 * If these values are -1, the current values will be used
 * @param p_rgbled rgbled device
 * @param color -1: don't update, else the wanted color
 * @param glow_dim_min -1: don't update, else the wanted dim min value
 * @param glow_dim_max -1: don't update, else the wanted dim max value
 * @param glowtime -1: don't update, else the wanted glowtime
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_LedGlow(rgbled_t *p_rgbled, int32_t color, int16_t glow_dim_min, int16_t glow_dim_max, int32_t glowinterval, int16_t glowstep);

#endif
/* End of file rgbled.h */
