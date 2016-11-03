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
volatile userapp_order_t Order;							/**< order for measurement updates */

/* userapp members */
uint16_t Rgbled_red_green_old;							/**< old value of this register */
uint16_t Rgbled_blue_dim_old;							/**< old value of this register */
uint16_t Rgbled_blon_bloff_glint_mode_old;				/**< old value of this register */
uint16_t Buzzer_control_old;							/**< old value of this register */

uint8_t HumidityErrorCount;								/**< error count for Humidity sensor */

uint8_t TemperatureErrorCount;							/**< error count for Temperature sensor */

uint16_t GasAlarmCount;									/**< to keep track of how many samples were above the gas treshold. */

uint16_t SoundAlarmCount;								/**< to keep track of how many samples were above the sound treshold. */

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
	rgbled_blon_bloff_glint_mode 		= MODBUSSLAVE_GetRegister(&Modbus, modbus_reg_rgbled_control);

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

		/* extra check for dimming, it can be maximum 100! */
		if(dimming > 100)
		{
			dimming = 100;
			/* and immediately change in modbusreg */
			MODBUSSLAVE_WriteField(&Modbus, modbus_reg_rgbled_blue_dim, MODBUSSLAVE_RGBLED_BLUE_DIM_BIT_DIM, dimming);
		}

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
	stat = PIR_HCSR501_GetResults(&PirHcsr501_M1, &detectcount, &detecttime); /* Always ok */
	USERAPP_HandleStatus(stat);

	/* and store them in the modbus registermap */
	MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pir_detectcount, detectcount);
	MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pir_detecttime, detecttime);

	/* also store the 8LSB bits in the FAST pircount register */
	MODBUSSLAVE_WriteField(&Modbus, modbus_reg_fast_pircount_soundpercent, MODBUSSLAVE_FAST_PIRCOUNT_SOUNDPERCENT_PIRCOUNT, (uint16_t)(0xFF & detectcount)); /* truncate to the 8LSB of detectcount */
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
	bool running;

	/* get the running flag */
	HUMIDITY_DHT22_GetRunning(&HumidityDht22_U5, &running);
	/* also get the ready flag */
	HUMIDITY_DHT22_GetReady(&HumidityDht22_U5, &ready);		/* always ok */


	/* if we are not running, check if we are ready, if ready, get result, and set done to true */
	if(ready)
	{
		stat = HUMIDITY_DHT22_GetResults(&HumidityDht22_U5, &humidity, &temperature);
		USERAPP_HandleStatus(stat);
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_humidity, humidity);
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_humidity_temp, (uint16_t)temperature);
		done = true;
		/* reset the error count, as we want to know consecutive errors, and also reset the error bit in the status register */
		HumidityErrorCount = 0;
		MODBUSSLAVE_DiscreteClear(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_HUM_ERR);
	}
	/* if we are not running, try to start a new conversion */
	else if(!running)
	{
		stat = HUMIDITY_DHT22_Start(&HumidityDht22_U5);
		/* if we could not start a new conversion (needed time between polls has not been passed) */
		if(stat != status_ok)
		{
			done = true;
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
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_kpa, (uint16_t)(pressure / 1000));	/* divid by 1000 to get KPA */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_pa, (uint16_t)(pressure % 1000));		/* modulo by 1000 to get PA */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_temp, (uint16_t)temperature);
		/* and clear error bit in status register */
		MODBUSSLAVE_DiscreteClear(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_PRESS_ERR);
	}
	else
	{
		/* if not ok, fill in 0xFFFF, this indicates an error */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_kpa, 0xFFFF);
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_pa, 0xFFFF);
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_pressure_temp, 0xFFFF);
		/* and set error bit in status register */
		MODBUSSLAVE_DiscreteSet(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_PRESS_ERR);
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
	bool ready;
	bool running;
	int16_t temperature_int;
	uint16_t temperature_frac;


	/* get the running flag */
	TEMP_DS18B20_GetRunningFlag(&Tempds18b20_U4, &running);
	/* get the ready flag */
	TEMP_DS18B20_GetReadyFlag(&Tempds18b20_U4, &ready);		/* always ok */


	/* if new results are ready, get them, and put them in the modbusreg */
	if(ready)
	{
		stat = TEMP_DS18B20_GetResult(&Tempds18b20_U4, &temperature_int, &temperature_frac);
		USERAPP_HandleStatus(stat);
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_temp_int, (uint16_t)temperature_int);
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_temp_frac, temperature_frac);
		/* reset the error count, as we want to know consecutive errors, and also reset the error bit in the status register */
		TemperatureErrorCount = 0;
		MODBUSSLAVE_DiscreteClear(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_TEMP_ERR);
	}
	/* if we are not running, try to start a new conversion */
	else if(!running)
	{
		stat = TEMP_DS18B20_Start(&Tempds18b20_U4);
	}

	/* otherwise, trigger the temp ds18b20 state machine that it is ok to readout starting from now */
	else
	{
		stat = TEMP_DS18B20_OkToReadOut(&Tempds18b20_U4);	/* always ok */
	}

	//return done;
	return true;
}


/**
 * User application gas routine.
 */
