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
 * @file modbusslave.c
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
#include "modbusslave.h"

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
 * update the modbus slave. receive packages
 * @param p_modbusslave modbus slave
 * @return errorcount
 */
unsigned int modbusslave_update(modbusslave_t *p_modbusslave);

/**
 * give an exception response
 * @param p_modbusslave modbusslave device
 * @param exception exception to report
 */
void modbusslave_exceptionResponse(modbusslave_t *p_modbusslave, unsigned char exception);


/**
 * calculate crc
 * @param p_modbusslave modbus slave device
 * @param bufferSize buffersize
 * @return crc
 */
unsigned int modbusslave_calculatecrc(modbusslave_t *p_modbusslave, unsigned char bufferSize);


/**
 * Send packet.
 * @param p_modbusslave modbusslave device
 * @param bufferSize buffersize
 */
void modbusslave_sendpacket(modbusslave_t *p_modbusslave, unsigned char bufferSize);

/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */


/**
 * update the modbus slave. receive packages
 * @param p_modbusslave modbus slave
 * @return errorcount
 */
unsigned int modbusslave_update(modbusslave_t *p_modbusslave)
{
	bool dataAvailable;
	/* Check if there is uart data available */
	USART_RxDataAvailable(MODBUSSLAVE_USART_USED, &dataAvailable);
	/* is there something to check? */
	if (dataAvailable)
	{
		unsigned char buffer = 0;
		unsigned char overflow = 0;

		while (dataAvailable)
		{
			// If more bytes is received than the BUFFER_SIZE the overflow flag will be set and the
			// serial buffer will be red untill all the data is cleared from the receive buffer.
			if (overflow)
			{
				USART_RxFlush(MODBUSSLAVE_USART_USED);
			}
			else
			{
				if (buffer == MODBUSSLAVE_FRAMEBUFFER_SIZE)
				{
					  overflow = 1;
				}

				uint32_t rxAmount;
				USART_RxCount(MODBUSSLAVE_USART_USED, &rxAmount);	/* get the amount of data available in the rx buffer */
				uint8_t rxData[rxAmount];

				status_t status = USART_Receive(MODBUSSLAVE_USART_USED, &rxData[0], rxAmount);/* for now, if not ok trap here */
				if(status != status_ok)
				{
					while(1);
				}

				uint8_t i;
				for(i = 0; i < rxAmount; i++)
				{
					p_modbusslave->frame[buffer] = rxData[i];
					buffer++;
				}

			}
			// This is not 100% correct but it will suffice.
			// worst case scenario is if more than one character time expires
			// while reading from the buffer then the buffer is most likely empty
			// If there are more bytes after such a delay it is not supposed to
			// be received and thus will force a frame_error.
			MRT_Delay(MODBUSSLAVE_TIMER_USED, p_modbusslave->T1_5 / MODBUSSLAVE_TIMER_PRESCALER);			/* todo: maybe change this to non blocking... */

			/* check if there still is rx data available */
			USART_RxDataAvailable(MODBUSSLAVE_USART_USED, &dataAvailable);
		}

		// If an overflow occurred increment the errorCount
		// variable and return to the main sketch without
		// responding to the request i.e. force a timeout
		if (overflow)
		{
			p_modbusslave->errorCount++;
			return p_modbusslave->errorCount;
		}


		// The minimum request packet is 8 bytes for function 3 & 16
		if (buffer > 7)
		{
			unsigned char id = p_modbusslave->frame[0];
			p_modbusslave->broadcastFlag = 0;

			if (id == 0)
			{
				p_modbusslave->broadcastFlag = 1;
			}


			if (id == p_modbusslave->slaveID || p_modbusslave->broadcastFlag) // if the recieved ID matches the slaveID or broadcasting id (0), continue
			{
				unsigned int crc = ((p_modbusslave->frame[buffer - 2] << 8) | p_modbusslave->frame[buffer - 1]); // combine the crc Low & High bytes
				if (modbusslave_calculatecrc(p_modbusslave, buffer - 2) == crc) // if the calculated crc matches the recieved crc continue
				{
					p_modbusslave->function = p_modbusslave->frame[1];
					unsigned int startingAddress = ((p_modbusslave->frame[2] << 8) | p_modbusslave->frame[3]); // combine the starting address bytes
					unsigned int no_of_registers = ((p_modbusslave->frame[4] << 8) | p_modbusslave->frame[5]); // combine the number of register bytes
					unsigned int maxData = startingAddress + no_of_registers;
					unsigned char index;
					unsigned char address;
					unsigned int crc16;

					// broadcasting is not supported for function 3
					if (!p_modbusslave->broadcastFlag && (p_modbusslave->function == 3))
					{
						if (startingAddress < modbus_reg_size) // check exception 2 ILLEGAL DATA ADDRESS
						{
							if (maxData <= modbus_reg_size) // check exception 3 ILLEGAL DATA VALUE
							{
								unsigned char noOfBytes = no_of_registers * 2;
								// ID, function, noOfBytes, (dataLo + dataHi)*number of registers,
								//  crcLo, crcHi
								unsigned char responseFrameSize = 5 + noOfBytes;
								p_modbusslave->frame[0] = p_modbusslave->slaveID;
								p_modbusslave->frame[1] = p_modbusslave->function;
								p_modbusslave->frame[2] = noOfBytes;
								address = 3; // PDU starts at the 4th byte
								unsigned int temp;

								for (index = startingAddress; index < maxData; index++)
								{
									temp = p_modbusslave->register_array[index];
									p_modbusslave->frame[address] = temp >> 8; // split the register into 2 bytes
									address++;
									p_modbusslave->frame[address] = temp & 0xFF;
									address++;
								}

								crc16 = modbusslave_calculatecrc(p_modbusslave, responseFrameSize - 2);
								p_modbusslave->frame[responseFrameSize - 2] = crc16 >> 8; // split crc into 2 bytes
								p_modbusslave->frame[responseFrameSize - 1] = crc16 & 0xFF;
								modbusslave_sendpacket(p_modbusslave, responseFrameSize);
							}
							else
							{
								  modbusslave_exceptionResponse(p_modbusslave, 3); // exception 3 ILLEGAL DATA VALUE
							}
						}
						else
						{
							modbusslave_exceptionResponse(p_modbusslave, 2); // exception 2 ILLEGAL DATA ADDRESS
						}
					}
					else if (p_modbusslave->function == 6)
					{
						if (startingAddress < modbus_reg_size) // check exception 2 ILLEGAL DATA ADDRESS
						{
							p_modbusslave->register_array[startingAddress] = ((p_modbusslave->frame[4] << 8) | p_modbusslave->frame[5]);// the 4th and 5th elements in frame is the 16 bit data value

							// only the first 6 bytes are used for CRC calculation
							crc16 = modbusslave_calculatecrc(p_modbusslave, 6);
							p_modbusslave->frame[6] = crc16 >> 8; // split crc into 2 bytes
							p_modbusslave->frame[7] = crc16 & 0xFF;

							// a function 16 response is an echo of the first 6 bytes from
							// the request + 2 crc bytes
							if (!p_modbusslave->broadcastFlag) // don't respond if it's a broadcast message
							{
								modbusslave_sendpacket(p_modbusslave, 8);
							}
						}
						else
						{
							modbusslave_exceptionResponse(p_modbusslave, 2); // exception 2 ILLEGAL DATA ADDRESS
						}
					}
					else if (p_modbusslave->function == 16)
					{
						// Check if the recieved number of bytes matches the calculated bytes
						// minus the request bytes.
						// id + function + (2 * address bytes) + (2 * no of register bytes) +
						// byte count + (2 * CRC bytes) = 9 bytes
						if (p_modbusslave->frame[6] == (buffer - 9))
						{
							if (startingAddress < modbus_reg_size) // check exception 2 ILLEGAL DATA ADDRESS
							{
								if (maxData <= modbus_reg_size) // check exception 3 ILLEGAL DATA VALUE
								{
									address = 7; // start at the 8th byte in the frame

									for (index = startingAddress; index < maxData; index++)
									{
										p_modbusslave->register_array[index] = ((p_modbusslave->frame[address] << 8) | p_modbusslave->frame[address + 1]);
										address += 2;
									}

									// only the first 6 bytes are used for CRC calculation
									crc16 = modbusslave_calculatecrc(p_modbusslave, 6);
									p_modbusslave->frame[6] = crc16 >> 8; // split crc into 2 bytes
									p_modbusslave->frame[7] = crc16 & 0xFF;

									// a function 16 response is an echo of the first 6 bytes from
									// the request + 2 crc bytes
									if (!p_modbusslave->broadcastFlag) // don't respond if it's a broadcast message
									{
										modbusslave_sendpacket(p_modbusslave, 8);
									}
								}
								else
								{
									  modbusslave_exceptionResponse(p_modbusslave, 3); // exception 3 ILLEGAL DATA VALUE
								}
							}
							else
							{
								modbusslave_exceptionResponse(p_modbusslave, 2); // exception 2 ILLEGAL DATA ADDRESS
							}
						}
						else
						{
							p_modbusslave->errorCount++; // corrupted packet
						}
					}
					else
					{
						modbusslave_exceptionResponse(p_modbusslave, 1);	// exception 1 ILLEGAL FUNCTION
					}
				}
				else // checksum failed
				{
					p_modbusslave->errorCount++;
				}
			} // incorrect id
		}
		else if (buffer > 0 && buffer < 8)
		{
			p_modbusslave->errorCount++; // corrupted packet
		}
	}
	return p_modbusslave->errorCount;
}


