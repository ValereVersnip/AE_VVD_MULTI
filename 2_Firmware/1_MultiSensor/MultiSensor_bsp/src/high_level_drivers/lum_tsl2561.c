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
 * @file lum_tsl2561.c
 * @author TimB
 * @date 21-mei-2016
 * @brief	Brief.
 *
 * Description
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "lum_tsl2561.h"

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
/**
 * Write 8bit value over i2c bus.
 *
 * @param p_tsl2561 tsl2561 device
 * @param reg register to write to
 * @param value 8 bit value to write.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t lum_tsl2561_write8(lum_tsl2561_t *p_tsl2561, uint8_t reg, uint8_t value);


/**
 * Read 2 bytes from i2c bus getting a 16 bit unsigned integer.
 *
 * @param p_tsl2561 tsl2561 device
 * @param reg first reg to read from
 * @param p_value pointer to 16bit result
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t lum_tsl2561_read16(lum_tsl2561_t *p_tsl2561, uint8_t reg, uint16_t *p_value);


/**
 * Get the latest fullspectrum and infrared data over i2c.
 *
 * @note the result will be saved in the device struct, result pointers added for convenience, they can be made NULL if not needed.
 * @note this will get the latest result available in the device.
 * @param p_tsl2561 tsl2561 device
 * @param p_fullspectrum fullspectrum (can be NULL if not needed)
 * @param p_infrared infrared (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t lum_tsl2561_getdata(lum_tsl2561_t *p_tsl2561, uint16_t *p_fullspectrum, uint16_t *p_infrared);



/**
 * Convert the raw sensorvalues to the standard SI lux equivalent.
 *
 * @note the result will be saved in the device struct, result pointers added for convenience, they can be made NULL if not needed.
 * @param p_tsl2561 tsl2561 device
 * @param broadband raw fullspectrum value
 * @param ir raw infrared value
 * @param p_lux lux result (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t lum_tsl2561_calculatelux(lum_tsl2561_t *p_tsl2561, uint16_t broadband, uint16_t ir, uint32_t *p_lux);

/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Write 8bit value over i2c bus.
 *
 * @param p_tsl2561 tsl2561 device
 * @param reg register to write to
 * @param value 8 bit value to write.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t lum_tsl2561_write8(lum_tsl2561_t *p_tsl2561, uint8_t reg, uint8_t value)
{
	status_t status = status_ok;
	uint8_t tx[2];
	tx[0] = reg;	/* command | register in first place */
	tx[1] = value; 	/* value in second place */

	/* write over i2c */
	status = I2C_Transfer_Blocking(p_tsl2561->p_i2c, p_tsl2561->address, tx, 2, NULL, 0);

	return status;
}


/**
 * Read 2 bytes from i2c bus getting a 16 bit unsigned integer.
 *
 * @param p_tsl2561 tsl2561 device
 * @param reg first reg to read from
 * @param p_value pointer to 16bit result
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t lum_tsl2561_read16(lum_tsl2561_t *p_tsl2561, uint8_t reg, uint16_t *p_value)
{
	status_t status = status_ok;
	uint8_t tx;
	uint8_t rx[2];

	tx = reg;	/* command | register */

	/* write/read over i2c */
	status = I2C_Transfer_Blocking(p_tsl2561->p_i2c, p_tsl2561->address, &tx, 1, rx, 2);

	if(status == status_ok)
	{
		/* copy to result */
		*p_value = (uint16_t)((rx[1] << 8) | (rx[0] << 0));
	}
	return status;
}


