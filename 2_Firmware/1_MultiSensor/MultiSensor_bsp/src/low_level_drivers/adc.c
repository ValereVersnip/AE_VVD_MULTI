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
 * @file adc.c
 * @author TimB
 * @date 18-mei-2016
 * @brief	Low level routines for adc access.
 *
 * adc initialization and low level routines.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "adc.h"
#include "lowleveldriversconfig.h"
#include "pwm.h"
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

extern pwm_t Pwm;

bool SeqAComplete;																/**< true: sequencer A finshed */


#if LOWLEVELDRIVERSCONFIG_ADC0_USE
static RINGBUFF_T AdcRingBuffer0;												/**< adc0 ringbuffer */
static uint16_t AdcBuff0[ADC_BUFFERSIZE];										/**< adc0 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC1_USE
static RINGBUFF_T AdcRingBuffer1;												/**< adc1 ringbuffer */
static uint16_t AdcBuff1[ADC_BUFFERSIZE];										/**< adc1 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC2_USE
static RINGBUFF_T AdcRingBuffer2;												/**< adc2 ringbuffer */
static uint16_t AdcBuff2[ADC_BUFFERSIZE];										/**< adc2 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC3_USE
static RINGBUFF_T AdcRingBuffer3;												/**< adc3 ringbuffer */
static uint16_t AdcBuff3[ADC_BUFFERSIZE];										/**< adc3 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC4_USE
static RINGBUFF_T AdcRingBuffer4;												/**< adc4 ringbuffer */
static uint16_t AdcBuff4[ADC_BUFFERSIZE];										/**< adc4 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC5_USE
static RINGBUFF_T AdcRingBuffer5;												/**< adc5 ringbuffer */
static uint16_t AdcBuff5[ADC_BUFFERSIZE];										/**< adc5 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC6_USE
static RINGBUFF_T AdcRingBuffer6;												/**< adc6 ringbuffer */
static uint16_t AdcBuff6[ADC_BUFFERSIZE];										/**< adc6 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC7_USE
static RINGBUFF_T AdcRingBuffer7;												/**< adc7 ringbuffer */
static uint16_t AdcBuff7[ADC_BUFFERSIZE];										/**< adc7 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC8_USE
static RINGBUFF_T AdcRingBuffer8;												/**< adc8 ringbuffer */
static uint16_t AdcBuff8[ADC_BUFFERSIZE];										/**< adc8 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC9_USE
static RINGBUFF_T AdcRingBuffer9;												/**< adc9 ringbuffer */
static uint16_t AdcBuff9[ADC_BUFFERSIZE];										/**< adc9 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC10_USE
static RINGBUFF_T AdcRingBuffer10;												/**< adc10 ringbuffer */
static uint16_t AdcBuff10[ADC_BUFFERSIZE];										/**< adc10 data buffer */
#endif

#if LOWLEVELDRIVERSCONFIG_ADC11_USE
static RINGBUFF_T AdcRingBuffer11;												/**< adc11 ringbuffer */
static uint16_t AdcBuff11[ADC_BUFFERSIZE];										/**< adc11 data buffer */
#endif




/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */

/**
 * Get the pointer to the adc channel ringbuffer.
 *
 * Returns NULL if the channel is not enabled!.
 * @param p_adc	adc device
 * @param channel channel (0-11)
 * @return	pinter to ringbuffer, or NULL if channel is not enabled, or not found.
 */
RINGBUFF_T* adc_getringbuffer(adc_t *p_adc, uint8_t channel);

/**
 * Setup the used ringbuffers.
 */
void adc_setupringbuffers();

/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */

/**
 * ADC Sequency A interrupt handler.
 *
 * When seq A finishes, this will just set a flag indicating we have to read out the enabled adc values in the run routine.
 */
void ADC_SEQA_IRQHandler(void)
{
	uint32_t pending;
	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK)
	{
		SeqAComplete = true;
	}

	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC, pending);
}

/**
 * Get the pointer to the adc channel ringbuffer.
 *
 * Returns NULL if the channel is not enabled!.
 * @param p_adc	adc device
 * @param channel channel (0-11)
 * @return	pinter to ringbuffer, or NULL if channel is not enabled, or not found.
 */
