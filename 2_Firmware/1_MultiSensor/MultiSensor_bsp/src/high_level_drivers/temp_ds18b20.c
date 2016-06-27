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
 * @file temp_ds18b20.c
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
#include "temp_ds18b20.h"

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


/**
 * Do one step of the crc calculation.
 *
 * This function does one byte crc calculation using Polynomial: x^8 + x^5 + x^4 + 1 (0x8C).
 * @param crc current crc
 * @param data new databyte
 * @return crc
 */
static uint8_t temp_ds18b20_crc(uint8_t crc, uint8_t data)
{
	uint8_t i;
	crc = crc ^ data;
	for (i = 0; i < 8; i++)
	{
		if (crc & 0x01)
			crc = (crc >> 1) ^ 0x8C;
		else
			crc >>= 1;
	}
	return crc;
}



/**
 * Check the current raw data, in the databuffer for crc.
 *
 * @param p_temp temp_ds18b20 device
 * @param rom_ram	true: romcheck, false: scratchpad ram check
 * @return true: crc OK, false NOT OK
 */
static bool temp_ds18b20_checkdatacrc(temp_ds18b20_t *p_temp, bool rom_ram)
{
	bool checkcrc = false;
	uint8_t i, end;
	uint8_t crc;

	if(rom_ram)
	{
		end = 8;	/* if rom check, there should be 8 valid databytes in the buffer */
	}
	else
	{
		end = 9;	/* if ram check, there should be 9 valid databytes in the buffer */
	}

	/* first reset crc */
	crc = 0;
	for(i = 0; i < end; i++)
	{
		crc = temp_ds18b20_crc(crc, p_temp->data[i]);
	}

	/* crc should be zero here */
	if(!crc)
	{
		checkcrc = true;
	}
	return checkcrc;
}


