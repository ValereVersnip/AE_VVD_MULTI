 /*             #
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
 * @file usart.h
 * @author TimB
 * @date 25-apr.-2016
 * @brief Header file for usart.c
 *
 */

#ifndef DEF_usart_H
#define DEF_usart_H

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
 * Enumeration for usart.
 */
typedef enum
{
	usart_0,			/**< USART 0 */
	usart_1,			/**< USART 1 */
	usart_2,			/**< USART 2 */
	usart_size			/**< amount of USART controllers */
}usart_name_t;



/**
 * Enumeration for usart parity.
 */
typedef enum
{
	usart_parity_none,				/**< NO parity */
	usart_parity_even,				/**< EVEN parity */
	usart_parity_odd,				/**< ODD parity */
}usart_parity_t;


/**
 * Struct for usart configuration data.
 */
typedef struct usart_config_t
{
	bool enable[usart_size];						/**< enable the uart */
	uint8_t databits[usart_size];								/**< 5,6,7 or 8 databits --> rest is invalid */
	uint32_t baud[usart_size];									/**< baudrate */
	usart_parity_t parity[usart_size];							/**< parity definition */
	uint8_t stopbits[usart_size];								/**< 1 or 2 */
}usart_config_t;


/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */
/**
 * Initialize the USART.
 * @param p_usartConfig config data
 * @return usart_ok if succeeded
 */
status_t USART_Init(usart_config_t *p_usartConfig);


/**
 * DeInitialize the USART.
 * @param p_usartConfig config data
 * @return usart_ok if succeeded
 */
status_t USART_DeInit();


/**
 * Check if the receive ringbuffer has data.
 * @param usart usartname
 * @param p_available pointer to result. False: RX buffer is empty, True: data available
 * @return usart_ok
 */
status_t USART_RxDataAvailable(usart_name_t usart, bool *p_available);


/**
 * Flush the USART rx ringbuffer.
 * @param usart usartname
 * @return usart_ok
 */
status_t USART_RxFlush(usart_name_t usart);


/**
 * Increment the usart rx ringbuffer with skips amount, do nothing with popped data.
 * @param usart usartname
 * @param skips amount of characters to skip
 * @return usart_ok
 */
status_t USART_IncrementRxBufPointer(usart_name_t usart, uint32_t skips);



/**
 * this method will handle the receiving of data through uart
 * @param usart usartname
 * @param p_receiveData a pointer to the buffer where you want the data to be stored
 * @param length the amount of bytes you want to read
 * @return usart_ok if succeeded
 */
status_t USART_Receive(usart_name_t usart, uint8_t* p_receiveData, uint8_t length);


/**
 * Get the amount of received items in the uart buffer.
 * @param p_RxCount pointer to result
 * @return usart_ok
 */
status_t USART_RxCount(usart_name_t usart, uint32_t *p_RxCount);



/**
 * this method will handle sending through uart
 * @param usart usartname
 * @param p_sendData pointer to the data you want to send
 * @param length the maximum amount of data to send
 * @return uart_ok if sending was successful
 */
status_t USART_Send(usart_name_t usart, uint8_t* p_sendData, uint8_t length);


/**
 * this method will handle sending through uart, in a blocking manner.
 * @param usart usartname
 * @param p_sendData pointer to the data you want to send
 * @param length the maximum amount of data to send
 * @return uart_ok if sending was successful
 */
status_t USART_SendBlocking(usart_name_t usart, uint8_t* p_sendData, uint8_t length);


/**
 * Enable or disable all rx interrupts.
 * @param enable true: enable, false disable
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_EnableInterrupt(bool enable);


#endif
/* End of file usart.h */
