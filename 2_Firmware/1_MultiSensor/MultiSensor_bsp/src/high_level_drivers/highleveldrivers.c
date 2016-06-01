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
 * @file highleveldrivers.c
 * @author TimB
 * @date 21-mei-2016
 * @brief	Highleveldrivers device collection, initialization and run functions.
 *
 * All high level drivers device structs are declared here.
 * These are initialized here.
 * All high level drivers run functions are grouped here, into 1 highleveldrivers run function, which should be called periodically by higher level routines.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "highleveldriversconfig.h"
#include "highleveldrivers.h"

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

/* rgbled devices */
rgbled_t RgbLed_D2;						/**< RGBLED D2 */

/* address device */
address_t Address;						/**< Address */

/* humidity dht22 device */
humidity_dht22_t HumidityDht22_U5;		/**< Humidity DHT22 sensor U5 */

/* pressure bmp280 device */
pressure_bmp280_t PressureBmp280_M5;	/**< Pressure BMP280 sensor M5 */

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


/**
 * Initialize the rgbled.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t initialize_rgbled()
{
	status_t status = status_ok;
	rgbled_config_t config;

	config.id	          =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_ID;
	config.p_pwm          =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_PWM;
	config.red_channel    =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_REDCHANNEL;
	config.green_channel  =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GREENCHANNEL;
	config.blue_channel   =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_BLUECHANNEL;
	config.ledout         =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_LEDOUT;
	config.blinkontime    =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_BLINKONTIME;
	config.blinkofftime   =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_BLINKOFFTIME;
	config.glow_dim_min   =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GLOWDIMMIN;
	config.glow_dim_max   =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GLOWDIMMAX;
	config.glowinterval   =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GLOWINTERVAL;
	config.glowstep       =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_GLOWSTEP;
	config.color          =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_COLOR;
	config.dim            =   HIGHLEVELDRIVERSCONFIG_RGBLED_D2_DIM;


	status = RGBLED_Init(&RgbLed_D2, &config);
	return status;
}


/**
 * Initialize the address.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t initialize_address()
{
	status_t status = status_ok;
	address_config_t addressConfig;

	addressConfig.port[0]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT;
	addressConfig.port[1]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT;
	addressConfig.port[2]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT;
	addressConfig.port[3]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT;
	addressConfig.port[4]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT;
	addressConfig.port[5]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT;
	addressConfig.port[6]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT;
	addressConfig.port[7]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PORT;

	addressConfig.pin[0]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_0;
	addressConfig.pin[1]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_1;
	addressConfig.pin[2]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_2;
	addressConfig.pin[3]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_3;
	addressConfig.pin[4]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_4;
	addressConfig.pin[5]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_5;
	addressConfig.pin[6]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_6;
	addressConfig.pin[7]	=	HIGHLEVELDRIVERSCONFIG_ADDRESS_PIN_7;

	status = ADDRESS_Init(&Address, &addressConfig);

	return status;
}


/**
 * Initialize the humidity dht22 sensors.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t initialize_humidity_dht22()
{
	status_t status = status_ok;
	humidity_dht22_config_t config;

	config.id				=	HIGHLEVELDRIVERSCONFIG_HUMIDITYDHT22_U5_ID;
	config.gpioport			=	HIGHLEVELDRIVERSCONFIG_HUMIDITYDHT22_U5_GPIOPORT;
	config.gpiopin			=	HIGHLEVELDRIVERSCONFIG_HUMIDITYDHT22_U5_GPIOPIN;
	config.mrt				=	HIGHLEVELDRIVERSCONFIG_HUMIDITYDHT22_U5_MRT;

	status = HUMIDITY_DHT22_Init(&HumidityDht22_U5, &config);

	return status;
}


/**
 * Initialize the pressure bmp280 sensors.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t initialize_pressure_bmp280()
{
	status_t status = status_ok;
	pressure_bmp280_config_t config;

	config.id				=	HIGHLEVELDRIVERSCONFIG_PRESSUREBMP280_M5_ID;
	config.p_i2c			=	HIGHLEVELDRIVERSCONFIG_PRESSUREBMP280_M5_I2C;
	config.address			=	HIGHLEVELDRIVERSCONFIG_PRESSUREBMP280_M5_ADDRESS;

	status = PRESSURE_BMP280_Init(&PressureBmp280_M5, &config);

	return status;
}

/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize all highlevel drivers.
 *
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HIGHLEVELDRIVERS_Init()
{
	status_t status = status_ok;

	/* Initialize RGB leds */
	if(status == status_ok)
	{
		status = initialize_rgbled();
	}

	/* Initialize Adrress */
	if(status == status_ok)
	{
		status = initialize_address();
	}

	/* Initialize Humidity dht22 */
	if(status == status_ok)
	{
		status = initialize_humidity_dht22();
	}

	/* Initialize Pressure bmp280 */
	if(status == status_ok)
	{
		status = initialize_pressure_bmp280();
	}

	return status;
}


/**
 * highleveldrivers Run0 Function.
 *
 * @note This function should be called periodically by higher level routines.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t HIGHLEVELDRIVERS_Run0()
{
	status_t status = status_ok;

	/* rgbled Run0 */
	if(status == status_ok)
	{
		status = RGBLED_Run0(&RgbLed_D2);
	}

	/* humidity dht22 Run0 */
	if(status == status_ok)
	{
		status = HUMIDITY_DHT22_Run0(&HumidityDht22_U5);
	}

	if(status != status_ok)
	{
		RGBLED_LedOn(&RgbLed_D2, COLOR_RGB_RED, 10);
		while(1)
		{
			status = status;
		}
	}

	return status;
}


/**
 * interrupt routine for DHT20 data pin falling edge handler.
 *
 * This function will be called when the data pin goes low, and we will get the 10s ticks.
 *
 * @note: this handler will be given to low level driver gpio pin interrupt routine in lowleveldrivers config file.
 */
void HIGHLEVELRIVERS_DHT22_Pin_Handler()
{
	HUMIDITY_DHT22_FallingEdgeRoutine(&HumidityDht22_U5);
}


/* End of file highleveldrivers.c */