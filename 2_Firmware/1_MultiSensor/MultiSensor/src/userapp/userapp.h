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
 * @project MultiSensor
 * @file userapp.h
 * @author TimB
 * @date 9-jun.-2016
 * @brief	Header file for userapp.c
 *
 */

#ifndef DEF_userapp_H
#define DEF_userapp_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "userfunctions/modbusslave.h"


/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */
#define USERAPP_VERSION_MAJOR					10
#define USERAPP_VERSION_MINOR					5
#define USERAPP_VERSION_CHANGE					1




/* MODBUS defines */
#define USERAPP_MODBUS_BAUDRATE					115200						/**< the baudrate for the modbus */
#define USERAPP_MODBUS_TXENABLE_PORT			0							/**< rs485 tx enable pin connected to port 0 */
#define USERAPP_MODBUS_TXENABLE_PIN				27							/**< rs485 tx enable pin connected to pin 27 */


/* DHT22 HUMIDTIY */
#define USERAPP_HUMIDITY_POLLRATE				2500						/**< Pollrate for the dht22 humidity sensor (should be at least 2000!) (in ms) */
#define USERAPP_HUMIDITY_CONSECUTIVE_ERRTH		5							/**< if we get this many consecutive faulty readings from the humidity sensor, we will consider this to be a hardware problem
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 (one of two faulty readings is possible though...) */


/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */


/**
 * enum for the order in which we will update the sensor measurements.
 */
typedef enum
{
	order_humidity,														/**< update humidity */
	order_temperature,														/**< update temperature */
	order_pressure,                                                         /**< update pressure */
	order_pir,                                                              /**< update motion (pir) */
	order_luminosity,                                                       /**< update luminosity */
	order_gas,                                                              /**< update gas */
	order_sound,                                                            /**< update sound */
	order_rgbled,                                                           /**< update rgbled */
	order_buzzer,                                                           /**< update buzzer */

	order_done,																/**< done */

	order_last,																/**< last member, so we know size of this enum */
}userapp_order_t;


/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the userapp.
 */
void USERAPP_Init();


/**
 * Run function for the user app.
 *
 * @note this function should be called periodically by higher level routines.
 */
void USERAPP_Run0();


/**
 * Handle the status.
 *
 * If we don't get status_ok, we will put on the red led, and trap here.
 * So we can attach our debugger and watch the failing callstack.
 * @param stat status
 */
void USERAPP_HandleStatus(status_t stat);

#endif
/* End of file userapp.h */