RINGBUFF_T* adc_getringbuffer(adc_t *p_adc, uint8_t channel)
{
	RINGBUFF_T *p_ringbuffer;

	/* get ringbuffer */
	switch(channel)
	{
#if LOWLEVELDRIVERSCONFIG_ADC0_USE
	case 0:
		p_ringbuffer = &AdcRingBuffer0;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC1_USE
	case 1:
		p_ringbuffer = &AdcRingBuffer1;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC2_USE
	case 2:
		p_ringbuffer = &AdcRingBuffer2;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC3_USE
	case 3:
		p_ringbuffer = &AdcRingBuffer3;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC4_USE
	case 4:
		p_ringbuffer = &AdcRingBuffer4;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC5_USE
	case 5:
		p_ringbuffer = &AdcRingBuffer5;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC6_USE
	case 6:
		p_ringbuffer = &AdcRingBuffer6;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC7_USE
	case 7:
		p_ringbuffer = &AdcRingBuffer7;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC8_USE
	case 8:
		p_ringbuffer = &AdcRingBuffer8;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC9_USE
	case 9:
		p_ringbuffer = &AdcRingBuffer9;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC10_USE
	case 10:
		p_ringbuffer = &AdcRingBuffer10;
		break;
#endif
#if LOWLEVELDRIVERSCONFIG_ADC11_USE
	case 11:
		ingbuffer = &AdcRingBuffer11;
		break;
#endif
	default:
		p_ringbuffer = NULL;
		break;
	}
	return p_ringbuffer;
}

/**
 * Setup the used ringbuffers.
 */
