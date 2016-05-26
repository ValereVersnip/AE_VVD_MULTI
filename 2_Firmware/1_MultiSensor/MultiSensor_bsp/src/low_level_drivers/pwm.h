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
 * @file pwm.h
 * @author TimB
 * @date 16-mei-2016
 * @brief	Header file for pwm.c
 *
 */

#ifndef DEF_pwm_H
#define DEF_pwm_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "chip.h"
#include "status.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */
#define PWM_SCT					LPC_SCT		/**< Pointer to the LPC4357 SCT base address */
#define PWM_MAX_FREQUENCY		10000000 	/**< the maximum frequency of the PWM to guarantee a good resolution of the dutycycle */
#define PWM_AMOUNTOFCHANNELS 	6			/**< there are 6 available pwm channels */


/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */
/**
 * PWM_Channel_t is an enum that controls the id of the SCT channel.
 */
typedef enum
{
	ctout0 = 0,			/**< the pwm on CTOUT0*/
	ctout1,				/**< the pwm on CTOUT1*/
	ctout2,				/**< the pwm on CTOUT2*/
	ctout3,				/**< the pwm on CTOUT3*/
	ctout4,				/**< the pwm on CTOUT4*/
	ctout5,				/**< the pwm on CTOUT5*/
}pwm_channel_t;




/**
 * configuration data low level driver struct for the PWM.
 */
typedef struct pwm_config_t
{
	uint32_t frequency; 						/**<the frequency setting of the pwm multiplexer in Hz */
	bool usechannel[PWM_AMOUNTOFCHANNELS]; 		/**< use the corresponding pwm channel or not */
	uint8_t dutycycle[PWM_AMOUNTOFCHANNELS]; 	/**<Target dutycycle for the channels in percent 0-100 */
}pwm_config_t;

/**
 * this is the low level driver struct for the PWM.
 */
typedef struct pwm_t
{
	uint32_t frequency; 						/**<the frequency setting of the pwm multiplexer in Hz */
	bool usechannel[PWM_AMOUNTOFCHANNELS]; 		/**< use the corresponding pwm channel or not */
	uint8_t dutycycle[PWM_AMOUNTOFCHANNELS]; 	/**<Target dutycycle for the channels in percent 0-100 */
}pwm_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
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
status_t PWM_Init(pwm_t* p_pwm, pwm_config_t *p_pwmConfig);


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
status_t PWM_setdutycycle(pwm_t* p_pwm, pwm_channel_t channel, uint8_t dutycycle);


#endif
/* End of file pwm.h */
