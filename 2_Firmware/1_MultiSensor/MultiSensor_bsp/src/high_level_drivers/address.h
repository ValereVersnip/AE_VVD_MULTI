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
 * @file address.h
 * @author TimB
 * @date 21-mei-2016
 * @brief	Header file for address.c
 *
 */

#ifndef DEF_address_H
#define DEF_address_H

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
#define ADDRESS_BITAMOUNT				8				/**< ADDRESS NR OF BITS */


/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */


/**
 * address config
 */
typedef struct address_config_t
{
	uint8_t port[ADDRESS_BITAMOUNT];							/**< port  */
	uint8_t pin[ADDRESS_BITAMOUNT];								/**< pins */
}address_config_t;

/**
 * address Struct
 */
typedef struct address_t
{
	uint8_t port[ADDRESS_BITAMOUNT];							/**< port  */
	uint8_t pin[ADDRESS_BITAMOUNT];								/**< pins */
	uint8_t address;											/**< the address found at initialization */
}address_t;
/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */


/**
 * Initialize address.
 * @param p_address address device
 * @param p_config address config
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADDRESS_Init(address_t *p_address, address_config_t *p_config);

/**
 * Get the address setting, as was found at startup.
 *
 * @note This will not read the values again from gpio!
 * @param p_address address device
 * @param p_result pointer to result
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADDRESS_Get(address_t *p_address, uint8_t *p_result);


#endif
/* End of file address.h */
