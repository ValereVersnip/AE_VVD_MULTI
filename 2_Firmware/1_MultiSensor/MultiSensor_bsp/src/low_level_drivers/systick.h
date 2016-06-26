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
 * @file systick.h
 * @author TimB
 * @date 16-mei-2016
 * @brief	Header file for systick.c
 *
 */

#ifndef DEF_systick_H
#define DEF_systick_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "chip.h"
#include "../library/utilities.h"
#include "status.h"

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
 * Struct for grouping of all systick configuration parameters.
 */
typedef struct systick_config_t
{
	uint32_t rate;			/**< rate Amount of ticks per second (eg: rate = 1000 --> 1 tick every ms) */
	handler *p_handler;		/**< pointer to high level interrupt function (which will be called by the systick interrupt function, can be NULL if not used */
}systick_config_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the systic timer.
 *
 * @param rate Amount of ticks per second (eg: rate = 1000 --> 1 tick every ms)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t SYSTICK_Init(systick_config_t *p_sysTickConfig);


/**
 * Delay the processor for the specified ms.
 *
 * If the systemtick timer is configured for 1Tick/ ms (eg: rate = 1000).
 * @param ms amount of ms to delay
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t SYSTICK_DelayMs(uint32_t msec);


/**
 * Get the current Ticks value.
 * @param p_ticks pointer where result will be saved
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t SYSTICK_GetTicks(uint32_t *p_ticks);

/**
 * Increment the Ticks.
 *
 * This can be usefull, if we have to suspend all interrupts for a while.
 * @param ticks amount of ticks to increment Ticks with
 * @return status_ok
 */
status_t SYSTICK_IncTicks(uint32_t ticks);

#endif
/* End of file systick.h */