static void userapp_gas()
{
	uint16_t treshold;
	uint16_t count;
	uint16_t value;

	/* set the latest treshold value */
	treshold = MODBUSSLAVE_ReadField(&Modbus, modbus_reg_gas_treshold, MODBUSSLAVE_GAS_TRESHOLD_BIT_TRESHOLD);
	GAS_MQ2_SetTreshold(&GasMq2_M2, treshold); /* always OK */

	/* get the latest alarmcount, and latest gas value (in mV) */
	GAS_MQ2_GetResult(&GasMq2_M2, &value, &count);

	/* store the latest value in modbus reg */
	MODBUSSLAVE_WriteField(&Modbus, modbus_reg_gas_mv, MODBUSSLAVE_GAS_MV_BIT_GAS, value);

	/* if we got more samples over treshold as previous time, OR the current value is also above the treshold, set the alarm bit, otherwise reset it */
	if(count > GasAlarmCount || value > treshold)
	{
		/* first update count */
		GasAlarmCount = count;
		/* and set the alarmbit */
		MODBUSSLAVE_DiscreteSet(&Modbus, modbus_reg_gas_treshold, MODBUSSLAVE_GAS_TRESHOLD_BIT_ALARM);
	}
	else
	{
		/* clear the alarmbit */
		MODBUSSLAVE_DiscreteClear(&Modbus, modbus_reg_gas_treshold, MODBUSSLAVE_GAS_TRESHOLD_BIT_ALARM);
	}


}


/**
 * User application sound routine.
 */
static void userapp_sound()
{
	uint16_t treshold;
	uint16_t treshtime;
	uint16_t treshcount;
	uint16_t latestvalue;
	uint32_t latestvaluepercent;

	/* set the latest treshold and  tresholdttime value */
	treshold = MODBUSSLAVE_ReadField(&Modbus, modbus_reg_sound_treshold, MODBUSSLAVE_SOUND_TRESHOLD_BIT_TRESHOLD);
	treshtime = MODBUSSLAVE_GetRegister(&Modbus, modbus_reg_sound_tresholdtime);
	MICROPHONE_SetTreshold(&Microphone_M6, treshold, treshtime); /* always OK */

	/* get the latest alarmcount, and latest sound value (in mV) */
	MICROPHONE_GetResults(&Microphone_M6, &treshcount, &latestvalue); /* always OK */


	/* store the latest value in modbus reg */
	MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_sound_mv, latestvalue);

	/* also store the latest value in percents in the fast register */
	latestvaluepercent = (uint32_t)((latestvalue * 100) / 255);
	MODBUSSLAVE_WriteField(&Modbus, modbus_reg_fast_pircount_soundpercent, MODBUSSLAVE_FAST_PIRCOUNT_SOUNDPERCENT_SOUNDPERCENT, (uint16_t)latestvaluepercent);

	/* if we got more samples over treshold as previous time, OR the current value is also above the treshold, set the alarm bit, otherwise reset it */
	if(treshcount > SoundAlarmCount || latestvalue > treshold)
	{
		/* first update count */
		SoundAlarmCount = treshcount;
		/* and set the alarmbit */
		MODBUSSLAVE_DiscreteSet(&Modbus, modbus_reg_sound_treshold, MODBUSSLAVE_SOUND_TRESHOLD_BIT_ALARM);
	}
	else
	{
		/* clear the alarmbit */
		MODBUSSLAVE_DiscreteClear(&Modbus, modbus_reg_sound_treshold, MODBUSSLAVE_SOUND_TRESHOLD_BIT_ALARM);
	}


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
		/* and clear error bit in status register */
		MODBUSSLAVE_DiscreteClear(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_LUM_ERR);
	}
	else
	{
		/* if not ok, fill in 0xFFFF, this indicates an error */
		MODBUSSLAVE_SetRegister(&Modbus, modbus_reg_luminosity_lux, 0xFFFF);
		/* and set error bit in status register */
		MODBUSSLAVE_DiscreteSet(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_LUM_ERR);
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


void userapp_checkglobalerror()
{
	uint16_t readvalue;
	/* if one of the bits (except bit 0) of the status register is set, there is an error */
	readvalue = MODBUSSLAVE_GetRegister(&Modbus, modbus_reg_status);
	if(readvalue & 0xFE)
	{
		MODBUSSLAVE_DiscreteSet(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_ERR);
	}
	else
	{
		MODBUSSLAVE_DiscreteClear(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_ERR);
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
	HumidityErrorCount = 0;

	/* no alarms found for gas */
	GasAlarmCount = 0;

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

	/* check the global error bit */
	userapp_checkglobalerror();

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

	/* first make exceptions, for time driven measurement bad statusses (these will be returned in higherlevel run function). */
	/**************************************************************************************************************************/

	/* if we get bad crc for dht22, this is because we are receiving uart requests, at the same time as readout. No ready flag will be set */
	if(stat == humidity_dht22_crc)
	{
		/* anyway increment the order, so we go to the next measurement */
		Order++;
		/* then increment the HumidityErrorCount */
		HumidityErrorCount++;
		/* if we get to many consecutive errors, raise error flag in status register*/
		if(HumidityErrorCount > USERAPP_HUMIDITY_CONSECUTIVE_ERRTH)
		{
			MODBUSSLAVE_DiscreteSet(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_HUM_ERR);
		}
		/* overwrite stat to status_ok */
		stat = status_ok;
	}


	/* if we get bad crc for temp ds18b20 ram or rom, this is because we are receiving uart requests, at the same time as readout. No ready flag will be set */
	if(stat == temp_ds18b20_ramcrc || stat == temp_ds18b20_romcrc)
	{
		/* anyway increment the order, so we go to the next measurement */
		Order++;
		/* then increment the HumidityErrorCount */
		TemperatureErrorCount++;
		/* if we get to many consecutive errors, raise error flag in status register*/
		if(TemperatureErrorCount > USERAPP_TEMPERATURE_CONSECUTIVE_ERRTH)
		{
			MODBUSSLAVE_DiscreteSet(&Modbus, modbus_reg_status, MODBUSSLAVE_STATUS_BIT_TEMP_ERR);
		}
		/* overwrite stat to status_ok */
		stat = status_ok;
	}


	/* for debugging, really trap here, TODO: comment this for release */
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
