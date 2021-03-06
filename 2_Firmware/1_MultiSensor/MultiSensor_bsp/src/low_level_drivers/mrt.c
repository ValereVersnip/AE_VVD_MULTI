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
 * @file mrt.c
 * @author TimB
 * @date 18-mei-2016
 * @brief	Low level routines for mrt access.
 *
 * Mrt initialization and low level routines.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "mrt.h"

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
static handler *p_MrtIntrHandler[4];		/**< pointer to handlers, can be NULL if not needed */
volatile uint32_t MrtTicks[mrt_size]; 		/**< counts timeTicks */

/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */
/**
 * Get the mrt interval setting for wanted frequency.
 * @param freq	wanted frequency (in mHz!!!!!)
 * @return interval setting.
 */
uint32_t mrt_getintervalfromfreq(uint32_t freq);

/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * MRT interrupt handler
 */
void MRT_IRQHandler(void)
{
	uint32_t int_pend;

	/* Get interrupt pending status for all timers */
	int_pend = Chip_MRT_GetIntPending();
	Chip_MRT_ClearIntPending(int_pend);
	/* mrt 0 */
	if(int_pend & 0x01)
	{
		if(p_MrtIntrHandler[0] != NULL)
		{
			(*p_MrtIntrHandler[0])();
		}
		MrtTicks[0]++;
	}
	/* mrt 1 */
	if(int_pend & 0x02)
	{
		if(p_MrtIntrHandler[1] != NULL)
		{
			(*p_MrtIntrHandler[1])();
		}
		MrtTicks[1]++;
	}
	/* mrt 2 */
	if(int_pend & 0x04)
	{
		if(p_MrtIntrHandler[2] != NULL)
		{
			(*p_MrtIntrHandler[2])();
		}
		MrtTicks[2]++;
	}
	/* mrt 3 */
	if(int_pend & 0x08)
	{
		if(p_MrtIntrHandler[3] != NULL)
		{
			(*p_MrtIntrHandler[3])();
		}
		MrtTicks[3]++;
	}
}

/**
 * Get the mrt interval setting for wanted frequency.
 * @param freq	wanted frequency (in mHz!!!!!)
 * @return interval setting.
 */
uint32_t mrt_getintervalfromfreq(uint32_t freq)
{
	uint64_t interval;
	uint32_t systemrate;

	/* get systemrate */
	systemrate = Chip_Clock_GetSystemClockRate();

	/* if we want a frequency bigger or equal to 1Hz */
	if(freq >= 1000)
	{
		interval = (uint64_t)(systemrate / (freq / 1000));
	}
	/* if the frequency is lower than 1Hz */
	else
	{
		interval = (uint64_t)(1000000000 / freq);
		interval *= systemrate;
		interval /= 1000000;
	}
	return (uint32_t)interval;
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the MRT timers.
 *
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MRT_Init(mrt_config_t *p_config)
{
	LPC_MRT_CH_T *p_Mrt;
	int mrtch;

	uint32_t interval;


	/* MRT Initialization and disable all timers */
	Chip_MRT_Init();
	for (mrtch = 0; mrtch < MRT_CHANNELS_NUM; mrtch++)
	{
		Chip_MRT_SetDisabled(Chip_MRT_GetRegPtr(mrtch));
	}

	/* Enable the interrupt for the MRT */
	NVIC_EnableIRQ(MRT_IRQn);

	/* enable the wanted timers, and set them up */
	for (mrtch = 0; mrtch < MRT_CHANNELS_NUM; mrtch++)
	{
		if(p_config->enable[mrtch])
		{
			/* Set the handler pointer */
			p_MrtIntrHandler[mrtch] = p_config->p_handler[mrtch];

			/* Get pointer to timer selected by ch */
			p_Mrt = Chip_MRT_GetRegPtr(mrtch);

			/* Setup timer with rate based on MRT clock */
			interval = mrt_getintervalfromfreq(p_config->interruptfreq[mrtch]);
			Chip_MRT_SetInterval(p_Mrt, interval | MRT_INTVAL_LOAD);

			/* Timer mode */
			Chip_MRT_SetMode(p_Mrt, MRT_MODE_REPEAT);	/* for now, only repeat mode is supported */

			/* Clear pending interrupt */
			Chip_MRT_IntClear(p_Mrt);
			/* if we want interrupts to be generated immediately, enable timer */
			if(p_config->enablematchinterrupt[mrtch])
			{
				Chip_MRT_SetEnabled(p_Mrt);
			}
		}
	}
	return status_ok;
}


/**
 * Delay the processor for the specified amount of ticks.
 *
 * The user should know the timer freq.
 * @param ticks amount of ticks to delay
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MRT_Delay(mrt_name_t mrt, uint32_t ticks)
{
	volatile uint32_t done = MrtTicks[mrt] + ticks;
	while (MrtTicks[mrt] != done);
	return status_ok;
}


/**
 * Get the current Ticks value.
 * @param p_ticks pointer where result will be saved
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MRT_GetTicks(mrt_name_t mrt, uint32_t *p_ticks)
{
	*p_ticks = MrtTicks[mrt];
	return status_ok;
}


/**
 * Enable or disable the specified timer.
 * @param mrt channel
 * @param enable true: enable, false: disable
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MRT_EnableTimer(mrt_name_t mrt, bool enable)
{
	status_t status = status_ok;
	LPC_MRT_CH_T *p_Mrt;

	/* Get pointer to timer selected by ch */
	p_Mrt = Chip_MRT_GetRegPtr(mrt);

	if(enable)
	{
		/* Clear pending interrupt and enable timer */
		Chip_MRT_IntClear(p_Mrt);
		Chip_MRT_SetEnabled(p_Mrt);
	}
	else
	{
		/* Clear pending interrupt and disable timer */
		Chip_MRT_IntClear(p_Mrt);
		Chip_MRT_SetDisabled(p_Mrt);
	}


	return status;
}


/* End of file mrt.c */
