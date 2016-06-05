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
 * @file microphone.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for microphone.c
 *
 */

#ifndef DEF_microphone_H
#define DEF_microphone_H

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
#define MICROPHONE_SAMPLEBUFFER_SIZE			30				/**< size of the samplebuffer */

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */

/**
 * Struct microphone configuration data.
 */
typedef struct microphone_config_t
{
	uint8_t id;													/**< Id for this device */
	adc_t *p_adc;												/**< adc device */
	uint8_t adcchannel;											/**< adc channel */
	uint32_t adcperiod;											/**< adc sample period (in ms) best results are found with 100ms */
	uint16_t treshold;											/**< treshold (in mV) */
	uint16_t tresholdtime;										/**< the time (in ms) the sound has to be above treshold value, for trigger (has to be at least the adcfreq) */
	uint16_t maxtimebetweenclaps;								/**< the maximum time (in ms) between two claps (must be at least 2*adcperiod) */
	uint16_t claptresh;											/**< treshold clap rising/falling detection */
	uint8_t clapamount;											/**< the amount of claps for triggering */

}microphone_config_t;


/**
 * Struct microphone device.
 */
typedef struct microphone_t
{
	uint8_t id;													/**< Id for this device */
	adc_t *p_adc;												/**< adc device */
	uint8_t adcchannel;											/**< adc channel */
	uint32_t adcperiod;											/**< adc sample frequency (in ms) */
	uint16_t treshold;											/**< treshold (in mV) */
	uint16_t tresholdtime;										/**< the time (in ms) the sound has to be above treshold value, for trigger */
	uint16_t maxtimebetweenclaps;								/**< the maximum time (in ms) between two claps */
	uint16_t claptresh;											/**< treshold clap rising/falling detection */
	uint8_t clapamount;											/**< the amount of claps for triggering */

	uint16_t tresholddetectioncount;							/**< keeps track how many times we have "crossed" the timetreshold */
	uint16_t clapdetectioncount;								/**< counter for keeping track of clap detection */

	uint16_t samplebuffer[MICROPHONE_SAMPLEBUFFER_SIZE];		/**< buffer where samples will be stored, so we can determine sounddetecions */
	uint16_t *p_write;											/**< writepointer in samplebuffer */
	uint16_t *p_read_clap;										/**< clap readpointer */
	uint16_t *p_read_tresh;										/**< treshold readpointer */

	int8_t tresh_overrun;										/**< if this becomes more than the buffersize, we have a treshold detection overrun */
	int8_t clap_overrun;										/**< if this becomes more than the buffersize, we have a clap detection overrun */

	uint16_t abovetreshcount;									/**< counter to keep track of how many consecutive samples were above treshold */
	uint8_t clappers;											/**< counter for individual clappers */
	uint16_t claptimeout;										/**< if we detect a clap, we need to detect clapamount more before this time runs out (maxtimebetweenclaps) */
}microphone_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the microphone.
 * @param p_microphone microphone device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MICROPHONE_Init(microphone_t *p_microphone, microphone_config_t *p_config);



/**
 * Run0 function for Microphone.
 *
 * This function will gather new adc samples, if available.
 * Depending on how many, the timetreshold and clap detection algorithms will be run.
 *
 * @note this function should be called periodically by higherlevel routines.
 * @param p_microphone
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MICROPHONE_Run0(microphone_t *p_microphone);


/**
 * Get the last threshold and clap detection counters.
 *
 * @param p_microphone microphone device
 * @param p_tresholddetectioncount pointer to tresholddetection count (can be NULL if not needed)
 * @param p_clapdetectioncount pointer to clapdetection count (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t MICROPHONE_GetResults(microphone_t *p_microphone, uint16_t *p_tresholddetectioncount, uint16_t *p_clapdetectioncount);


#endif
/* End of file microphone.h */
