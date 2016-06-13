 /*             #
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
 * @file usart.c
 * @author TimB
 * @date 25-apr.-2016
 * @brief	Low level routines for usart access.
 *
 * Usart initialization and low level routines.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */

#include "usart.h"
#include "lowleveldriversconfig.h"

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

#ifdef LOWLEVELDRIVERSCONFIG_USART0_USE
static RINGBUFF_T UsartTransmitBuf0;										/**< usart0 transmit ringbuffer */
static RINGBUFF_T UsartReceiveBuf0;											/**< usart0 receive ringbuffer */
static uint8_t rxbuff0[LOWLEVELDRIVERSCONFIG_USART0_RX_BUFFERSIZE];			/**< usart0 receive buffer */
static uint8_t txbuff0[LOWLEVELDRIVERSCONFIG_USART0_TX_BUFFERSIZE]; 		/**< usart0 transmit buffer */
#endif

#ifdef LOWLEVELDRIVERSCONFIG_USART1_USE
static RINGBUFF_T UsartTransmitBuf1;										/**< usart1 transmit ringbuffer */
static RINGBUFF_T UsartReceiveBuf1;											/**< usart1 receive ringbuffer */
static uint8_t rxbuff1[LOWLEVELDRIVERSCONFIG_USART1_RX_BUFFERSIZE];			/**< usart1 receive buffer */
static uint8_t txbuff1[LOWLEVELDRIVERSCONFIG_USART1_TX_BUFFERSIZE]; 		/**< usart1 transmit buffer */
#endif

#ifdef LOWLEVELDRIVERSCONFIG_USART2_USE
static RINGBUFF_T UsartTransmitBuf2;										/**< usart2 transmit ringbuffer */
static RINGBUFF_T UsartReceiveBuf2;											/**< usart2 receive ringbuffer */
static uint8_t rxbuff2[LOWLEVELDRIVERSCONFIG_USART2_RX_BUFFERSIZE];			/**< usart2 receive buffer */
static uint8_t txbuff2[LOWLEVELDRIVERSCONFIG_USART2_TX_BUFFERSIZE]; 		/**< usart2 transmit buffer */
#endif



/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * get the usart base.
 *
 * @param usart usart name
 * @return usart base
 */
static LPC_USART_T* usart_getbase(usart_name_t usart);

/**
 * Get the pointers to the ringbuffers for the specified usart.
 *
 * @param usart usartname
 * @param pp_UsartReceiveBuf pointer to receive buffer
 * @param pp_UsartTransmitBuf pointer to transmitbuffer
 */
static void usart_getringbuffer(usart_name_t usart, RINGBUFF_T **pp_UsartReceiveBuf, RINGBUFF_T **pp_UsartTransmitBuf);


/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * get the usart base.
 *
 * @param usart usart name
 * @return usart base
 */
static LPC_USART_T* usart_getbase(usart_name_t usart)
{
	switch(usart)
	{
	case usart_0:
		return LPC_USART0;
		break;
	case usart_1:
		return LPC_USART1;
		break;
	case usart_2:
		return LPC_USART2;
		break;
	default:
		return NULL;
		break;
	}
}

/**
 * Get the pointers to the ringbuffers for the specified usart.
 *
 * @param usart usartname
 * @param pp_UsartReceiveBuf pointer to receive buffer
 * @param pp_UsartTransmitBuf pointer to transmitbuffer
 */
static void usart_getringbuffer(usart_name_t usart, RINGBUFF_T **pp_UsartReceiveBuf, RINGBUFF_T **pp_UsartTransmitBuf)
{
	switch(usart)
	{
#ifdef LOWLEVELDRIVERSCONFIG_USART0_USE
	case usart_0:
		*pp_UsartReceiveBuf = &UsartReceiveBuf0;
		*pp_UsartTransmitBuf = &UsartTransmitBuf0;
		break;
#endif
#ifdef LOWLEVELDRIVERSCONFIG_USART1_USE
	case usart_1:
		*pp_UsartReceiveBuf = &UsartReceiveBuf1;
		*pp_UsartTransmitBuf = &UsartTransmitBuf1;
		break;
#endif
#ifdef LOWLEVELDRIVERSCONFIG_USART2_USE
	case usart_2:
		*pp_UsartReceiveBuf = &UsartReceiveBuf2;
		*pp_UsartTransmitBuf = &UsartTransmitBuf2;
		break;
#endif
	default:
		break;
	}
}

/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

#ifdef LOWLEVELDRIVERSCONFIG_USART0_USE
/**
 * @brief	UART0 interrupt handler using ring buffers
 * @return	Nothing
 */
void UART0_IRQHandler(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_USART0, &UsartReceiveBuf0, &UsartTransmitBuf0);
}
#endif

#ifdef LOWLEVELDRIVERSCONFIG_USART1_USE
/**
 * @brief	UART1 interrupt handler using ring buffers
 * @return	Nothing
 */
void UART1_IRQHandler(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_USART1, &UsartReceiveBuf1, &UsartTransmitBuf1);
}
#endif

