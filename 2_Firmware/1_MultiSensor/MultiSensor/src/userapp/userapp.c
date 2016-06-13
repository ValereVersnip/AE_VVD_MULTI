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
 * @file userapp.c
 * @author TimB
 * @date 9-jun.-2016
 * @brief	Brief.
 *
 * Description
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "userapp.h"


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

/* modbus device */
modbusslave_t Modbus;									/**< Modbus slave device */

/* sensor measurement order */
userapp_order_t Order;									/**< order for measurement updates */

/* userapp members */
uint16_t Rgbled_red_green_old;							/**< old value of this register */
uint16_t Rgbled_blue_dim_old;							/**< old value of this register */
uint16_t Rgbled_blon_bloff_glint_mode_old;				/**< old value of this register */
uint16_t Buzzer_control_old;							/**< old value of this register */

bool Humidity_Running;									/**< current state for humitity measurement */

/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the modbus.
 */
static void initialize_modbus();

/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the modbus.
 */
static void initialize_modbus()
{
	modbusslave_config_t modbusConfig;
	modbusConfig.baud 			= USERAPP_MODBUS_BAUDRATE;
	modbusConfig.TxEnablePin 	= USERAPP_MODBUS_TXENABLE_PIN;
	modbusConfig.TxEnablePort 	= USERAPP_MODBUS_TXENABLE_PORT;

	/* get the address found at startup */
	uint8_t addr;
	ADDRESS_Get(&Address, &addr);

	modbusConfig.slaveID = addr;

	MODBUSSLAVE_Init(&Modbus, &modbusConfig);
}


/**
 * User application Buzzer routine.
 */
static void userapp_buzzer()
{
	status_t stat = status_ok;
	uint16_t buzzer_control;
	bool onoff, wantbuzz, buzzing;
	uint16_t buzztime;

	buzzer_control = MODBUSSLAVE_GetRegister(&Modbus, modbus_reg_buzzer_control);

	/* check if something changed, otherwise don't do anything */
	if(buzzer_control != Buzzer_control_old)
	{
		/* first check if we are perfoming a timedbuzz at the moment */
		BUZZER_GetTimedBuzz(&Buzzer_SPK1, &buzzing); /* status allways ok */

		/* turn hard on or off, only if no timed buzz is going on */
		if(!buzzing)
		{
			/* check if we have to start */
			onoff = buzzer_control & 0x0001;
			/* then turn on or off */
			stat = BUZZER_Start(&Buzzer_SPK1, onoff);
			USERAPP_HandleStatus(stat);
		}

		/* then check if we already want to start another timed buzz */
		wantbuzz = buzzer_control & 0x0002;
		if(wantbuzz)
		{
			/* Reset the bit again */
			buzzer_control &= 0xFFFD;
			MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_buzzer_control, buzzer_control);

			buzztime = (buzzer_control >> 4) & 0x0FFF;
			stat = BUZZER_Buzz(&Buzzer_SPK1, buzztime * 100);	/* times 100, because we give buzz time in 0.1 seconds */
			USERAPP_HandleStatus(stat);
		}

		/* update the old value again */
		Buzzer_control_old = MODBUSSLAVE_GetRegister(&Modbus, modbus_reg_buzzer_control);
	}
}


/**
 * User application rgbled routine.
 */