/**
 * Get the latest fullspectrum and infrared data over i2c.
 *
 * @note the result will be saved in the device struct, result pointers added for convenience, they can be made NULL if not needed.
 * @note this will get the latest result available in the device.
 * @param p_tsl2561 tsl2561 device
 * @param p_fullspectrum fullspectrum (can be NULL if not needed)
 * @param p_infrared infrared (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t lum_tsl2561_getdata(lum_tsl2561_t *p_tsl2561, uint16_t *p_fullspectrum, uint16_t *p_infrared)
{
	status_t status = status_ok;
	uint16_t fullspectrum_temp;
	uint16_t infrared_temp;

	fullspectrum_temp = 0;
	infrared_temp = 0;

	if(status == status_ok)
	{
		/* get the fullspectrum over i2c */
		status = lum_tsl2561_read16(p_tsl2561, LUM_TSL2561_COMMAND_BIT | LUM_TSL2561_WORD_BIT | LUM_TSL2561_REGISTER_CHAN0_LOW, &fullspectrum_temp);
	}
	if(status == status_ok)
	{
		/* get the infrared over i2c */
		status = lum_tsl2561_read16(p_tsl2561, LUM_TSL2561_COMMAND_BIT | LUM_TSL2561_WORD_BIT | LUM_TSL2561_REGISTER_CHAN1_LOW, &infrared_temp);
	}
	if(status == status_ok)
	{
		/* save in struct */
		p_tsl2561->fullspectrum = fullspectrum_temp;
		p_tsl2561->infrared = infrared_temp;

		/* save in pointers if needed */
		UTILITIES_StoreInPointer(p_fullspectrum, fullspectrum_temp);
		UTILITIES_StoreInPointer(p_infrared, infrared_temp);
	}
	return status;
}


