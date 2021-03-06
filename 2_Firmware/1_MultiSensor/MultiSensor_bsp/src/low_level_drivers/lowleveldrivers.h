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
 * @file lowleveldrivers.h
 * @author TimB
 * @date 16-mei-2016
 * @brief	Header file for lowleveldrivers.c
 *
 *	This header makes all Low level device structs external, so they can be accessed by higherlevel routines.
 *	This header also makes initilization, runtimefunctions public.
 */

#ifndef DEF_lowleveldrivers_H
#define DEF_lowleveldrivers_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "boardconfig.h"
#include "pinmux.h"
#include "systick.h"
#include "mrt.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "pwm.h"
#include "adc.h"
#include "wdt.h"


/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */

/*
 * ***********************************************************************************************************************************************
 * external device declaration
 * ***********************************************************************************************************************************************
 */

/* i2c devices */
extern i2c_t I2C0;					/**< I2C 0 Device */

/* pwm SCTs */
extern pwm_t Pwm;					/**< Pwm device */

/* adc device */
extern adc_t Adc;					/**< Adc device */

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

/**
 * Initialize all lowlevel drivers.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LOWLEVELDRIVERS_Init();


/**
 * lowleveldrivers Run0 Function.
 *
 * This function should be called periodically by higher level routines.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LOWLEVELDRIVERS_Run0();


#endif
/* End of file lowleveldrivers.h */