/**
 * give an exception response
 * @param p_modbusslave modbusslave device
 * @param exception exception to report
 */
void modbusslave_exceptionResponse(modbusslave_t *p_modbusslave, unsigned char exception)
{
	// each call to exceptionResponse() will increment the errorCount
	p_modbusslave->errorCount++;
	if (!p_modbusslave->broadcastFlag) // don't respond if its a broadcast message
	{
		p_modbusslave->frame[0] = p_modbusslave->slaveID;
		p_modbusslave->frame[1] = (p_modbusslave->function | 0x80); // set MSB bit high, informs the master of an exception
		p_modbusslave->frame[2] = exception;
		unsigned int crc16 = modbusslave_calculatecrc(p_modbusslave, 3); // ID, function|0x80, exception code
		p_modbusslave->frame[3] = crc16 >> 8;
		p_modbusslave->frame[4] = crc16 & 0xFF;
		// exception response is always 5 bytes
		// ID, function + 0x80, exception code, 2 bytes crc
		modbusslave_sendpacket(p_modbusslave, 5);
	}
}


/**
 * calculate crc
 * @param p_modbusslave modbus slave device
 * @param bufferSize buffersize
 * @return crc
 */
unsigned int modbusslave_calculatecrc(modbusslave_t *p_modbusslave, unsigned char bufferSize)
{
	unsigned int temp, temp2, flag;
	unsigned char i,j;
	temp = 0xFFFF;
	for (i = 0; i < bufferSize; i++)
	{
		temp = temp ^ p_modbusslave->frame[i];
		for (j = 1; j <= 8; j++)
		{
			flag = temp & 0x0001;
			temp >>= 1;
			if (flag)
			{
				temp ^= 0xA001;
			}
		}
	}
	// Reverse byte order.
	temp2 = temp >> 8;
	temp = (temp << 8) | temp2;
	temp &= 0xFFFF;
	// the returned value is already swapped
	// crcLo byte is first & crcHi byte is last
	return temp;
}