static void userapp_rgbled()
{
	status_t stat = status_ok;
	uint16_t rgbled_red_green;
	uint16_t rgbled_blue_dim;
	uint16_t rgbled_blon_bloff_glint_mode;
	uint32_t color;
	uint8_t dimming;
	rgbled_ledout_t ledout;
	uint32_t glowinterval;
	uint32_t blinkon, blinkoff;

	/* get the current modbus values */
	rgbled_red_green 					= MODBUSSLAVE_GetRegister(&Modbus, modbus_reg_rgbled_red_green);
	rgbled_blue_dim 					= MODBUSSLAVE_GetRegister(&Modbus, modbus_reg_rgbled_blue_dim);
	rgbled_blon_bloff_glint_mode 		= MODBUSSLAVE_GetRegister(&Modbus, modbus_reg_rgbled_blon_bloff_glint_mode);

	/* then only update the rgb stuff, if something changed */
	if(rgbled_red_green != Rgbled_red_green_old || rgbled_blue_dim != Rgbled_blue_dim_old || rgbled_blon_bloff_glint_mode != Rgbled_blon_bloff_glint_mode_old)
	{
		/* update old values */
		Rgbled_red_green_old				= rgbled_red_green;
		Rgbled_blue_dim_old                 = rgbled_blue_dim;
		Rgbled_blon_bloff_glint_mode_old    = rgbled_blon_bloff_glint_mode;

		/* then get the wanted color, dim, glowinterval, and ledoutmode */
		color 			= (uint32_t)(rgbled_red_green << 8) | (uint32_t)((rgbled_blue_dim & 0xFF00) >> 8);
		dimming 		= (uint8_t)(rgbled_blue_dim & 0x00FF);
		glowinterval	= (uint32_t)(((rgbled_blon_bloff_glint_mode & 0x003C) >> 2)*100);
		blinkon			= (uint32_t)(((rgbled_blon_bloff_glint_mode & 0xF800) >> 11)*100);
		blinkoff		= (uint32_t)(((rgbled_blon_bloff_glint_mode & 0x07C0) >> 6)*100);
		ledout			= (rgbled_ledout_t)(rgbled_blon_bloff_glint_mode & 0x0003);

		/* and then set according */
		switch(ledout)
		{
		case rgbled_off:
			stat = RGBLED_LedOff(&RgbLed_D2);
			break;
		case rgbled_on:
			stat = RGBLED_LedOn(&RgbLed_D2, color, dimming);
			break;
		case rgbled_blink:
			stat = RGBLED_LedBlink(&RgbLed_D2, color, dimming, blinkon, blinkoff);
			break;
		case rgbled_glow:
			stat = RGBLED_LedGlow(&RgbLed_D2, color, -1, -1, glowinterval, -1);
			break;
		default:
			stat = status_nok;
			break;
		}
	}
	USERAPP_HandleStatus(stat);
}


/**
 * User application pir routine.
 */
static void userapp_pir()
{
	status_t stat = status_ok;
	uint16_t detectcount;
	uint16_t detecttime;

	/* get the results from the pir */
	stat = PIR_HCSR501_GetResults(&PirHcsr501_M1, &detectcount, &detecttime);
	USERAPP_HandleStatus(stat);

	/* and store them in the modbus registermap */
	MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pir_detectcount, detectcount);
	MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pir_detecttime, detecttime);
}





/**
 *  User application humidity routine.
 *
 * @return true: routine finished, false: routine is still running.
 */
static bool userapp_humidity()
{
	status_t stat = status_ok;
	bool ready;
	uint16_t humidity;
	int16_t temperature;
	bool done = false;

	/* if we are not running, try to start a new conversion */
	if(!Humidity_Running)
	{
		stat = HUMIDITY_DHT22_Start(&HumidityDht22_U5);
		/* if we could start a new conversion (needed time between polls has been passed) */
		if(stat == status_ok)
		{
			Humidity_Running = true;
		}
		else
		{
			done = true;		/* done for now */
		}
	}
	/* if we are running, now check if we are ready, if ready, get result, and set done to true */
	else
	{
		/* check if new results are ready */
		HUMIDITY_DHT22_GetReady(&HumidityDht22_U5, &ready);		/* always ok */
		/* if new results are ready, get them, and put them in the modbusreg */
		if(ready)
		{
			stat = HUMIDITY_DHT22_GetResults(&HumidityDht22_U5, &humidity, &temperature);
			USERAPP_HandleStatus(stat);
			MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_humidity_humidity, humidity);
			MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_humidity_temperature, (uint16_t)temperature);
			done = true;
			Humidity_Running = false;
		}
	}
	return done;
}

/**
 * User application pressure routine.
 */
static void userapp_pressure()
{
	status_t stat = status_ok;
	int32_t temperature;
	uint32_t pressure;

	stat = PRESSURE_BMP280_GetResult(&PressureBmp280_M5, NULL, NULL, &temperature, &pressure);

	if(stat == status_ok)
	{
		/* if the returned status is ok, fill in the values in the modbus registermap */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_pressure_kpa, (uint16_t)(pressure / 1000));	/* divid by 1000 to get KPA */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_pressure_pa, (uint16_t)(pressure % 1000));		/* modulo by 1000 to get PA */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_temperature, (uint16_t)temperature);
	}
	else
	{
		/* if not ok, fill in 0xFFFF, this indicates an error */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_pressure_kpa, 0xFFFF);
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_pressure_pa, 0xFFFF);
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_temperature, 0xFFFF);
	}
}