/**
 * Convert the raw sensorvalues to the standard SI lux equivalent.
 *
 * @note the result will be saved in the device struct, result pointers added for convenience, they can be made NULL if not needed.
 * @param p_tsl2561 tsl2561 device
 * @param broadband raw fullspectrum value
 * @param ir raw infrared value
 * @param p_lux lux result (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t lum_tsl2561_calculatelux(lum_tsl2561_t *p_tsl2561, uint16_t broadband, uint16_t ir, uint32_t *p_lux)
{
	status_t status = status_ok;
	uint32_t chScale;
	uint32_t channel1;
	uint32_t channel0;
	uint16_t clipThreshold;
	uint32_t ratio1 = 0;
	uint32_t ratio;
	int32_t b, m;
	uint32_t temp;
	uint32_t lux;

	/* first make sure that the sensor isn't saturated */
	switch (p_tsl2561->integration_time)
	{
		case LUM_TSL2561_INTEGRATIONTIME_13MS:
			clipThreshold = LUM_TSL2561_CLIPPING_13MS;
			break;
		case LUM_TSL2561_INTEGRATIONTIME_101MS:
			clipThreshold = LUM_TSL2561_CLIPPING_101MS;
			break;
		case LUM_TSL2561_INTEGRATIONTIME_402MS:
			clipThreshold = LUM_TSL2561_CLIPPING_402MS;
			break;
		default:
			clipThreshold = LUM_TSL2561_CLIPPING_402MS;
			break;
	}

	/* check for saturated */
	if ((broadband > clipThreshold) || (ir > clipThreshold))
	{
		status = lum_tsl2561_saturated;
	}

	if(status == status_ok)
	{
		/* get the correct scale depending on the integration time */
		switch (p_tsl2561->integration_time)
		{
			case LUM_TSL2561_INTEGRATIONTIME_13MS:
				chScale = LUM_TSL2561_LUX_CHSCALE_TINT0;
				break;
			case LUM_TSL2561_INTEGRATIONTIME_101MS:
				chScale = LUM_TSL2561_LUX_CHSCALE_TINT1;
				break;
			case LUM_TSL2561_INTEGRATIONTIME_402MS:
				chScale = (1 << LUM_TSL2561_LUX_CHSCALE);
				break;
			default: /* No scaling ... integration time = 402ms */
				chScale = (1 << LUM_TSL2561_LUX_CHSCALE);
				break;
		}


		/* scale for gain (1x or 16x) */
		if (!p_tsl2561->gain)
		{
			chScale = chScale << 4;
		}

		/* scale the channel values */
		channel0 = (broadband * chScale) >> LUM_TSL2561_LUX_CHSCALE;
		channel1 = (ir * chScale) >> LUM_TSL2561_LUX_CHSCALE;

		/* find the ratio of the channel values (Channel1/Channel0) */
		if (channel0 != 0)
		{
			ratio1 = (channel1 << (LUM_TSL2561_LUX_RATIOSCALE + 1)) / channel0;
		}

		/* round the ratio value */
		ratio = (ratio1 + 1) >> 1;

		#ifdef TSL2561_PACKAGE_CS
		if ((ratio >= 0) && (ratio <= LUM_TSL2561_LUX_K1C))
			{b=LUM_TSL2561_LUX_B1C; m=LUM_TSL2561_LUX_M1C;}
		else if (ratio <= LUM_TSL2561_LUX_K2C)
			{b=LUM_TSL2561_LUX_B2C; m=LUM_TSL2561_LUX_M2C;}
		else if (ratio <= LUM_TSL2561_LUX_K3C)
			{b=LUM_TSL2561_LUX_B3C; m=LUM_TSL2561_LUX_M3C;}
		else if (ratio <= LUM_TSL2561_LUX_K4C)
			{b=LUM_TSL2561_LUX_B4C; m=LUM_TSL2561_LUX_M4C;}
		else if (ratio <= LUM_TSL2561_LUX_K5C)
			{b=LUM_TSL2561_LUX_B5C; m=LUM_TSL2561_LUX_M5C;}
		else if (ratio <= LUM_TSL2561_LUX_K6C)
			{b=LUM_TSL2561_LUX_B6C; m=LUM_TSL2561_LUX_M6C;}
		else if (ratio <= LUM_TSL2561_LUX_K7C)
			{b=LUM_TSL2561_LUX_B7C; m=LUM_TSL2561_LUX_M7C;}
		else if (ratio > LUM_TSL2561_LUX_K8C)
			{b=LUM_TSL2561_LUX_B8C; m=LUM_TSL2561_LUX_M8C;}
		#else
		if ((ratio >= 0) && (ratio <= LUM_TSL2561_LUX_K1T))
			{b=LUM_TSL2561_LUX_B1T; m=LUM_TSL2561_LUX_M1T;}
		else if (ratio <= LUM_TSL2561_LUX_K2T)
			{b=LUM_TSL2561_LUX_B2T; m=LUM_TSL2561_LUX_M2T;}
		else if (ratio <= LUM_TSL2561_LUX_K3T)
			{b=LUM_TSL2561_LUX_B3T; m=LUM_TSL2561_LUX_M3T;}
		else if (ratio <= LUM_TSL2561_LUX_K4T)
			{b=LUM_TSL2561_LUX_B4T; m=LUM_TSL2561_LUX_M4T;}
		else if (ratio <= LUM_TSL2561_LUX_K5T)
			{b=LUM_TSL2561_LUX_B5T; m=LUM_TSL2561_LUX_M5T;}
		else if (ratio <= LUM_TSL2561_LUX_K6T)
			{b=LUM_TSL2561_LUX_B6T; m=LUM_TSL2561_LUX_M6T;}
		else if (ratio <= LUM_TSL2561_LUX_K7T)
			{b=LUM_TSL2561_LUX_B7T; m=LUM_TSL2561_LUX_M7T;}
		else if (ratio > LUM_TSL2561_LUX_K8T)
			{b=LUM_TSL2561_LUX_B8T; m=LUM_TSL2561_LUX_M8T;}
		#endif

		temp = ((channel0 * b) - (channel1 * m));

		/* do not allow negative lux value */
		if (temp < 0)
		{
			temp = 0;
		}

		/* round lsb (2^(LUX_SCALE-1)) */
		temp += (1 << (LUM_TSL2561_LUX_LUXSCALE-1));

		/* strip off fractional portion */
		lux = temp >> LUM_TSL2561_LUX_LUXSCALE;

		/* Signal I2C had no errors */
		/* put in struct */
		p_tsl2561->lux = lux;
		/* if needed, put in result pointer */
		UTILITIES_StoreInPointer(p_lux, lux);
	}
	return status;
}



