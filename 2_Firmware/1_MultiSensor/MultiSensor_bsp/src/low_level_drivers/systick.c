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
 * @file systick.c
 * @author TimB
 * @date 16-mei-2016
 * @brief	Low level routines for systick access.
 *
 * Systick initialization and low level routines.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "systick.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Local Members
 * ***********************************************************************************************************************************************
 */
volatile uint32_t Ticks; 						/**< counts timeTicks */
static handler *p_SysTickIntrHandler;			/**< systick interrupt function */

/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Systemtick interrupt handler.
 *
 * Increments the Ticks member. (at configured rate).
 * Also calls the user specified interrupt function, if specified.
 */
void SysTick_Handler(void)
{
	Ticks++; 		/* increment counter necessary in Delay() */
	if(p_SysTickIntrHandler != NULL)
	{
		(*p_SysTickIntrHandler)();
	}
}

/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */
/**
 * Initialize the systic timer.
 *
 * @param rate Amount of ticks per second (eg: rate = 1000 --> 1 tick every ms)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t SYSTICK_Init(systick_config_t *p_sysTickConfig)
{
	p_SysTickIntrHandler = p_sysTickConfig->p_handler;

	SysTick_Config(SystemCoreClock / p_sysTickConfig->rate);
	return status_ok;

}


/**
 * Delay the processor for the specified ms.
 *
 * If the systemtick timer is configured for 1Tick/ ms (eg: rate = 1000).
 * @param ms amount of ms to delay
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t SYSTICK_DelayMs(uint32_t msec)
{
	volatile uint32_t done = Ticks + msec;
	while (Ticks != done);
	return status_ok;
}


/**
 * Get the current Ticks value.
 * @param p_ticks pointer where result will be saved
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t SYSTICK_GetTicks(uint32_t *p_ticks)
{
	*p_ticks = Ticks;
	return status_ok;
}


/* End of file systick.c */
