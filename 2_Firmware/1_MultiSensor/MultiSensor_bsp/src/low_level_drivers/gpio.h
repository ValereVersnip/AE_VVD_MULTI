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
 * @file gpio.h
 * @author TimB
 * @date 16-mei-2016
 * @brief	Header file for gpio.c
 *
 */

#ifndef DEF_gpio_H
#define DEF_gpio_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "chip.h"
#include "status.h"
#include "../library/utilities.h"


/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */
#define GPIO_PIN_AMOUNT					29		/**< there are 29 gpios on port 0 */

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */
/**
 * Enumeration for gpio direction.
 */
typedef enum
{
	input,			                    /**<  input */
	outputhigh,		                    /**<  output, pin is high @startup */
	outputlow							/**<  output, pin is low @startup */
}gpio_direction_t;


/**
 * Enumeration for gpio interrupt selection.
 */
typedef enum
{
	none,			                    /**< no interrupt */
	levelhigh,							/**< pininterrupt level mode, high */
	levellow,							/**< pininterrupt level mode, low */
	edgehigh,							/**< pininterrupt edge mode, high */
	edgelow,							/**< pininterrupt edge mode, low */
	edgeboth,							/**< pininterrupt on both edges */
}gpio_irqmode_t;



/**
 * Struct for grouping gpio pin parameters
 */
typedef struct gpio_pin_t
{
	uint8_t port;						/**< port */
	uint8_t pin;						/**< pin */
	gpio_direction_t dir;				/**< input or output */
	gpio_irqmode_t irq;						/**< interrupt selection */
	uint8_t irqchannel;					/**< interrupt channel 0 - 7  */
}gpio_pin_t;

/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize all Gpio pins.
 *
 * If a pin is not in this define, it will be configured as input.
 * @param gpioParams array with gpio params
 * @param gpioParamsLen length of the gpioparams array
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Init(const gpio_pin_t gpioParams[], uint8_t gpioParamsLen);


/**
 * DeInitialize the GPIO.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_DeInit();



/**
 * Toggle GPIO pin.
 *
 * @param port GPIO port
 * @param pin GPIO pin
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Toggle_Pin(uint8_t port, uint8_t pin);


/**
 * Set GPIO pin.
 *
 * @param port GPIO port
 * @param pin GPIO pin
 * @param set true: high, false: low
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Set_Pin(uint8_t port, uint8_t pin, bool set);


/**
 * Get GPIO Pin.
 *
 * @param port GPIO port
 * @param pin GPIO pin
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Get_Pin(uint8_t port, uint8_t pin, bool *p_result);


/**
 * Set GPIO Pin Direction.
 *
 * If output: this function will also set the appropriate level.
 * @param port GPIO port
 * @param pin GPIO pin
 * @param dir true: output,
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_Set_PinDir(uint8_t port, uint8_t pin, gpio_direction_t dir);


/**
 * Enable/Disable the pin interrupt.
 * @param pin pin
 * @param enable true: enable interrupt, false: disable interrupt
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t GPIO_PinInterrupt_Enable(uint8_t pin, bool enable);

#endif
/* End of file gpio.h */