/**
 * Send packet.
 * @param p_modbusslave modbusslave device
 * @param bufferSize buffersize
 */
void modbusslave_sendpacket(modbusslave_t *p_modbusslave, unsigned char bufferSize)
{
	/* first enable the data enable pin */
	GPIO_Set_Pin(p_modbusslave->TxEnablePort, p_modbusslave->TxEnablePin, true);

	/* then send the frame */
	status_t status = USART_SendBlocking(MODBUSSLAVE_USART_USED, p_modbusslave->frame, bufferSize);	/* todo, maybe change this to non blocking... */

	/* increment the rx uart buffer with buffersize, because the nRE pin of the rs485 transceiver is always active, so we have to ignore data we are sending */
	USART_IncrementRxBufPointer(MODBUSSLAVE_USART_USED, (uint32_t)bufferSize);


	if(status != status_ok)
	{
		//printf("Tx Overflow!\r\n");
		while(1);		/* this means we have an overflow --> should never get here */
	}

	/* frame delay */
	MRT_Delay(MODBUSSLAVE_TIMER_USED, p_modbusslave->T3_5 / MODBUSSLAVE_TIMER_PRESCALER);			/* todo: maybe change this to non blocking... */

	/* disable the data enable pin */
	GPIO_Set_Pin(p_modbusslave->TxEnablePort, p_modbusslave->TxEnablePin, false);

	/* set the just responded flag here */
	p_modbusslave->justresponded = true;
}