#ifdef LOWLEVELDRIVERSCONFIG_USART2_USE
/**
 * @brief	UART2 interrupt handler using ring buffers
 * @return	Nothing
 */
void UART2_IRQHandler(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_USART2, &UsartReceiveBuf2, &UsartTransmitBuf2);
}
#endif

/**
 * Initialize the USART.
 *
 * @param p_usartConfig config data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_Init(usart_config_t *p_usartConfig)
{
	status_t status = status_ok;
	uint32_t configMask;
	uint8_t i;

	if(p_usartConfig->enable[0] || p_usartConfig->enable[1] || p_usartConfig->enable[2])	/* if one of the usarts should be enabled */
	{
		Chip_Clock_SetUSARTNBaseClockRate((115200 * 16), true);	/* then enable and set the usart base clock --> we can use a maximum baud of 115200 */
	}

	for(i = 0; i < usart_size; i++)
	{
		if(status == status_ok)
		{
			if(p_usartConfig->enable[i])	/* if we want to enable the usart */
			{
				LPC_USART_T* p_usartbase = usart_getbase((usart_name_t)i);

				Chip_UART_Init(p_usartbase);

				/* create the config mask */
				configMask = 0;
				switch(p_usartConfig->databits[i])
				{
				case 7:
					configMask |= UART_CFG_DATALEN_7;
					break;
				case 8:
					configMask |= UART_CFG_DATALEN_8;
					break;
				case 9:
					configMask |= UART_CFG_DATALEN_9;
					break;
				default:
					status = usart_databits;
				}

				if(status == status_ok)
				{

					switch(p_usartConfig->parity[i])
					{
					case usart_parity_none:
						configMask |= UART_CFG_PARITY_NONE;
						break;
					case usart_parity_even:
						configMask |= UART_CFG_PARITY_EVEN;
						break;
					case usart_parity_odd:
						configMask |= UART_CFG_PARITY_ODD;
						break;
					default:
						break;
					}

					switch(p_usartConfig->stopbits[i])
					{
					case 1:
						configMask |= UART_CFG_STOPLEN_1;
						break;
					case 2:
						configMask |= UART_CFG_STOPLEN_2;
						break;
					default:
						status = usart_stopbits;
						break;
					}
				}

				if(status == status_ok)
				{
					Chip_UART_ConfigData(p_usartbase, configMask);

					if(p_usartConfig->baud[i] > 115200)	/* check if the baudrate is lower than 115200 */
					{
						status = usart_baudrate;
					}
				}


				if(status == status_ok)
				{
					Chip_UART_SetBaud(p_usartbase, p_usartConfig->baud[i]);	/* set the  baudrate */
					Chip_UART_Enable(p_usartbase);
					Chip_UART_TXEnable(p_usartbase);


					/* initialize the ringbuffers */
					switch((usart_name_t) i)
					{
#ifdef LOWLEVELDRIVERSCONFIG_USART0_USE
					case usart_0:
						RingBuffer_Init(&UsartReceiveBuf0, rxbuff0, 1, LOWLEVELDRIVERSCONFIG_USART0_RX_BUFFERSIZE);
						RingBuffer_Init(&UsartTransmitBuf0, txbuff0, 1, LOWLEVELDRIVERSCONFIG_USART0_TX_BUFFERSIZE);
						break;
#endif
#ifdef LOWLEVELDRIVERSCONFIG_USART1_USE
					case usart_1:
						RingBuffer_Init(&UsartReceiveBuf1, rxbuff1, 1, LOWLEVELDRIVERSCONFIG_USART1_RX_BUFFERSIZE);
						RingBuffer_Init(&UsartTransmitBuf1, txbuff1, 1, LOWLEVELDRIVERSCONFIG_USART1_TX_BUFFERSIZE);
						break;
#endif
#ifdef LOWLEVELDRIVERSCONFIG_USART2_USE
					case usart_2:
						RingBuffer_Init(&UsartReceiveBuf2, rxbuff2, 1, LOWLEVELDRIVERSCONFIG_USART2_RX_BUFFERSIZE);
						RingBuffer_Init(&UsartTransmitBuf2, txbuff2, 1, LOWLEVELDRIVERSCONFIG_USART2_TX_BUFFERSIZE);
						break;
#endif
					default:
						break;
					}

					/* Enable receive data and line status interrupt */
					Chip_UART_IntEnable(p_usartbase, UART_INTEN_RXRDY);
					Chip_UART_IntDisable(p_usartbase, UART_INTEN_TXRDY);	/* May not be needed */

					/* preemption = 1, sub-priority = 1 */
					NVIC_EnableIRQ(UART0_IRQn + i);

				}
			}
		}
	}
	return status;
}


/**
 * DeInitialize the USART.
 *
 * @param p_usartConfig config data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_DeInit()
{
	uint8_t i;
	for(i = 0; i < usart_size; i++)
	{
		LPC_USART_T* p_usartbase = usart_getbase((usart_name_t)i);
		/* DeInitialize UART peripheral */
		NVIC_DisableIRQ(UART0_IRQn + i);
		Chip_UART_DeInit(p_usartbase);
	}
	return status_ok;
}