/**
 * User application temperature routine.
 *
 * @return true: routine finished, false: routine is still running.
 */
static bool userapp_temperature()
{
	status_t stat = status_ok;
	bool done = false;


	return true; /* fixme: change */
}


/**
 * User application gas routine.
 */
static void userapp_gas()
{
	status_t stat = status_ok;

	/* todo */

}


/**
 * User application sound routine.
 */
static void userapp_sound()
{
	status_t stat = status_ok;

	/* todo */

}


/**
 * User application luminosity routine.
 */
static void userapp_luminosity()
{
	status_t stat = status_ok;
	uint32_t lux;

	stat = LUM_TSL2561_SenseLight(&LumTsl2561_M4, NULL, NULL, &lux, false);
	if(stat == status_ok)
	{
		/* if the returned status is ok, fill in the values in the modbus registermap */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_luminosity_lux, (uint16_t)(lux));

	}
	else
	{
		/* if not ok, fill in 0xFFFF, this indicates an error */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_luminosity_lux, 0xFFFF);
	}
}


/**
 * statemachine which runs throug different measurements in the order of the userapp_order_t enum.
 *
 * @note we will start when the Order variable is reset to 0.
 */
static void userapp_updatemeasurements()
{
	bool done;
	switch(Order)
	{
	case order_humidity:
		done = userapp_humidity();		/* because this is a time divided function, check if it is done */
		if(done)
		{
			Order++;
		}
		break;
	case order_temperature:
		done = userapp_temperature();	/* because this is a time divided function, check if it is done */
		if(done)
		{
			Order++;
		}
		break;
	case order_pressure:
		userapp_pressure();
		Order++;
		break;
	case order_pir:
		userapp_pir();
		Order++;
		break;
	case order_rgbled:
		userapp_rgbled();
		Order++;
		break;
	case order_buzzer:
		userapp_buzzer();
		Order++;
		break;
	case order_luminosity:
		userapp_luminosity();
		Order++;
		break;
	case order_gas:
		userapp_gas();
		Order++;
		break;
	case order_sound:
		userapp_sound();
		Order++;
		break;
	case order_done:
		break;
	default:
		break;
	}
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the userapp.
 */
void USERAPP_Init()
{
	uint8_t i;

	/* order */
	Order = 0;

	/* Humidity not running */
	Humidity_Running = false;

	/* initialize the modbus */
	initialize_modbus();

	/* reset all modbus registers */
	for(i = 0; i < modbus_reg_size; i++)
	{
		MODBUSSLAVE_SetRegister(&Modbus, i, 0);
	}

	/* set the version */
	/* version: B11-8 = MAJOR, B7-4 = MINOR, B3-0 = CHANGE */
	uint16_t  version = (USERAPP_VERSION_MAJOR << 8) | (USERAPP_VERSION_MINOR << 4) | (USERAPP_VERSION_CHANGE << 0);
	MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_version, version);
}



/**
 * Run function for the user app.
 *
 * @note this function should be called periodically by higher level routines.
 */
void USERAPP_Run0()
{
	bool justresponded;

	/* modbus run */
	MODBUSSLAVE_Run0(&Modbus);

	/* check if we just responded to a modbus request for this id, this is the best time to begin blocking transactions (such as humidity and temperature readout) */
	justresponded = MODBUSSLAVE_GetSetJustResponded(&Modbus);

	/* if just responded is found, start the measurement updates */
	if(justresponded)
	{
		Order = 0;	/* to start the measurements, reset the Order */
	}

	/* run function for updating measurements */
	userapp_updatemeasurements();



}




/**
 * Handle the status.
 *
 * First make exceptions for known bad statusses...
 * If we don't get status_ok, we will put on the red led, and trap here.
 * So we can attach our debugger and watch the failing callstack.
 * @param stat status
 */
void USERAPP_HandleStatus(status_t stat)
{
	/* first make exceptions. */

//	/* if we get bad crc for dht22, this is because we are receiving uart requests, at the same time as readout. No ready flag will be */
//	if(stat = humidity_dht22_crc)
//	{
//
//	}


	if(stat != status_ok)
	{
		while(1)
		{
			/* if something went wrong, set the color to red, and trap here! */
			RGBLED_LedOn(&RgbLed_D2, COLOR_RGB_RED, 20);
			while(1)
			{
				stat = stat;
			}
		}
	}
}

/* End of file userapp.c */
