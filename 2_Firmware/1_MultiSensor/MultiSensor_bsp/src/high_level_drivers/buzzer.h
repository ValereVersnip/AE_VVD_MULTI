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
 * @file buzzer.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for buzzer.c
 *
 */

#ifndef DEF_buzzer_H
#define DEF_buzzer_H

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

#define BUZZER_MAX_FREQUENCY		15000	/**< Maximum allowed frequency for a buzzer (in Hz) */
#define BUZZER_MIN_FREQUENCY		100		/**< Minimum allowed frequency for a buzzer (in Hz) */
#define BUZZER_DUTYCYCLE			50		/**< Duty cycle for buzzers (in Percent) */


/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/** buzzer statemachine states */
typedef enum
{
	buzzer_state_idle,						/**< buzzer idle */
	buzzer_state_buzz,						/**< buzzer buzzing */
}buzzer_state_t;

/**
 * Struct buzzer configuration data.
 */
typedef struct buzzer_config_t
{
	uint8_t id;								/**< Id for this device */
	uint8_t port;							/**< gpio port which operates this buzzer */
	uint8_t pin;							/**< gpio pin which operates this buzzer */
}buzzer_config_t;


/**
 * Struct buzzer device.
 */
typedef struct buzzer_t
{
	uint8_t id;								/**< Id for this device */
	bool init;								/**< true: initialized, false: not initialized */
	uint8_t port;							/**< gpio port which operates this buzzer */
	uint8_t pin;							/**< gpio pin which operates this buzzer */
	bool state;								/**< true: on, false: off */
	uint32_t timestamp;						/**< timestamp for timed buzzing operation */
	uint32_t pulsewidth;					/**< pulsewidth for timed buzzing operation */
	buzzer_state_t buzzerstate;				/**< statemachine state */
	bool buzzpending;						/**< buzzpending */
}buzzer_t;



/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */


/**
 * Initialize buzzer device.
 * @param p_buzzerDevice
 * @param p_buzzerConfig
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_Init(buzzer_t *p_buzzerDevice, buzzer_config_t *p_buzzerConfig);



/**
 * DeInitialize buzzer device.
 * @param p_buzzerDevice
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_DeInit(buzzer_t *p_buzzerDevice);


/**
 * Start or Stop the buzzer.
 * @param p_buzzerDevice buzzer device
 * @param start true: start buzzer, false: stop buzzer
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_Start(buzzer_t *p_buzzerDevice, bool start);



/**
 * start a timed buzz operation.
 *
 * This will set the buzzpending flag for the statemachine.
 * And set the time (in ms) for the pulsewidth
 * @param p_buzzerDevice buzzer device
 * @param time time to buzz in ms
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_Buzz(buzzer_t *p_buzzerDevice, uint32_t time);



/**
 * Run function for the buzzer device.
 *
 * This function shoulc be called periodically by higher level functions.
 * @param p_buzzerDevice buzzer device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t BUZZER_Run0(buzzer_t *p_buzzerDevice);


#endif
/* End of file buzzer.h */