/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */



/**
 * Modbusslave Init
 * @param p_modbusslave modbusslave device
 * @param p_config configuration data
 */
void MODBUSSLAVE_Init(modbusslave_t *p_modbusslave, modbusslave_config_t *p_config)
{
	p_modbusslave->slaveID		= p_config->slaveID;

	// Modbus states that a baud rate higher than 19200 must use a fixed 750 us
	// for inter character time out and 1.75 ms for a frame delay for baud rates
	// below 19200 the timing is more critical and has to be calculated.
	// E.g. 9600 baud in a 10 bit packet is 960 characters per second
	// In milliseconds this will be 960characters per 1000ms. So for 1 character
	// 1000ms/960characters is 1.04167ms per character and finally modbus states
	// an inter-character must be 1.5T or 1.5 times longer than a character. Thus
	// 1.5T = 1.04167ms * 1.5 = 1.5625ms. A frame delay is 3.5T.

	if (p_config->baud > 19200)
	{
		p_modbusslave->T1_5 = 750;
		p_modbusslave->T3_5 = 1750;
	}
	else
	{
		p_modbusslave->T1_5 = 15000000/p_config->baud; // 1T * 1.5 = T1.5
		p_modbusslave->T3_5 = 35000000/p_config->baud; // 1T * 3.5 = T3.5
	}

	p_modbusslave->TxEnablePort	= p_config->TxEnablePort;
	p_modbusslave->TxEnablePin	= p_config->TxEnablePin;
	/* set the tx enable pin low */
	GPIO_Set_Pin(p_modbusslave->TxEnablePort, p_modbusslave->TxEnablePin, false);
	p_modbusslave->errorCount = 0; // initialize errorCount

	/* reset the justresponded flag */
	p_modbusslave->justresponded = false;
}



/**
 * Modbusslave Run function
 * @param p_modbusslave modbusslave device
 * @return errorcount
 */
int MODBUSSLAVE_Run0(modbusslave_t *p_modbusslave)
{
	return modbusslave_update(p_modbusslave);
}




/**
 * Get modbus slave register.
 * @param p_modbusslave modbus slave device
 * @param reg register
 * @return uint16 data
 */
uint16_t MODBUSSLAVE_GetRegister(modbusslave_t *p_modbusslave, uint8_t reg)
{
	return p_modbusslave->register_array[reg];
}

/**
 * Set modbus slave register
 * @param p_modbusslave modbus slave device
 * @param reg register
 * @param data data to set
 */
void MODBUSSLAVE_SetRegister(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t data)
{
	p_modbusslave->register_array[reg] = data;
}



/**
 * Get the pointer to the modbusslave registerarray.
 * @param p_modbusslave modbusslave device
 * @return pointer to modbusslave registerarray.
 */
modbusslave_regarray_t *MODBUSSLAVE_GetRegisterArray(modbusslave_t *p_modbusslave)
{
	return &p_modbusslave->register_array;
}


