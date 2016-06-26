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
 * @file status.h
 * @author TimB
 * @date 25-apr.-2016
 * @brief	This is the header file containing all status codes.
 *
 */

#ifndef DEF_status_H
#define DEF_status_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */


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
 * Statuscode return values.
 *
 *  Statuscodes are a 32-bit value with :
 *      - The 16 MSB contains the peripheral code number
 *      - The 16 LSB contains an error code number associated to that peripheral
 */
typedef enum
{
	/*
	 * ***********************************************************************************************************************************************
	 * Low Level Status Codes
	 * ***********************************************************************************************************************************************
	 */

	/* global codes */
	status_ok,									/**< global status ok */
	status_nok,									/**< global status NOT ok */

	/* GPIO related codes */

	/* SYSTICK related codes */
	systick_initfailed,							/**< systick initialization failed */

	/* pwm related codes */
	pwm_dutycycle,								/**< out of bounds pwm dutycycle specified */
	pwm_frequency,								/**< wrong pwm frequency */

	/* CDC related codes */
	cdc_usbinit_failure,						/**< the USB initialization failed */
	cdc_vcominit_failure,						/**< the VCOM initializtion failed */
	cdc_overflow,								/**< an overflow has occured */
	cdc_txringbuffer_overflow,					/**< the tx ringbuffer flows over */
	cdc_transmiterror,							/**< an error occured during transmitting */

	/* USART related codes */
	usart_busy,									/**< usart busy */
	usart_databits,								/**< wrong databits amount */
	usart_stopbits,								/**< wrong stopbits amount */
	usart_txoverflow,							/**< usart tx overflow */
	usart_lengthMisMatch,						/**< length mismatch for send or receive */
	usart_fifoError,							/**< fifo error */
	usart_baudrate,								/**< wrong baud rate */


	/* I2C related codes */
	i2c_wrongbase,								/**< wrong i2c base */
	i2c_sanityerror,							/**< i2c sanity error */
	i2c_handlesetup,							/**< handle setup error */
	i2c_bitrate,								/**< i2c bitrate error */
	i2c_transfer,								/**< transfer error */
	i2c_overflow,								/**< buffer overflow */


	/* ADC related codes */
	adc_channelnotenabled,						/**< the adc channel is not enabled */
	adc_datanotvalid,							/**< althoug sequencer is completed, data is not valid */
	adc_seqArunning,							/**< we request a new seqA conversin, but previous is not yet completed */


	/*
	 * ***********************************************************************************************************************************************
	 * High Level Status Codes
	 * ***********************************************************************************************************************************************
	 */

	/* rgbled related codes */
	rgbled_dimvalue,							/**< specified dimvalue not between 0 and 100 */

	/* humidity_dht22 related codes */
	humidity_dht22_running,						/**< the sensor is already ready and cannot be started now */
	humidity_dht22_toofast,						/**< the timeout between two conversions is not respected */
	humidity_dht22_timeout,						/**< the conversion has timed out */
	humidity_dht22_crc,							/**< wrong crc */
	humidity_dht22_notready,					/**< no conversion results are ready */


	/* pressure bmp280 related codes */
	pressure_bmp280_init,						/**< initialization error */
	pressure_bmp280_readout,					/**< readout error */

	/* lum tsl2561 related codes */
	lum_tsl2561_saturated,						/**< the sensor is saturated */

	/* buzzer related codes */
	buzzer_frequencyerror,						/**< wrong frequency */

	/* microphone related codes */
	microphone_bufferoverflow,					/**< one of the buffers has overflowed, this should never happen! */
	microphone_pop,								/**< there was a pop error */

	/* onewire related codes */
	onewire_presence,							/**< no presence detection after reset pulse */

	/* temp_ds18b20 related codes */
	temp_ds18b20_romcrc,						/**< bad rom crc check */
	temp_ds18b20_ramcrc,						/**< bad ram crc check */
	temp_ds18b20_running,						/**< we try to start a new conversion, but another is already running */
	temp_ds18b20_timeout,						/**< the conversion has timed out */
	temp_ds18b20_state,							/**< unknown state */
	temp_ds18b20_notready,						/**< no conversion result is ready */

}status_t;
/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */



#endif
/* End of file status.h */
