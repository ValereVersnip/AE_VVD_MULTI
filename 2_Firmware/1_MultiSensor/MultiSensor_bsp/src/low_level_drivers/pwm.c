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
 * @file pwm.c
 * @author TimB
 * @date 16-mei-2016
 * @brief	Low level routines for PWM access.
 *
 * PWM initialization and low level routines.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "pwm.h"

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


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the PWM.
 *
 * This function will initialize the PWM.
 * It will set the global frequency.
 * And it will enable any enabled pwm channel
 *
 * Note: In higher level functions we will use pwm channel 0 - 5, but in fact these are routed to logic channels 1 - 6.
 * This number conversion has been taken care of in all pwm functions.
 * We need to do this, because pwm logic channel 0 cannot be used. (See datasheet)
 * We fix this mapping in this init function, because we do not need to remap afterwards.
 * @param p_pwm 	p_pwm device
 * @param p_pwmConfig	configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PWM_Init(pwm_t* p_pwm, pwm_config_t *p_pwmConfig)
{
	status_t status = status_ok;
	uint8_t i;
	/* populate the pwm struct */
	p_pwm->frequency 	   = p_pwmConfig->frequency;
	for(i = 0; i < PWM_AMOUNTOFCHANNELS; i++)
	{
		p_pwm->dutycycle[i]		= p_pwmConfig->dutycycle[i];
		p_pwm->usechannel[i]	= p_pwmConfig->usechannel[i];
	}

	/* Initialize the SCT as PWM and set frequency */
	Chip_SCTPWM_Init(PWM_SCT);
	Chip_SCTPWM_SetRate(PWM_SCT, p_pwm->frequency);

	/* then enable the outputchannel, we here fix pins to logic channel + 1 (because channel 0 should not be used) */
	for(i = 0; i < PWM_AMOUNTOFCHANNELS; i++)
	{
		if(p_pwm->usechannel[i])
		{
			Chip_SCTPWM_SetOutPin(PWM_SCT, ctout1 + i, ctout0 + i);
		}
	}

	/* then set the dutycycles */
	for(i = 0; i < PWM_AMOUNTOFCHANNELS; i++)
	{
		if(status == status_ok)
		{
			if(p_pwm->usechannel[i])
			{
				status = PWM_setdutycycle(p_pwm, i, p_pwm->dutycycle[i]);
			}
		}
	}

	if(status == status_ok)
	{
		/* start the pwm */
		Chip_SCTPWM_Start(PWM_SCT);
	}

	return status;
}



/**
 * Change the dutycycle of the selected channel.
 *
 * Note: we do channel conversion (+1) for correct logical channel mapping
 *
 * @param p_pwm 		p_pwm device
 * @param channel		channel (0 - 5)
 * @param dutycycle		wanted dutycycle (in percent)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PWM_setdutycycle(pwm_t* p_pwm, pwm_channel_t channel, uint8_t dutycycle)
{
	status_t status = status_ok;
	uint32_t ticks = 0;


	if(dutycycle <= 100)
	{
		ticks = Chip_SCTPWM_GetTicksPerCycle(PWM_SCT);
		ticks = ticks / 100;
		ticks = ticks * dutycycle;
		Chip_SCTPWM_SetDutyCycle(PWM_SCT, channel + 1, ticks); /* we add + 1 because the logic channel is 1 more (we cannot use logic channel 0) */
	}
	else
	{
		status = pwm_dutycycle;
	}
	return status;
}





/* End of file pwm.c */