/**
 * Get the justresponded flag.
 *
 * @note this will immediately reset the justresponded flag in the modbus device struct.
 * @param p_modbusslave modbus device
 * @return true: just responded, false: not just responded
 */
bool MODBUSSLAVE_GetSetJustResponded(modbusslave_t *p_modbusslave)
{
	bool justresponded;
	justresponded = p_modbusslave->justresponded;
	p_modbusslave->justresponded = false;
	return justresponded;
}


/**
 * Set the bits specified in mask, in the specified register.
 *
 * @param p_modbusslave modbusslave device
 * @param reg register
 * @param mask mask
 */
void MODBUSSLAVE_DiscreteSet(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t mask)
{
	uint16_t readvalue;
	readvalue = MODBUSSLAVE_GetRegister(p_modbusslave, reg);
	MODBUSSLAVE_SetRegister(p_modbusslave, reg, readvalue | mask);
}


/**
 * Clear the bits specified in mask, in the specified register.
 *
 * @param p_modbusslave modbusslave device
 * @param reg register
 * @param mask mask
 */
void MODBUSSLAVE_DiscreteClear(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t mask)
{
	uint16_t readvalue;
	readvalue = MODBUSSLAVE_GetRegister(p_modbusslave, reg);
	MODBUSSLAVE_SetRegister(p_modbusslave, reg, readvalue & ~mask);
}


/**
 * Write a field in the modbus register.
 *
 * @note this function will check if the value will fit in the amount of mask spaces, and if there even is a mask.
 * @param p_modbusslave modbusslave device
 * @param reg register
 * @param fieldmask fielsmask (EG: 0x0F70)
 * @param fieldvalue value (eg: 15);
 * @return true: if succeeded, false: if somehting is wrong
 */
bool MODBUSSLAVE_WriteField(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t fieldmask, uint16_t fieldvalue)
{
	uint8_t i,j;
	bool check = false;
	uint16_t tempmask;
	uint16_t readvalue;

	/* first find where the LSB of the fieldmask is */
	for(i = 0; i < 16;i++)
	{
		if((fieldmask >> i) & 0x0001)
		{
			break;
		}
	}

	/* then find how wide the fieldmask is */
	tempmask = (fieldmask >> i);
	for(j = 0; j < 16;j++)
	{
		if(!((tempmask >> j) & 0x0001))
		{
			break;
		}
	}

	/* i = lsb of fieldmask, j = width of fieldmask*/

	if(i < 16)
	{
		/* clear (wrong) upperbits of tempmask */
		uint8_t k;
		tempmask = 0;
		for(k = 0; k < j; k++)
		{
			tempmask = (tempmask << k) | 0x0001;
		}

		/* if we found a fieldmask, check if the value will fit */
		if(fieldvalue <= tempmask)
		{
			/* set check to true */
			check = true;
			/* and fill in the fieldvalue */
			readvalue = MODBUSSLAVE_GetRegister(p_modbusslave, reg);
			readvalue &= ~fieldmask;
			readvalue |= (fieldvalue << i);
			MODBUSSLAVE_SetRegister(p_modbusslave, reg, readvalue);
		}
	}
	return check;
}



/**
 * Read a field in the modbus register.
 *
 * @param p_modbusslave modbusslave device
 * @param reg register
 * @param fieldmask fieldmask (eg: 0x0F70)
 * @return the field value (if no valid mask --> 0)
 */
uint16_t MODBUSSLAVE_ReadField(modbusslave_t *p_modbusslave, uint8_t reg, uint16_t fieldmask)
{
	uint8_t i;
	uint16_t fieldvalue = 0;

	/* first find where the LSB of the fieldmask is */
	for(i = 0; i < 16;i++)
	{
		if(fieldmask & 0x0001)
		{
			break;
		}
	}

	if(i < 16)
	{
		fieldvalue = MODBUSSLAVE_GetRegister(p_modbusslave, reg);
		fieldvalue = (fieldvalue & fieldmask) >> i;
	}
	return fieldvalue;
}



/* End of file modbusslave.c */