/**
 * Check if the receive ringbuffer has data.
 *
 * @param usart usartname
 * @param p_available pointer to result. False: RX buffer is empty, True: data available
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_RxDataAvailable(usart_name_t usart, bool *p_available)
{
	status_t status = status_ok;
	RINGBUFF_T *p_rxBuf;
	RINGBUFF_T *p_txBuf;

	usart_getringbuffer(usart, &p_rxBuf, &p_txBuf);
	*p_available = !RingBuffer_IsEmpty(p_rxBuf);
	return status;
}

/**
 * Flush the USART rx ringbuffer.
 *
 * @param usart usartname
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_RxFlush(usart_name_t usart)
{
	RINGBUFF_T *p_rxBuf;
	RINGBUFF_T *p_txBuf;

	usart_getringbuffer(usart, &p_rxBuf, &p_txBuf);
	RingBuffer_Flush(p_rxBuf);
	return status_ok;
}


/**
 * Increment the usart rx ringbuffer with skips amount, do nothing with popped data.
 *
 * @param usart usartname
 * @param skips amount of characters to skip
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_IncrementRxBufPointer(usart_name_t usart, uint32_t skips)
{
	status_t status = status_ok;
	uint8_t temp[skips];
	RINGBUFF_T *p_rxBuf;
	RINGBUFF_T *p_txBuf;

	usart_getringbuffer(usart, &p_rxBuf, &p_txBuf);
	RingBuffer_PopMult(p_rxBuf, temp, skips);
	return status;
}



/**
 * this method will handle the receiving of data through uart.
 *
 * @param usart usartname
 * @param p_receiveData a pointer to the buffer where you want the data to be stored
 * @param length the amount of bytes you want to read
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_Receive(usart_name_t usart, uint8_t* p_receiveData, uint8_t length)
{
	status_t status = status_ok;
	RINGBUFF_T *p_rxBuf;
	RINGBUFF_T *p_txBuf;
	LPC_USART_T* p_usartbase = usart_getbase(usart);

	usart_getringbuffer(usart, &p_rxBuf, &p_txBuf);

	uint8_t readLength =0;
	if(0 != length) /* if there is something to receive */
	{
		readLength = Chip_UART_ReadRB(p_usartbase, p_rxBuf, p_receiveData, length);
		if(readLength != length)
		{
			status = usart_lengthMisMatch;
		}
	}
	return status;
}

/**
 * Get the amount of received items in the uart buffer.
 *
 * @param p_RxCount pointer to result
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_RxCount(usart_name_t usart, uint32_t *p_RxCount)
{
	status_t status = status_ok;
	RINGBUFF_T *p_rxBuf;
	RINGBUFF_T *p_txBuf;
	usart_getringbuffer(usart, &p_rxBuf, &p_txBuf);

	*p_RxCount = RingBuffer_GetCount(p_rxBuf);
	return status;
}



/**
 * this method will handle sending through uart.
 *
 * @param usart usartname
 * @param p_sendData pointer to the data you want to send
 * @param length the maximum amount of data to send
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_Send(usart_name_t usart, uint8_t* p_sendData, uint8_t length)
{
	status_t status = status_ok;
	RINGBUFF_T *p_rxBuf;
	RINGBUFF_T *p_txBuf;
	LPC_USART_T* p_usartbase = usart_getbase(usart);

	usart_getringbuffer(usart, &p_rxBuf, &p_txBuf);

	if(0 != length)	/* if there is something to send */
	{
		if(status == status_ok)
		{
			uint32_t sendBytes = Chip_UART_SendRB(p_usartbase, p_txBuf, p_sendData, length);
			if(sendBytes != length)
			{
				status = usart_txoverflow;
			}
		}
	}
	return status;
}


/**
 * this method will handle sending through uart, in a blocking manner.
 *
 * @param usart usartname
 * @param p_sendData pointer to the data you want to send
 * @param length the maximum amount of data to send
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_SendBlocking(usart_name_t usart, uint8_t* p_sendData, uint8_t length)
{
	status_t status = status_ok;
	RINGBUFF_T *p_rxBuf;
	RINGBUFF_T *p_txBuf;
	usart_getringbuffer(usart, &p_rxBuf, &p_txBuf);

	status = USART_Send(usart, p_sendData, length);

	if(status == status_ok)
	{
		/* wait until tx buffer is empty */
		while(!RingBuffer_IsEmpty(p_txBuf));
	}

	return status;
}


/**
 * Enable or disable all rx interrupts.
 * @param enable true: enable, false disable
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t USART_EnableInterrupt(bool enable)
{
	status_t stat = status_ok;
	uint8_t i;

	for(i = 0; i < usart_size; i++)
	{
		LPC_USART_T* p_usartbase = usart_getbase((usart_name_t)i);
		if(enable)
		{
			Chip_UART_IntEnable(p_usartbase, UART_INTEN_RXRDY);
		}
		else
		{
			Chip_UART_IntDisable(p_usartbase, UART_INTEN_RXRDY);
		}
	}
	return stat;
}


/* End of file usart.c */
