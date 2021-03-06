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
 * @file mrt.h
 * @author TimB
 * @date 18-mei-2016
 * @brief	Header file for mrt.c
 *
 */

#ifndef DEF_mrt_H
#define DEF_mrt_H

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
 * Enumeration for mrt timers.
 */
typedef enum
{
	mrt_0,				/**< MRT TIMER 0 */
	mrt_1,				/**< MRT TIMER 1 */
	mrt_2,				/**< MRT TIMER 2 */
	mrt_3,				/**< MRT TIMER 3 */
	mrt_size			/**< amount of timers */
}mrt_name_t;


/**
 * Struct for timer configuration data.
 */
typedef struct mrt_config_t
{
	bool enable[mrt_size];										/**< true: enable the timer, false: disable the timer */
	uint32_t interruptfreq[mrt_size];							/**< interrupt frequency in mHz!!!!*/
	bool enablematchinterrupt[mrt_size];						/**< enable the match interrupt */
	handler *p_handler[mrt_size];								/**< pointer to high level interrupt function (which will be called by the timer interrupt function, can be NULL if not used */
}mrt_config_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the MRT timers.
 *
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MRT_Init(mrt_config_t *p_config);


/**
 * Delay the processor for the specified amount of ticks.
 *
 * The user should know the timer freq.
 * @param ticks amount of ticks to delay
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MRT_Delay(mrt_name_t mrt, uint32_t ticks);


/**
 * Get the current Ticks value.
 * @param p_ticks pointer where result will be saved
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MRT_GetTicks(mrt_name_t mrt, uint32_t *p_ticks);


/**
 * Enable or disable the specified timer.
 * @param mrt channel
 * @param enable true: enable, false: disable
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MRT_EnableTimer(mrt_name_t mrt, bool enable);

#endif
/* End of file mrt.h */
