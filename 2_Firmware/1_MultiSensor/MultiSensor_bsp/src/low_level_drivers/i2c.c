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
 * @file i2c.c
 * @author TimB
 * @date 16-mei-2016
 * @brief	low level i2c routines.
 *
 * low level i2c routines, and initialization.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "i2c.h"
#include "string.h"

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
 * get the i2c base.
 *
 * @param i2c i2c name
 * @return i2c base
 */
static LPC_I2C_T* i2c_getbase(i2c_name_t i2c);

/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * get the i2c base.
 *
 * @param i2c i2c name
 * @return i2c base
 */
static LPC_I2C_T* i2c_getbase(i2c_name_t i2c)
{
	switch(i2c)
	{
	case i2c_0:
		return LPC_I2C0;
		break;
	case i2c_1:
		return LPC_I2C1;
		break;
	case i2c_2:
		return LPC_I2C2;
		break;
	case i2c_3:
		return LPC_I2C3;
		break;
	default:
		return NULL;
		break;
	}
}

/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the I2C controller.
 *
 * @param p_i2c i2c device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t I2C_Init(i2c_t *p_i2c, i2c_config_t *p_config)
{
	status_t status = status_ok;
	LPC_I2C_T *p_i2cBase;

	p_i2c->i2c = p_config->i2c;
	p_i2c->bitrate = p_config->bitrate;

	p_i2cBase = i2c_getbase(p_config->i2c);
	if(p_i2cBase == NULL)
	{
		status = i2c_wrongbase;
	}

	if(status == status_ok)
	{
		/* Enable I2C clock and reset I2C peripheral - the boot ROM does not  do this */
		Chip_I2C_Init(p_i2cBase);

		/* Perform a sanity check on the storage allocation */
		if (LPC_I2CD_API->i2c_get_mem_size() > sizeof(p_i2c->HandleMemory))
		{
			/* Example only: this should never happen and probably isn't needed for
			   most I2C code. */
			status = i2c_sanityerror;
		}
	}


	if(status == status_ok)
	{
		/* Setup the I2C handle */
		p_i2c->i2cHandleMaster = LPC_I2CD_API->i2c_setup((uint32_t)p_i2cBase, p_i2c->HandleMemory);
		if (p_i2c->i2cHandleMaster == NULL)
		{
			status = i2c_handlesetup;
		}
	}

	if(status == status_ok)
	{
		/* Set I2C bitrate */
		if(LPC_I2CD_API->i2c_set_bitrate(p_i2c->i2cHandleMaster, Chip_Clock_GetSystemClockRate(),  p_i2c->bitrate) != LPC_OK)
		{
			status = i2c_bitrate;
		}
	}

	return status;
}




/**
 * Transmit and Receive data in master mode over specified i2c bus (Polling).
 *
 * This function write and/or read over the specified bus.
 *
 * @param p_i2cDevice pointer to selected i2c bus
 * @param addr slaveaddress (7 LSB) MSB is ignored
 * @param p_txData pointer to send databuffer, NULL if only receive
 * @param txLen amount of bytes to send, 0 if only receive
 * @param p_rxData pointer to receive databuffer, NULL if only send
 * @param rxLen amount of bytes to receive, 0 if only send
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t I2C_Transfer_Blocking(i2c_t *p_i2cDevice, uint8_t addr, uint8_t *p_txData, uint32_t txLen, uint8_t *p_rxData, uint32_t rxLen)
{
	status_t status = status_ok;
	ErrorCode_t errorcode;
	I2C_PARAM_T param;
	I2C_RESULT_T result;
	uint32_t index = 0;


	/* check if the buffer is big enough */
	if(txLen + 1 + rxLen + 1 > I2C_BUFFER_SIZE)
	{
		status = i2c_overflow;
	}


	if(status == status_ok)
	{
		uint8_t temp;
		param.stop_flag         = 1;									/* set the stop flag */
		param.num_bytes_send    = 0;									/* reset send length */
		param.num_bytes_rec    	= 0;									/* reset receive length */
		param.buffer_ptr_send   = &temp;								/* tx data buffer reset */
		param.buffer_ptr_rec	= &temp;								/* rx data buffer reset */

		/* first send, if needed */
		if(txLen > 0)
		{
			/* create the send address with write bit*/
			p_i2cDevice->buffer[index++] = (addr << 1) & 0xFE;			/* shift left, and clear last bit (write bit) */
			/* move data to buffer */
			memcpy(&p_i2cDevice->buffer[index], p_txData, txLen);
			index += txLen;												/* increment the buffer with txlength, so we are now pointing to first free  byte, where we will begin the rxbuffer, if needed */

			/* here set the params for the I2C ROM API */
			param.num_bytes_send    = txLen + 1;						/* tx length + 1 for the address byte (with write bit) */
			param.buffer_ptr_send   = &p_i2cDevice->buffer[0];			/* tx data is allways at the beginning of the buffer */
		}
		/* then read, if needed */
		if(rxLen > 0)
		{
			/* create the receive address with read bit */
			p_i2cDevice->buffer[index] = (addr << 1) | 0x01;			/* shift left, and set the last bit (read bit) */

			/* here set the params for the I2C ROM API */
			param.num_bytes_rec     = rxLen + 1;							/* rx length, apparaently we don't need to add 1 for the addressbyte: refer to AN11249 NXP. */
			param.buffer_ptr_rec	= &p_i2cDevice->buffer[index];		/* rx data begins at the first free byte in the buffer */
		}

		/* Set timeout (much) greater than the transfer length */
		LPC_I2CD_API->i2c_set_timeout(p_i2cDevice->i2cHandleMaster, 100000);


		if(rxLen > 0)/* Do master write/read transfer */
		{
			errorcode = LPC_I2CD_API->i2c_master_tx_rx_poll(p_i2cDevice->i2cHandleMaster, &param, &result);
		}
		else	/* only trasmit */
		{
			errorcode = LPC_I2CD_API->i2c_master_transmit_poll(p_i2cDevice->i2cHandleMaster, &param, &result);
		}
		if(errorcode != LPC_OK)
		{
			status = i2c_transfer;
		}
	}

	if(status == status_ok)
	{
		/* if we are receiving, copy data to p_rxData */
		if(rxLen > 0)
		{
			memcpy(p_rxData, &p_i2cDevice->buffer[index + 1], rxLen);
		}
	}
	return status;
}

/* End of file i2c.c */