void adc_setupringbuffers()
{
#if LOWLEVELDRIVERSCONFIG_ADC0_USE
	RingBuffer_Init(&AdcRingBuffer0, AdcBuff0, 2, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC1_USE
	RingBuffer_Init(&AdcRingBuffer1, AdcBuff1, 2, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC2_USE
	RingBuffer_Init(&AdcRingBuffer2, AdcBuff2, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC3_USE
	RingBuffer_Init(&AdcRingBuffer3, AdcBuff3, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC4_USE
	RingBuffer_Init(&AdcRingBuffer4, AdcBuff4, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC5_USE
	RingBuffer_Init(&AdcRingBuffer5, AdcBuff5, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC6_USE
	RingBuffer_Init(&AdcRingBuffer6, AdcBuff6, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC7_USE
	RingBuffer_Init(&AdcRingBuffer7, AdcBuff7, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC8_USE
	RingBuffer_Init(&AdcRingBuffer8, AdcBuff8, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC9_USE
	RingBuffer_Init(&AdcRingBuffer9, AdcBuff9, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC10_USE
	RingBuffer_Init(&AdcRingBuffer10, AdcBuff10, 1, ADC_BUFFERSIZE);
#endif
#if LOWLEVELDRIVERSCONFIG_ADC11_USE
	RingBuffer_Init(&AdcRingBuffer11, AdcBuff11, 1, ADC_BUFFERSIZE);
#endif
}


/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */

/**
 * Initialize the ADC.
 *
 * For enabled channels:
 * 	ringbuffers will be initialized
 * 	they will all be put in sequencer A (for our purpose, 1 sequencer is enough)
 * 	We don't use tresholds, we only give interrupts when sequencer A is finished.
 * @param p_adc adc device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADC_Init(adc_t *p_adc, adc_config_t *p_config)
{
	status_t status = status_ok;
	uint8_t i;
	uint16_t channelmask;
	/* reset seqA finished bool */
	SeqAComplete = false;
	/* reset seq A running bool */
	p_adc->seqA_running = false;
	/* populate struct */
	for(i = 0; i < ADC_CHANNELAMOUNT; i++)
	{
		p_adc->overrun[i]		= false;
		p_adc->usechannel[i]	= p_config->usechannel[i];
		/* only if the channel is used, set the pointer, otherwise set null */
		p_adc->p_ringbuffer[i] = adc_getringbuffer(p_adc, i);
	}

	/* setup the ringbuffers */
	adc_setupringbuffers();

	/* Setup ADC for 12-bit mode and normal power */
	Chip_ADC_Init(LPC_ADC, 0);

	/* Need to do a calibration after initialization and trim */
	Chip_ADC_StartCalibration(LPC_ADC);
	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC))) {}

	/* Setup for maximum ADC clock rate using sycnchronous clocking */
	Chip_ADC_SetClockRate(LPC_ADC, ADC_MAX_SAMPLE_RATE);


	/* create channel mask */
	channelmask = 0;
	for(i = 0; i < ADC_CHANNELAMOUNT; i++)
	{
		channelmask = (channelmask >> 1) | ((p_adc->usechannel[i]) ? 0x0800 : 0x0000);
	}

	/* put only the enabled channels in sequence A, and set for EOS interrupt mode */
	Chip_ADC_SetupSequencer(LPC_ADC, ADC_SEQA_IDX,	(channelmask | ADC_SEQ_CTRL_MODE_EOS));

	/* Clear all pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC, Chip_ADC_GetFlags(LPC_ADC));

//	/* Enable ADC overrun and sequence A completion interrupts */
//	Chip_ADC_EnableInt(LPC_ADC, (ADC_INTEN_SEQA_ENABLE | ADC_INTEN_OVRRUN_ENABLE));

	/* Enable sequence A completion interrupts */
	Chip_ADC_EnableInt(LPC_ADC, (ADC_INTEN_SEQA_ENABLE));

	/* Enable ADC NVIC interrupt */
	NVIC_EnableIRQ(ADC_SEQA_IRQn);

	/* Enable sequencer */
	Chip_ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);

	return status;
}


/**
 * Run function for ADC.
 *
 * Check if SeqA is finished.
 * Ifso: check the overrun status, and set in struct.
 * Push new data to relevant ringbuffer.
 *
 * @note This function should be called periodically by higher level routines.
 * @param p_adc adc device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADC_Run0(adc_t *p_adc)
{
	status_t status = status_ok;
	uint8_t i;
	uint32_t rawsample;
	uint16_t data;
	RINGBUFF_T *p_ringbuffer;
	uint32_t datamv;
	/* check if the seqA finished */
	if(SeqAComplete)
	{
		p_adc->seqA_running = false;
		SeqAComplete = false;
		for(i = 0; i < ADC_CHANNELAMOUNT; i++)
		{
			/* only if the channel is enabled */
			if(p_adc->usechannel[i])
			{
				/* get the raw sample for this enabled channel */
				rawsample = Chip_ADC_GetDataReg(LPC_ADC, i);

				/* if data is not valid raise error and break */
				bool datavalid;
				datavalid = rawsample & ADC_SEQ_GDAT_DATAVALID;
				if(!datavalid)
				{
					status = adc_datanotvalid;
					break;
				}

				/* get overrun */
				p_adc->overrun[i] = (rawsample & ADC_DR_OVERRUN) ? "true" : "false";

//				if(p_adc->overrun[i])
//				{
//					while(1);
//				}

				/* push data to ringbuffer */
				data =  ADC_DR_RESULT(rawsample);

				/* convert the data to mV */
				datamv = (uint32_t)(3300 * data);
				datamv /= 0xFFF;
				data = (uint16_t)datamv;

				p_ringbuffer = adc_getringbuffer(p_adc, i);
				RingBuffer_Insert(p_ringbuffer, &data);
			}
		}
	}
	return status;
}


/**
 * Start Sequence A ADC conversion.
 *
 * This function is best called from a timer interrupt routine.
 * It will start a seq A adc conversion.
 * But it will first check if no previous conversion is running.
 * @param p_adc adc device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t ADC_StartSequenceA(adc_t *p_adc)
{
	status_t status = status_ok;
	/* check if the seqA is not running */
	if(p_adc->seqA_running)
	{
		status = adc_seqArunning;
	}
	if(status == status_ok)
	{
		p_adc->seqA_running = true;
		Chip_ADC_StartSequencer(LPC_ADC, ADC_SEQA_IDX);
	}
	return status;
}


/* End of file adc.c */