/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the tsl2561 device.
 *
 * @note we will turn it on forever, and let it run forever... it consumes less than 0.5mA --> for my application this is not a big deal.
 * @param p_tsl2561 tsl2561 device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_Init(lum_tsl2561_t *p_tsl2561, lum_tsl2561_config_t *p_config)
{
	status_t status = status_ok;

	/* populate the struct */
	p_tsl2561->id 					= p_config->id;
	p_tsl2561->p_i2c 				= p_config->p_i2c;
	p_tsl2561->address 				= p_config->address;

	p_tsl2561->fullspectrum			= 0;
	p_tsl2561->infrared				= 0;
	p_tsl2561->lux					= 0;

	/* turn the sensor on */
	status = LUM_TSL2561_On(p_tsl2561);

	if(status == status_ok)
	{
		/* set the integration time */
		status = LUM_TSL2561_SetIntegrationTime(p_tsl2561, p_config->integration_time);
	}
	if(status == status_ok)
	{
		/* set the gain */
		status = LUM_TSL2561_SetGain(p_tsl2561, p_config->gain);
	}


	return status;
}


/**
 * Set the integrationtime.
 *
 * @note LUM_TSL2561_INTEGRATIONTIME_402MS is slower but more precise, LUM_TSL2561_INTEGRATIONTIME_13MS is very fast but not so precise
 * @param p_tsl2561 tsl2561 device
 * @param integrationtime LUM_TSL2561_INTEGRATIONTIME_402MS or LUM_TSL2561_INTEGRATIONTIME_101MS or LUM_TSL2561_INTEGRATIONTIME_13MS
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_SetIntegrationTime(lum_tsl2561_t *p_tsl2561, lum_tsl2561_integrationtime_t integrationtime)
{
	status_t status = status_ok;

	if(status == status_ok)
	{
		/* set integration time over i2c */
		status = lum_tsl2561_write8(p_tsl2561, LUM_TSL2561_COMMAND_BIT | LUM_TSL2561_REGISTER_TIMING, integrationtime | p_tsl2561->gain);
	}

	if(status == status_ok)
	{
		/* save in struct */
		p_tsl2561->integration_time = integrationtime;
	}
	return status;
}


/**
 * Set the gain.
 *
 * @note Use 16X gain to get more precision in dark ambients, or enable auto gain with LUM_TSL2561_SenseLight()
 * @param p_tsl2561
 * @param gain LUM_TSL2561_GAIN_1X or LUM_TSL2561_GAIN_16X
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_SetGain(lum_tsl2561_t *p_tsl2561, lum_tsl2561_gain_t gain)
{
	status_t status = status_ok;

	if(status == status_ok)
	{
		/* set gain over i2c */
		status = lum_tsl2561_write8(p_tsl2561, LUM_TSL2561_COMMAND_BIT | LUM_TSL2561_REGISTER_TIMING, p_tsl2561->integration_time | gain);
	}

	if(status == status_ok)
	{
		/* save in struct */
		p_tsl2561->gain = gain;
	}
	return status;
}



