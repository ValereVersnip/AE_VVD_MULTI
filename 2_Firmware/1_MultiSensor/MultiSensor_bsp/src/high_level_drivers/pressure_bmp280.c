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
 * @file pressure_bmp280.c
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
#include "pressure_bmp280.h"
#include "BMP280_driver/bmp280.h"

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

static pressure_bmp280_t *p_PressureBmp280;					/**< pointer to current bmp280 device, needed because of how bosch driver is built */

/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * I2C bus write for this presure bmp280 sensor.
 *
 * @note users should make sure the current p_PressureBmp280 pointer is setup correctly!
 * @param dev_addr 7bit I2C address of this sensor.
 * @param reg_addr Address of the first register, where data is going to be written
 * @param reg_data pointer to write data buffer
 * @param cnt number of bytes to write
 * @return SUCCESS if succeeded, ERROR if not
 */
s8 pressure_bmp280_i2c_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);


/**
 * I2C bus read for this presure bmp280 sensor.
 *
 * @note users should make sure the current p_PressureBmp280 pointer is setup correctly!
 * @param dev_addr 7bit I2C address of this sensor.
 * @param reg_addr Address of the first register, where data is going to be read
 * @param reg_data pointer to read data buffer
 * @param cnt number of bytes to read
 * @return SUCCESS if succeeded, ERROR if not
 */
s8 pressure_bmp280_i2c_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);


/**
 * The delay routine.
 * @param msek delay in ms
 */
void  pressure_bmp280_delay(u16 msek);

/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */


/**
 * I2C bus write for this presure bmp280 sensor.
 *
 * @note users should make sure the current p_PressureBmp280 pointer is setup correctly!
 * @param dev_addr 7bit I2C address of this sensor.
 * @param reg_addr Address of the first register, where data is going to be written
 * @param reg_data pointer to write data buffer
 * @param cnt number of bytes to write
 * @return SUCCESS if succeeded, ERROR if not
 */
s8 pressure_bmp280_i2c_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	s32 iError = BMP280_INIT_VALUE;
	status_t status = status_ok;
	uint8_t txbuf[PRESSURE_BMP280_I2C_BUFFER_LEN + 1];
	uint8_t i;
	/* copy the regadress to first space in txbuff */
	txbuf[0] = reg_addr;
	/* then copy write data to the rest */
	for(i = 0; i < cnt; i++)
	{
		txbuf[1 + i] = *(reg_data + i);
	}

	status = I2C_Transfer_Blocking(p_PressureBmp280->p_i2c, (uint8_t)dev_addr, txbuf, (uint8_t)(cnt+1), NULL,0);

	if(status == status_ok)
	{
		iError = SUCCESS;
	}
	else
	{
		iError = ERROR;
	}
	return (s8)iError;
}



/**
 * I2C bus read for this presure bmp280 sensor.
 *
 * @note users should make sure the current p_PressureBmp280 pointer is setup correctly!
 * @param dev_addr 7bit I2C address of this sensor.
 * @param reg_addr Address of the first register, where data is going to be read
 * @param reg_data pointer to read data buffer
 * @param cnt number of bytes to read
 * @return SUCCESS if succeeded, ERROR if not
 */
s8 pressure_bmp280_i2c_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	s32 iError = BMP280_INIT_VALUE;
	status_t status = status_ok;

	status = I2C_Transfer_Blocking(p_PressureBmp280->p_i2c, (uint8_t)dev_addr, (uint8_t*)(&reg_addr), 1,(uint8_t*)reg_data, (uint8_t)cnt);

	if(status == status_ok)
	{
		iError = SUCCESS;
	}
	else
	{
		iError = ERROR;
	}
	return (s8)iError;
}


/**
 * The delay routine.
 * @param msek delay in ms
 */