/**
 * statemachine for temperature conversion.
 *
 * @note this should be called periodically by higher level routines
 * @param p_temp
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
static status_t temp_ds18b20_statemachine(temp_ds18b20_t *p_temp)
{
	status_t status = status_ok;
	bool bit;
	uint32_t currenttime;
	static uint8_t i;
	uint8_t crc;
	int16_t rawtemp;


	switch(p_temp->state)
	{
		/* idle state */
		case temp_ds18b20_state_idle:
			/* if we need to start a conversion */
			if(p_temp->start)
			{
				/* set the running flag */
				p_temp->running = true;
				/* reset done and start flag */
				p_temp->start = false;
				p_temp->ready = false;
				/* go to the send reset state */
				p_temp->state = temp_ds18b20_state_sendreset;
				/* also reset i and crc */
				i = 0;
				crc = 0;
			}
			break;

		/* send reset state */
		case temp_ds18b20_state_sendreset:
			/* send the reset command */
			status = ONEWIRE_reset(p_temp->p_onewire);
			/* go to the send conver t state*/
			p_temp->state = temp_ds18b20_state_sendskiprom;
			break;

		/* send skiprom command state */
		case temp_ds18b20_state_sendskiprom:
			/* send the skip rom command */
			status = ONEWIRE_writebyte(p_temp->p_onewire, TEMP_DS18B20_SKIP_ROM);		/* always ok */
			/* go to the send conver t state*/
			p_temp->state = temp_ds18b20_state_sendconvert;
			break;

		/* send convert t command state */
		case temp_ds18b20_state_sendconvert:
			/* send the convert t command */
			status = ONEWIRE_writebyte(p_temp->p_onewire, TEMP_DS18B20_CONVERT_T);		/* always ok */
			/* take a timestamp */
			SYSTICK_GetTicks(&p_temp->timestamp);
			/* go to the send wait for conversion state*/
			p_temp->state = temp_ds18b20_state_waitforconversion;
			break;

		/* wait for the conversion to finish */
		case temp_ds18b20_state_waitforconversion:
			/* wait for TEMP_DS18B20_CONVERSION time */
			/* get current time */
			SYSTICK_GetTicks(&currenttime);
			if(currenttime - p_temp->timestamp > TEMP_DS18B20_CONVERSION)
			{
				/* if conversion is ready, go to the wait for ok to readout state */
				p_temp->state = temp_ds18b20_state_waitforoktoreadout;
			}
			break;

		/* wait for OK to readout */
		case temp_ds18b20_state_waitforoktoreadout:
			/* if a higherlevel routine triggers ok to readout the scratchpad */
			if(p_temp->oktoreadout)
			{
				/* immediately reset the ok to readout flag */
				p_temp->oktoreadout = false;
				/* and go to the sendreset2 state, to start reading out the scratchpad */
				p_temp->state = temp_ds18b20_state_sendreset2;
			}
			break;

		/* send second reset state */
		case temp_ds18b20_state_sendreset2:
			/* send the reset command */
			status = ONEWIRE_reset(p_temp->p_onewire);
			/* go to the read send read scratchpad command state*/
			p_temp->state = temp_ds18b20_state_sendskiprom2;
			break;

		/* send second skiprom command state */
		case temp_ds18b20_state_sendskiprom2:
			/* send the skip rom command */
			status = ONEWIRE_writebyte(p_temp->p_onewire, TEMP_DS18B20_SKIP_ROM);		/* always ok */
			/* go to the send conver t state*/
			p_temp->state = temp_ds18b20_state_sendreadscratchpad;
			break;

		/* send the read scratchpad command */
		case temp_ds18b20_state_sendreadscratchpad:
			/* send the reset command */
			status = ONEWIRE_writebyte(p_temp->p_onewire, TEMP_DS18B20_READ_SCRATCHPAD);		/* always ok */
			/* go to the read scratchpad state*/
			p_temp->state = temp_ds18b20_state_readscratchpad;
			break;

		/* read the entire scratchpad */
		case temp_ds18b20_state_readscratchpad:
			/* there are 9 bytes to read */
			for(; i<9; )	/* i has been reset in the idle state, when we receive start command */
			{
				status = ONEWIRE_readbyte(p_temp->p_onewire, &p_temp->data[i]);	/* always ok */
				i++;
				break; /* break, so we go out of the loop */
			}
			/* if we have read 9 bytes, go to the crc  */
			if(i >= 9)
			{
				p_temp->state = temp_ds18b20_state_crc;
			}
			break;

		/* crc calcualtion */
		case temp_ds18b20_state_crc:
			/* check the crc */
			crc = 0;
			for(i = 0; i<9; i++)	/* i has been reset in the idle state, when we receive start command */
			{
				crc = temp_ds18b20_crc(crc, p_temp->data[i]);
			}
			/* crc should be zero */
			if(crc)
			{
				status = temp_ds18b20_ramcrc;
			}
			else
			{
				/* if crc ok, go to store state */
				p_temp->state = temp_ds18b20_state_store;
			}
			break;

		/* store the result */
		case temp_ds18b20_state_store:
			/* reset frac and dec parts */
			p_temp->temperature_int = 0;
			p_temp->temperature_frac = 0;
			/* get raw temp */
			rawtemp = 0;
			rawtemp |= p_temp->data[1];
			rawtemp <<= 8;
			rawtemp |= p_temp->data[0];

			/* if the result is negative */
			if(rawtemp & 0x8000)	/* if the MSB of the MSBYTE is 1, then the result is negative */
			{
				rawtemp = ~rawtemp + 1;
				p_temp->temperature_int = ~(((rawtemp >> 4) & 0x7F) - 1);
				p_temp->temperature_frac = ((rawtemp & 0x000F) * 625);
			}
			/* if the result is positive */
			else
			{
				p_temp->temperature_int = ((rawtemp >> 4) & 0x7F);
				p_temp->temperature_frac = ((rawtemp & 0x000F) * 625);
			}
			/* set the done flag, reset the running and start flag */
			p_temp->start = false;
			p_temp->running = false;
			p_temp->ready = true;
			/* go back to the idle state */
			p_temp->state = temp_ds18b20_state_idle;
			break;


		default:
			p_temp->state = temp_ds18b20_state;
			break;
	}

	/* if the status is not ok, reset everything and go to idle state */
	if(status != status_ok)
	{
		p_temp->running = false;
		p_temp->start = false;
		p_temp->ready = false;
		p_temp->state = temp_ds18b20_state_idle;
		/* and fill in 0xFFFF in temp register */
		p_temp->temperature_int = 0xFFFF;
		p_temp->temperature_frac = 0xFFFF;
	}

	/* ALWAYS CLEAR THE OK TO READOUT BIT, if we were not ready this time, too bad, wait for the next explicit oktoreadout */
	p_temp->oktoreadout = false;

	return status;
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the temp_ds18b20 device.
 *
 * @note we will get the devices unique 48bit serial code over onewire, and store it in the struct as well.
 * @param p_temp
 * @param p_config
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_Init(temp_ds18b20_t *p_temp, temp_ds18b20_config_t *p_config)
{
	status_t status = status_ok;

	/* populate the struct */
	p_temp->id 			= p_config->id;
	p_temp->p_onewire	= p_config->p_onewire;

	p_temp->temperature_int = 0;
	p_temp->temperature_frac = 0;
	p_temp->state = temp_ds18b20_state_idle;
	p_temp->oktoreadout = false;


	/* get the serialnumber over onewire, and store in struct */
	status = TEMP_DS18B20_GetSerialNumber(p_temp);

	return status;
}