/**
 * Wake up the TSL2561 by setting the control bit.
 *
 * @param p_tsl2561 tsl2561 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_On(lum_tsl2561_t *p_tsl2561)
{
	status_t status = status_ok;
	status = lum_tsl2561_write8(p_tsl2561, LUM_TSL2561_COMMAND_BIT | LUM_TSL2561_REGISTER_CONTROL, LUM_TSL2561_CONTROL_POWERON);
	return status;
}


/**
 * Turn the TSL2561 in power saving mode by clearing the control bit.
 *
 * @param p_tsl2561 tsl2561 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_Off(lum_tsl2561_t *p_tsl2561)
{
	status_t status = status_ok;
	status = lum_tsl2561_write8(p_tsl2561, LUM_TSL2561_COMMAND_BIT | LUM_TSL2561_REGISTER_CONTROL, LUM_TSL2561_CONTROL_POWEROFF);
	return status;
}



/**
 * Sense the ambient light.
 *
 * This function will get the latest raw fullspectrum and infrared value over i2c, and calculate the LUX from it.
 * It also provides the posibility to try the autogain.
 *
 * @note the result will be saved in the device struct, result pointers added for convenience, they can be made NULL if not needed.
 * @param p_tsl2561 tsl2561 device
 * @param p_fullspectrum quantity og light at full spectrum (including infrared) (can be NULL if not needed)
 * @param p_infrared quantity of infrared light (can be NULL if not needed)
 * @param p_lux lux result (can be NULL if not needed)
 * @param autogain false: single sensor reading is done, with gain and integration time previously selected. true: automatic gain adjustment algorithm is used.
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t LUM_TSL2561_SenseLight(lum_tsl2561_t *p_tsl2561, uint16_t *p_fullspectrum, uint16_t *p_infrared, uint32_t *p_lux, bool autogain)
{
	status_t status = status_ok;
	uint16_t fs, ir, hi, lo;

	/* if no autogain is requested */
	if(!autogain)
	{
		/* execute a single sensor read */
		status = lum_tsl2561_getdata(p_tsl2561, NULL, NULL);
	}
	/* if autogain is requested */
	else
	{
		switch(p_tsl2561->integration_time)
		{
			case LUM_TSL2561_INTEGRATIONTIME_13MS:
				hi = LUM_TSL2561_AGC_THI_13MS;
				lo = LUM_TSL2561_AGC_TLO_13MS;
				break;
			case LUM_TSL2561_INTEGRATIONTIME_101MS:
				hi = LUM_TSL2561_AGC_THI_101MS;
				lo = LUM_TSL2561_AGC_TLO_101MS;
				break;
			case LUM_TSL2561_INTEGRATIONTIME_402MS:
				hi = LUM_TSL2561_AGC_THI_402MS;
				lo = LUM_TSL2561_AGC_TLO_402MS;
				break;
			default:
				hi = LUM_TSL2561_AGC_THI_402MS;
				lo = LUM_TSL2561_AGC_TLO_402MS;
				break;
		}

		/* Try to adjust the gain */
		/* first get data */
		status = lum_tsl2561_getdata(p_tsl2561, &fs, &ir);

		if(status == status_ok)
		{
			/* if the light is too low with this gain */
			if ((fs < lo) && (p_tsl2561->gain == LUM_TSL2561_GAIN_1X))
			{
				/* raise the gain and redo the reading */
				status = LUM_TSL2561_SetGain(p_tsl2561, LUM_TSL2561_GAIN_16X);

				if(status == status_ok)
				{
					status = lum_tsl2561_getdata(p_tsl2561, &fs, &ir);
				}
			}
			/* if the light is too high with this gain */
			else if ((fs < hi) && (p_tsl2561->gain == LUM_TSL2561_GAIN_16X))
			{
				/* lower the gain and redo the reading */
				status = LUM_TSL2561_SetGain(p_tsl2561, LUM_TSL2561_GAIN_1X);

				if(status == status_ok)
				{
					status = lum_tsl2561_getdata(p_tsl2561, &fs, &ir);
				}
			}
		}
	}

	/* for all above possibilities the raw fullspectrum and infrared values are saved inside the struct, now calculate the lux (which will automatically be saved in struct) */
	if(status == status_ok)
	{
		/* calculate the lux */
		status = lum_tsl2561_calculatelux(p_tsl2561, p_tsl2561->fullspectrum, p_tsl2561->infrared, NULL);

		/* put in result pointers if needed */
		UTILITIES_StoreInPointer(p_fullspectrum, p_tsl2561->fullspectrum);
		UTILITIES_StoreInPointer(p_infrared, p_tsl2561->infrared);
		UTILITIES_StoreInPointer(p_lux, p_tsl2561->lux);
	}
	return status;
}


/* End of file lum_tsl2561.c */
