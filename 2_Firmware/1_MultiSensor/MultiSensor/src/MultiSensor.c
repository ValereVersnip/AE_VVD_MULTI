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
 * @file MultiSensor.c
 * @author TimB
 * @date 20-mei-2016
 * @brief	Brief.
 *
 * Description
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
 #include "multisensor_bsp.h"
#include <cr_section_macros.h>

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

int main(void) {

	status_t stat;

	/* Read clock settings and update SystemCoreClock variable */
	    SystemCoreClockUpdate();

	/* Initialization of drivers */

	LOWLEVELDRIVERS_Init();
	HIGHLEVELDRIVERS_Init();


//	RGBLED_LedGlow(&RgbLed_d2, -1, -1, -1, -1, -1);
	RGBLED_LedBlink(&RgbLed_D2, -1, -1, -1, -1);

	SYSTICK_DelayMs(2000);

		do
		{
			stat = HUMIDITY_DHT22_Start(&HumidityDht22_U5);
		}while(stat != status_ok);

//		while(!HumidityDht22_U5.ready)
//		{
//			HumidityDht22_U5.ready = HumidityDht22_U5.ready;
//		}
//
//		HumidityDht22_U5.ready = false;
//		/*test crc*/
//		uint8_t myCrc = (uint8_t)(HumidityDht22_U5.humidity + HumidityDht22_U5.temperature);
//		if(myCrc != HumidityDht22_U5.crc)
//		{
//			myCrc = myCrc;
//		}
//		else
//		{
//			myCrc = myCrc;
//		}



		uint32_t prevtime, nowtime;

		SYSTICK_GetTicks(&prevtime);
    while(1) {
    	LOWLEVELDRIVERS_Run0();
    	HIGHLEVELDRIVERS_Run0();

		SYSTICK_GetTicks(&nowtime);
    	if(nowtime - prevtime > 2000)
    	{
    		prevtime = nowtime;
    		stat = HUMIDITY_DHT22_Start(&HumidityDht22_U5);
    	}

    	if(HumidityDht22_U5.ready)
    	{
    		HumidityDht22_U5.ready = false;
			/*test crc*/
			uint8_t myCrc = (uint8_t)(HumidityDht22_U5.humidity >> 8) + (uint8_t)(HumidityDht22_U5.humidity >> 0) + (uint8_t)(HumidityDht22_U5.temperature >> 8) + (uint8_t)(HumidityDht22_U5.temperature >>0);
			if(myCrc != HumidityDht22_U5.crc)
			{
				myCrc = myCrc;
			}
			else
			{
				myCrc = myCrc;
			}
    	}

    }
    return 0 ;
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */



/* End of file MultiSensor.c */