/**
 * Get the serialnumber over onewire.
 *
 * @param p_temp temp_ds18b20 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_GetSerialNumber(temp_ds18b20_t *p_temp)
{
	status_t status = status_ok;
	uint8_t i;
	bool crccheck;

	/* First send onewire reset pulse */
	status = ONEWIRE_reset(p_temp->p_onewire);

	/* if succeeded (if we detect presence) */
	if(status == status_ok)
	{
		/* send the READ_ROM command */
		status = ONEWIRE_writebyte(p_temp->p_onewire, TEMP_DS18B20_READ_ROM);		/* always ok */
		/* then read back 8 values */
		for(i = 0; i < 8; i++)
		{
			status = ONEWIRE_readbyte(p_temp->p_onewire, &p_temp->data[i]);	/* always ok */
		}

		/* then do a crc check */
		crccheck = temp_ds18b20_checkdatacrc(p_temp, true);

		/* crccheck should be true */
		if(!crccheck)
		{
			status = temp_ds18b20_romcrc;
		}
	}

	if(status == status_ok)
	{
		/* if crc is ok, store the serialnumber in the struct */
		p_temp->serialnumber = 0;
		for(i = 6; i >= 1; i--)
		{
			p_temp->serialnumber = (p_temp->serialnumber << 8) | p_temp->data[i];
		}
	}

	/* and compensate the systick msTicks for lost interrupts. (above routine takes +/- 7ms) */
	SYSTICK_IncTicks(7);
	return status;
}



/**
 * Start a conversion cycle.
 *
 * @param p_temp temp_ds18b20 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_Start(temp_ds18b20_t *p_temp)
{
	status_t status = status_ok;
	/* check if it is not already running */
	if(p_temp->running)
	{
		status = temp_ds18b20_running;
	}
	else
	{
		p_temp->start = true;
	}
	return status;
}


/**
 * Get the last converted result.
 *
 * @note this function will immediately reset the ready flag
 * @param p_temp temp_ds18b20 device
 * @param p_temperature_int pointer to integer result (can be NULL if not needed)
 * @param p_temperature_frac pointer to fractional result (can be NULL if not needed)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_GetResult(temp_ds18b20_t *p_temp, int16_t *p_temperature_int, uint16_t *p_temperature_fraq)
{
	status_t status = status_ok;

	/* check if result is ready */
	if(p_temp->ready)
	{
		/* immediately reset the ready flag */
		p_temp->ready = false;
		/* and store in pointer if needed */
		UTILITIES_StoreInPointer(p_temperature_int, p_temp->temperature_int);
		UTILITIES_StoreInPointer(p_temperature_fraq, p_temp->temperature_frac);
	}
	/* not ready */
	else
	{
		status = temp_ds18b20_notready;
	}
	return status;
}


/**
 * Run0 function for the temp_ds18b20 device.
 *
 * @note this function should be called periodically by higherlevel routines
 * @param p_temp temp_ds18b20 device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t TEMP_DS18B20_Run0(temp_ds18b20_t *p_temp)
{
	status_t status = status_ok;

	status = temp_ds18b20_statemachine(p_temp);

	return status;

}


/**
 * Get the ready flag.
 *
 * @param p_temp temp ds18b20 device
 * @param p_ready pointer to result
 * @return status_ok
 */
status_t TEMP_DS18B20_GetReadyFlag(temp_ds18b20_t *p_temp, bool *p_ready)
{
	status_t status = status_ok;
	*p_ready = p_temp->ready;
	return status;
}


/**
 * Get the running flag.
 *
 * @param p_temp temp ds18b20 device
 * @param p_running pointer to result
 * @return status_ok
 */
status_t TEMP_DS18B20_GetRunningFlag(temp_ds18b20_t *p_temp, bool *p_running)
{
	status_t status = status_ok;
	*p_running = p_temp->running;
	return status;
}


/**
 * Signal the statemachine, that it is OK to start reading the scratchpad, so we can control when to do this in time.
 * @param p_temp temp ds18b20 device
 * @return status_ok
 */
status_t TEMP_DS18B20_OkToReadOut(temp_ds18b20_t *p_temp)
{
	status_t status = status_ok;
	p_temp->oktoreadout = true;
	return status;
}

/* End of file temp_ds18b20.c */
