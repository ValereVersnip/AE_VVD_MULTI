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
 * @file adc.h
 * @author TimB
 * @date 18-mei-2016
 * @brief	Header file for adc.c
 *
 */

#ifndef DEF_adc_H
#define DEF_adc_H

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

#define ADC_CHANNELAMOUNT			12					/**< The LPC824 has 12 ADC inputs */
#define ADC_BUFFERSIZE				32					/**< Adc databuffer size MUST be power of 2*/

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/**
 * Struct for adc configuration data.
 */
typedef struct adc_config_t
{
	bool usechannel[ADC_CHANNELAMOUNT];					/**< true: channel enabled, false: channel disabled */
}adc_config_t;

/**
 * Adc device.
 */
typedef struct adc_t
{
	bool usechannel[ADC_CHANNELAMOUNT];					/**< true: channel enabled, false: channel disabled */
	RINGBUFF_T *p_ringbuffer[ADC_CHANNELAMOUNT];		/**< pointer to adcchannel ringbuffer */
	bool overrun[ADC_CHANNELAMOUNT];					/**< true: channel overrun, false: no channel overrun */
	bool seqA_running;									/**< true: sequence A is running, false: no conversion is running */
}adc_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the ADC.
 *
 * For enabled channels:
 * 	ringbuffers will be initialized
 * 	they will all be put in sequencer A (for our purpose, 1 sequencer is enough)
 * 	We don't use tresholds, we only give interrupts when sequencer A is finished.
 * @param p_adc adc device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADC_Init(adc_t *p_adc, adc_config_t *p_config);



/**
 * Run function for ADC.
 *
 * Check if SeqA is finished.
 * Ifso: check the overrun status, and set in struct.
 * Push new data to relevant ringbuffer.
 *
 * @note This function should be called periodically by higher level routines.
 * @param p_adc adc device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADC_Run0(adc_t *p_adc);


/**
 * Start Sequence A ADC conversion.
 *
 * This function is best called from a timer interrupt routine.
 * It will start a seq A adc conversion.
 * But it will first check if no previous conversion is running.
 * @param p_adc adc device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADC_StartSequenceA(adc_t *p_adc);

#endif
/* End of file adc.h */