void  pressure_bmp280_delay(u16 msek)
{
	SYSTICK_DelayMs(msek);
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */


/**
 * Initialize the pressure_bmp280.
 *
 * @note We fix the following things in this function:
 * 	I2C mode, NORMAL mode,  BMP280_HIGH_RESOLUTION_MODE, 500MS standby mode.
 * @param p_pressurebmp280 pressure bmp280 device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PRESSURE_BMP280_Init(pressure_bmp280_t *p_pressurebmp280, pressure_bmp280_config_t *p_config)
{
	status_t status = status_ok;
	s32 com_rslt = ERROR;
	u8 testTime;



	/* populate the struct */
	p_pressurebmp280->id = p_config->id;
	p_pressurebmp280->p_i2c = p_config->p_i2c;
	p_pressurebmp280->address = p_config->address;

	/* i2c routine */
	p_pressurebmp280->bmp280.bus_write 	= pressure_bmp280_i2c_bus_write;
	p_pressurebmp280->bmp280.bus_read 	= pressure_bmp280_i2c_bus_read;
	p_pressurebmp280->bmp280.dev_addr 	= p_pressurebmp280->address;
	p_pressurebmp280->bmp280.delay_msec = pressure_bmp280_delay;

	/* reset results */
	p_pressurebmp280->uncomp_temp 		= 0;
	p_pressurebmp280->uncomp_press 		= 0;
	p_pressurebmp280->comp_temp 		= 0;
	p_pressurebmp280->comp_press 		= 0;

	/* set pointer to this pressure_bmp280_t */
	status = PRESSURE_BMP280_SetPointer(p_pressurebmp280);

	if(status == status_ok)
	{
		/* initialize the bmp280 */
		com_rslt = bmp280_init(&p_pressurebmp280->bmp280);

		//SYSTICK_DelayMs(500);

		/*	For initialization it is required to set the mode of
		 *	the sensor as "NORMAL"
		 *	data acquisition/read/write is possible in this mode
		 *	by using the below API able to set the power mode as NORMAL*/
		/* Set the power mode as NORMAL*/
		com_rslt += bmp280_set_power_mode(BMP280_NORMAL_MODE);

		/* needed otherwise unstable results */
		SYSTICK_DelayMs(500);


		/*	For reading the pressure and temperature data it is required to
		 *	set the work mode
		 *	The measurement period in the Normal mode is depends on the setting of
		 *	over sampling setting of pressure, temperature and standby time
		 *
		 *	OSS				pressure OSS	temperature OSS
		 *	ultra low power			x1			x1
		 *	low power				x2			x1
		 *	standard resolution		x4			x1
		 *	high resolution			x8			x2
		 *	ultra high resolution	x16			x2
		 */
		/* The oversampling settings are set by using the following API*/
		com_rslt += bmp280_set_work_mode(BMP280_HIGH_RESOLUTION_MODE);

		//SYSTICK_DelayMs(500);

		/* This API used to Write the standby time of the sensor input
		 *	value have to be given*/
		 /*	Normal mode comprises an automated perpetual cycling between an (active)
		 *	Measurement period and an (inactive) standby period.
		 *	The standby time is determined by the contents of the register t_sb.
		 *	Standby time can be set using BMP280_STANDBYTIME_125_MS.
		 *	Usage Hint : BMP280_set_standbydur(BMP280_STANDBYTIME_125_MS)*/

		com_rslt += bmp280_set_standby_durn(BMP280_STANDBY_TIME_250_MS);

		//SYSTICK_DelayMs(500);

		/* This API used to read back the written value of standby time*/
		com_rslt += bmp280_get_standby_durn(&testTime);

		/* check if we readback correctly */
		if(testTime != BMP280_STANDBY_TIME_250_MS)
		{
			com_rslt += -1;
		}

		/* check the result */
		if(com_rslt != 0)
		{
			status = pressure_bmp280_init;
		}

		/* needed otherwise unstable results */
		SYSTICK_DelayMs(250);
	}
	return status;
}


/**
 * Set the internal static pointer to the specified pressure bmp280 device.
 *
 * This function should be called prior to using any other function!
 * @param p_pressurebmp280 pressure bmp280 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PRESSURE_BMP280_SetPointer(pressure_bmp280_t *p_pressurebmp280)
{
	status_t status = status_ok;
	p_PressureBmp280 = p_pressurebmp280;
	return status;
}


/**
 * Get the latest temperature and pressure values over I2C.
 *
 * This function will get the latest conversion results over I2C from the BMP280.
 * @param p_pressurebmp280 pressure bmp280 device
 * @param p_uncomp_temp uncompensated temperature result (can be NULL if not needed)
 * @param p_uncomp_press uncompensated pressure result (can be NULL if not needed)
 * @param p_comp_temp compensated temperature result (can be NULL if not needed)
 * @param p_comp_press compensated temperature result (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t PRESSURE_BMP280_GetResult(pressure_bmp280_t *p_pressurebmp280, int32_t *p_uncomp_temp, int32_t *p_uncomp_press, int32_t *p_comp_temp, uint32_t *p_comp_press)
{
	status_t status = status_ok;
	s32 v_data_uncomp_tem_s32, v_data_uncomp_pres_s32, v_actual_temp_s32;
	u32 v_actual_press_u32;
	s32 com_rslt = ERROR;

	/* first set internal pointer */
	status = PRESSURE_BMP280_SetPointer(p_pressurebmp280);

	if(status == status_ok)
	{
		/* read uncompensated temp and press */
		com_rslt = bmp280_read_uncomp_pressure(&v_data_uncomp_pres_s32);
		com_rslt += bmp280_read_uncomp_temperature(&v_data_uncomp_tem_s32);

		com_rslt += bmp280_read_uncomp_pressure_temperature(&v_data_uncomp_pres_s32, &v_data_uncomp_tem_s32);
		/* read compensated temp and press */
		com_rslt += bmp280_read_pressure_temperature(&v_actual_press_u32, &v_actual_temp_s32);
	}

	if(com_rslt != SUCCESS)
	{
		status = pressure_bmp280_readout;
	}

	if(status == status_ok)
	{
		/* put results in struct */
		p_pressurebmp280->uncomp_temp 	= (int32_t)v_data_uncomp_tem_s32;
		p_pressurebmp280->uncomp_press 	= (int32_t)v_data_uncomp_pres_s32;
		p_pressurebmp280->comp_temp 	= (int32_t)v_actual_temp_s32;
		p_pressurebmp280->comp_press 	= (uint32_t)v_actual_press_u32;

		/* put in arguments if not NULL */
		UTILITIES_StoreInPointer(p_uncomp_temp, p_pressurebmp280->uncomp_temp);
		UTILITIES_StoreInPointer(p_uncomp_press, p_pressurebmp280->uncomp_press);
		UTILITIES_StoreInPointer(p_comp_temp, p_pressurebmp280->comp_temp);
		UTILITIES_StoreInPointer(p_comp_press, p_pressurebmp280->comp_press);

	}
	return status;
}

/* End of file pressure_bmp280.c */
