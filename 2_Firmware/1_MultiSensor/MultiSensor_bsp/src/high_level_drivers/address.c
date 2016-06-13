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
 * @file address.c
 * @author TimB
 * @date 21-mei-2016
 * @brief	Routines for Address.
 *
 * In the init function, we get the address present on the pins.
 * After init, the address cannot change, we just read out struct.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "address.h"

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
 * Initialize address.
 * @param p_address address device
 * @param p_config address config
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADDRESS_Init(address_t *p_address, address_config_t *p_config)
{
	status_t status = status_ok;
	uint8_t i;
	bool gpioState;

	for(i=0; i<ADDRESS_BITAMOUNT; i++)
	{
		p_address->port[i]	= p_config->port[i];
		p_address->pin[i]	= p_config->pin[i];
	}

	p_address->address = 0;
	for(i = 0; i < ADDRESS_BITAMOUNT; i++)
	{
		/* get the pin state */
		GPIO_Get_Pin(p_address->port[i], p_address->pin[i], &gpioState);
		/* and shift in result */
		if(!gpioState) /* inverse because of how the switch works */
		{
			p_address->address = (p_address->address << 1) | 0x01;
		}
		else
		{
			p_address->address = (p_address->address << 1) & 0xFE;
		}
	}

	return status;
}


/**
 * Get the address setting, as was found at startup.
 *
 * @note This will not read the values again from gpio!
 * @param p_address address device
 * @param p_result pointer to result
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADDRESS_Get(address_t *p_address, uint8_t *p_result)
{
	status_t status = status_ok;

	*p_result = p_address->address;

	return status;
}

/* End of file address.c */
