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

	stat = LOWLEVELDRIVERS_Init();
	if(stat != status_ok)
	{
		while(1)
		{
			stat = stat;
		}
	}

	stat = HIGHLEVELDRIVERS_Init();
	if(stat != status_ok)
	{
		RGBLED_LedOn(&RgbLed_D2, COLOR_RGB_RED, 20);
		while(1)
		{
			stat = stat;
		}
	}


//	RGBLED_LedGlow(&RgbLed_d2, -1, -1, -1, -1, -1);
	//RGBLED_LedBlink(&RgbLed_D2, -1, -1, -1, -1);

	/*while(1)
	{
		RGBLED_LedOff(&RgbLed_D2);
		stat = LUM_TSL2561_SenseLight(&LumTsl2561_M4, NULL, NULL, NULL, false);
		stat = PRESSURE_BMP280_GetResult(&PressureBmp280_M5, NULL, NULL, NULL, NULL);
	}



	SYSTICK_DelayMs(2000);






		do
		{
			stat = HUMIDITY_DHT22_Start(&HumidityDht22_U5);
		}while(stat != status_ok);*/

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

		uint16_t treshCount, clapCount;
		treshCount = clapCount = 0;



    while(1) {
    	stat = LOWLEVELDRIVERS_Run0();
    	if(stat != status_ok)
    	{
    		RGBLED_LedOn(&RgbLed_D2, COLOR_RGB_RED, 20);
    		while(1)
    		{
    			stat = stat;
    		}
    	}

    	stat = HIGHLEVELDRIVERS_Run0();
    	if(stat != status_ok)
    	{
    		RGBLED_LedOn(&RgbLed_D2, COLOR_RGB_RED, 20);
    		while(1)
    		{
    			stat = stat;
    		}
    	}

    	if(Microphone_M6.tresholddetectioncount > treshCount)
    	{
    		RGBLED_LedOn(&RgbLed_D2, COLOR_RGB_BLUE, 20);
    		treshCount = Microphone_M6.tresholddetectioncount;
    	}
    	if(Microphone_M6.clapdetectioncount > clapCount)
    	{
    		RGBLED_LedOn(&RgbLed_D2, COLOR_RGB_GREEN, 20);
    		clapCount = Microphone_M6.clapdetectioncount;
    	}


//		SYSTICK_GetTicks(&nowtime);
//
//		if(nowtime - prevtime > 2000)
//		{
//			prevtime = nowtime;
//			//stat = BUZZER_Buzz(&Buzzer_SPK1, 200);
//		}


//    	if(nowtime - prevtime > 2000)
//    	{
//    		prevtime = nowtime;
//    		stat = HUMIDITY_DHT22_Start(&HumidityDht22_U5);
//    	}
//
//    	if(HumidityDht22_U5.ready)
//    	{
//    		HumidityDht22_U5.ready = false;
//			/*test crc*/
//			uint8_t myCrc = (uint8_t)(HumidityDht22_U5.humidity >> 8) + (uint8_t)(HumidityDht22_U5.humidity >> 0) + (uint8_t)(HumidityDht22_U5.temperature >> 8) + (uint8_t)(HumidityDht22_U5.temperature >>0);
//			if(myCrc != HumidityDht22_U5.crc)
//			{
//				myCrc = myCrc;
//			}
//			else
//			{
//				myCrc = myCrc;
//			}
//    	}

    }
    return 0 ;
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */



/* End of file MultiSensor.c */
