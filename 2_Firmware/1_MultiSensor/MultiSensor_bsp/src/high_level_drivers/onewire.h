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
 * @file onewire.h
 * @author TimB
 * @date 26-jun.-2016
 * @brief	Header file for onewire.c
 *
 */

#ifndef DEF_onewire_H
#define DEF_onewire_H

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

#ifdef DEBUG
#define ONEWIRE_TIMING_TICK				657					/**< The real delay 1 tick in the inline delay function provides (in ns) */
#else
#define ONEWIRE_TIMING_TICK				216					/**< The real delay 1 tick in the inline delay function provides (in ns) */
#endif

#define ONEWIRE_TIMING_A				6					/**< recommended time for A (in us)*/
#define ONEWIRE_TIMING_B				64					/**< recommended time for B (in us)*/
#define ONEWIRE_TIMING_C				60					/**< recommended time for C (in us)*/
#define ONEWIRE_TIMING_D				10					/**< recommended time for D (in us)*/
#define ONEWIRE_TIMING_E				9					/**< recommended time for E (in us)*/
#define ONEWIRE_TIMING_F				55					/**< recommended time for F (in us)*/
#define ONEWIRE_TIMING_G				0					/**< recommended time for G (in us)*/
#define ONEWIRE_TIMING_H				480					/**< recommended time for H (in us)*/
#define ONEWIRE_TIMING_I				70					/**< recommended time for I (in us)*/
#define ONEWIRE_TIMING_J				410					/**< recommended time for J (in us)*/


/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/**
 * Struct onewire_config_t configuration data.
 */
typedef struct onewire_config_t
{
	uint8_t id;												/**< id */
	LPC_GPIO_T *p_lpc_gpio;									/**< pointer to LPC gpio struct */
	uint8_t gpioport;										/**< gpioport for data line */
	uint8_t gpiopin;										/**< gpiopin for data line */
}onewire_config_t;


/**
 * Onewire device.
 */
typedef struct onewire_t
{
	uint8_t id;												/**< id */
	LPC_GPIO_T *p_lpc_gpio;									/**< pointer to LPC gpio struct */
	uint8_t gpioport;										/**< gpioport for data line */
	uint8_t gpiopin;										/**< gpiopin for data line */
}onewire_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the onewire.
 * @param p_onewire onewire device
 * @param p_config configuration data
 * @return status_ok
 */
status_t ONEWIRE_Init(onewire_t *p_onewire, onewire_config_t *p_config);



/**
 * Generate a 1-wire reset.
 * @param p_onewire onewire device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ONEWIRE_reset(onewire_t *p_onewire);

/**
 * Send a 1-wire write bit. Provide 10us recovery time.
 * @param p_onewire onewire device
 * @param bit true: write '1', false": write '0'
 * @return status_ok;
 */
status_t ONEWIRE_writebit(onewire_t *p_onewire, bool bit);


/**
 * Read a bit from the 1wire bus. Provide 10us recovery time.
 * @param p_onewire onewire device
 * @param p_bit pointer to result
 * @return status_ok
 */
status_t ONEWIRE_readbit(onewire_t *p_onewire, bool *p_bit);


/**
 * Write 1 byte over the 1wire bus.
 * @param p_onewire onewire device
 * @param data data byte (8bits) to write
 * @return status_ok
 */
status_t ONEWIRE_writebyte(onewire_t *p_onewire, uint8_t data);


/**
 * Read 1 byte over the 1 wire bus.
 * @param p_onewire onewire device
 * @param p_data pointer to result
 * @return status_ok
 */
status_t ONEWIRE_readbyte(onewire_t *p_onewire, uint8_t *p_data);


#endif
/* End of file onewire.h */
