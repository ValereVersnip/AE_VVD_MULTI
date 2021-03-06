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
 * @file lowleveldriversconfig.h
 * @author TimB
 * @date 16-mei-2016
 * @brief	Configuration parameters for all lowleveldriver structs.
 *
 */

#ifndef DEF_lowleveldriversconfig_H
#define DEF_lowleveldriversconfig_H

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */

/*
 * SYSTICK low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_SYSTICK_RATE						1000							/**< systemtick counter increments every ms (1/1000) DO NOT CHANGE THIS!!!*/
//#define LOWLEVELDRIVERSCONFIG_SYSTICK_INTR_FUNCTION			HIGHLEVELDRIVERS_Heartbeat		/**< name of the void function(void) user function which will be called by systick timer, uncomment if used */

/* do not modify under this line */
/* ******************************************************************************* */
#ifdef LOWLEVELDRIVERSCONFIG_SYSTICK_INTR_FUNCTION
extern void LOWLEVELDRIVERSCONFIG_SYSTICK_INTR_FUNCTION(void);
#define LOWLEVELDRIVERSCONFIG_SYSTICK_INTRHANDLER			&LOWLEVELDRIVERSCONFIG_SYSTICK_INTR_FUNCTION		/**< Pointer to the high level Systick handler */
#else
#define LOWLEVELDRIVERSCONFIG_SYSTICK_INTRHANDLER			NULL
#endif
/* do not modify above this line */
/* ******************************************************************************* */


/*
 * MRT low level startup values
 * ***********************************************************
 */
/*
 * mrt0 low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_MRT_0_ENABLE					true							/**< this timer is enabled */
#define LOWLEVELDRIVERSCONFIG_MRT_0_ENABLEINTERRUPT			true							/**< interrupts enabled for this timer */
#define LOWLEVELDRIVERSCONFIG_MRT_0_INTERRUPTFREQ			20000000						/**< 20000000 --> 50uS (freqsetting in mHz!)*/
//#define LOWLEVELDRIVERSCONFIG_MRT_0_INTR_FUNCTION			HIGHLEVELDRIVERS_Test			/**< Define if higherlevel interrupt handler should be specified */

/* do not modify under this line */
/* ******************************************************************************* */
#ifdef LOWLEVELDRIVERSCONFIG_MRT_0_INTR_FUNCTION
extern void LOWLEVELDRIVERSCONFIG_MRT_0_INTR_FUNCTION(void);
#define LOWLEVELDRIVERSCONFIG_MRT_0_INTRHANDLER			&LOWLEVELDRIVERSCONFIG_MRT_0_INTR_FUNCTION		/**< Pointer to the high level TIMER16_0 handler */
#else
#define LOWLEVELDRIVERSCONFIG_MRT_0_INTRHANDLER			NULL
#endif
/* do not modify above this line */
/* ******************************************************************************* */

/*
 * mrt1 low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_MRT_1_ENABLE					true							/**< this timer is enabled */
#define LOWLEVELDRIVERSCONFIG_MRT_1_ENABLEINTERRUPT			true							/**< interrupts enabled for this timer */
#define LOWLEVELDRIVERSCONFIG_MRT_1_INTERRUPTFREQ			1000000							/**< 1000000 --> 1MS (freqsetting in mHz!)*/
#define LOWLEVELDRIVERSCONFIG_MRT_1_INTR_FUNCTION			LOWLEVELDRIVERS_MRT1_HANDLER	/**< Define if higherlevel interrupt handler should be specified */

/* do not modify under this line */
/* ******************************************************************************* */
#ifdef LOWLEVELDRIVERSCONFIG_MRT_1_INTR_FUNCTION
extern void LOWLEVELDRIVERSCONFIG_MRT_1_INTR_FUNCTION(void);
#define LOWLEVELDRIVERSCONFIG_MRT_1_INTRHANDLER			&LOWLEVELDRIVERSCONFIG_MRT_1_INTR_FUNCTION		/**< Pointer to the high level TIMER16_0 handler */
#else
#define LOWLEVELDRIVERSCONFIG_MRT_1_INTRHANDLER			NULL
#endif
/* do not modify above this line */
/* ******************************************************************************* */

/*
 * mrt2 low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_MRT_2_ENABLE					true							/**< this timer is enabled */
#define LOWLEVELDRIVERSCONFIG_MRT_2_ENABLEINTERRUPT			false							/**< interrupts enabled for this timer */
#define LOWLEVELDRIVERSCONFIG_MRT_2_INTERRUPTFREQ			150000000						/**< 200000000 --> 10uS (freqsetting in mHz!)*/
//#define LOWLEVELDRIVERSCONFIG_MRT_2_INTR_FUNCTION											/**< Define if higherlevel interrupt handler should be specified */

/* do not modify under this line */
/* ******************************************************************************* */
#ifdef LOWLEVELDRIVERSCONFIG_MRT_2_INTR_FUNCTION
extern void LOWLEVELDRIVERSCONFIG_MRT_2_INTR_FUNCTION(void);
#define LOWLEVELDRIVERSCONFIG_MRT_2_INTRHANDLER			&LOWLEVELDRIVERSCONFIG_MRT_2_INTR_FUNCTION		/**< Pointer to the high level TIMER16_0 handler */
#else
#define LOWLEVELDRIVERSCONFIG_MRT_2_INTRHANDLER			NULL
#endif
/* do not modify above this line */
/* ******************************************************************************* */

/*
 * mrt3 low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_MRT_3_ENABLE					false							/**< this timer is disabled */
#define LOWLEVELDRIVERSCONFIG_MRT_3_ENABLEINTERRUPT			false							/**< interrupts disabled for this timer */
#define LOWLEVELDRIVERSCONFIG_MRT_3_INTERRUPTFREQ			20000							/**< 20000 --> 50uS */
//#define LOWLEVELDRIVERSCONFIG_MRT_3_INTR_FUNCTION											/**< Define if higherlevel interrupt handler should be specified */

/* do not modify under this line */
/* ******************************************************************************* */
#ifdef LOWLEVELDRIVERSCONFIG_MRT_3_INTR_FUNCTION
extern void LOWLEVELDRIVERSCONFIG_MRT_3_INTR_FUNCTION(void);
#define LOWLEVELDRIVERSCONFIG_MRT_3_INTRHANDLER			&LOWLEVELDRIVERSCONFIG_MRT_3_INTR_FUNCTION		/**< Pointer to the high level TIMER16_0 handler */
#else
#define LOWLEVELDRIVERSCONFIG_MRT_3_INTRHANDLER			NULL
#endif
/* do not modify above this line */
/* ******************************************************************************* */


/*
 * GPIO low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_GPIO_PARAMS			{\
													{ .port = 0, .pin = 13, 	.dir = input,		.irq = edgelow,		.irqchannel = 0},\
													{ .port = 0, .pin = 4, 		.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 28, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 24,		.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 15,		.dir = input, 		.irq = edgehigh,	.irqchannel = 1},\
													{ .port = 0, .pin = 14, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 23, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 22, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 21, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 20, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 19, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 18, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 17, 	.dir = input, 		.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 0, 		.dir = outputlow,	.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 27, 	.dir = outputlow,	.irq = none, 		.irqchannel = 0},\
													{ .port = 0, .pin = 12, 	.dir = outputlow,	.irq = none, 		.irqchannel = 0},\
													}												/**< startup direction for gpio pins, and value if output */


/* Pininterrupt user functions */

#define LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_0			HIGHLEVELRIVERS_DHT22_Pin_Handler		/**< name of the void function(void) user function which will be called by PinInterrupt 0, uncomment if used */
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_1			HIGHLEVELRIVERS_PIR_Pin_Handler			/**< name of the void function(void) user function which will be called by PinInterrupt 1, uncomment if used */
//#define LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_2			test		/**< name of the void function(void) user function which will be called by PinInterrupt 2, uncomment if used */
//#define LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_3			test		/**< name of the void function(void) user function which will be called by PinInterrupt 3, uncomment if used */
//#define LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_4			test		/**< name of the void function(void) user function which will be called by PinInterrupt 4, uncomment if used */
//#define LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_5			test		/**< name of the void function(void) user function which will be called by PinInterrupt 5, uncomment if used */
//#define LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_6			test		/**< name of the void function(void) user function which will be called by PinInterrupt 6, uncomment if used */
//#define LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_7			test		/**< name of the void function(void) user function which will be called by PinInterrupt 7, uncomment if used */


/* do not modify under this line */
/* ******************************************************************************* */
#ifdef LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_0
extern void LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_0(void);
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_0			&LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_0		/**< Pointer to the high level PinInterrupt 0 handler */
#else
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_0			NULL
#endif
#ifdef LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_1
extern void LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_1(void);
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_1			&LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_1		/**< Pointer to the high level PinInterrupt 1 handler */
#else
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_1			NULL
#endif
#ifdef LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_2
extern void LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_2(void);
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_2			&LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_2		/**< Pointer to the high level PinInterrupt 2 handler */
#else
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_2			NULL
#endif
#ifdef LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_3
extern void LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_3(void);
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_3			&LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_3		/**< Pointer to the high level PinInterrupt 3 handler */
#else
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_3			NULL
#endif
#ifdef LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_4
extern void LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_4(void);
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_4			&LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_4		/**< Pointer to the high level PinInterrupt 4 handler */
#else
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_4			NULL
#endif
#ifdef LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_5
extern void LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_5(void);
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_5			&LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_5		/**< Pointer to the high level PinInterrupt 5 handler */
#else
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_5			NULL
#endif
#ifdef LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_6
extern void LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_6(void);
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_6			&LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_6		/**< Pointer to the high level PinInterrupt 6 handler */
#else
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_6			NULL
#endif
#ifdef LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_7
extern void LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_7(void);
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_7			&LOWLEVELDRIVERSCONFIG_GPIO_PININTR_FUNCTION_7		/**< Pointer to the high level PinInterrupt 7 handler */
#else
#define LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_7			NULL
#endif


#define LOWLEVELDRIVERSCONFIG_GPIO_INTERRUPTHANDLERS			{\
													LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_0,\
													LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_1,\
													LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_2,\
													LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_3,\
													LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_4,\
													LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_5,\
													LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_6,\
													LOWLEVELDRIVERSCONFIG_GPIO_PININTRHANDLER_7\
													}			/**< Pointers to the higherlevel pin interrupt routines, NULL if pininterrupt is not used */

/* do not modify above this line */
/* ******************************************************************************* */





/*
 * I2C low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_I2C0_BITRATE					400000									/**< 400kHz Bitrate */


/*
 * USART low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_USART0_USE															/**< uncomment if usart0 is used (modbus)*/
#define LOWLEVELDRIVERSCONFIG_USART1_USE															/**< uncomment if usart1 is used (secondary bootloader)*/
//#define LOWLEVELDRIVERSCONFIG_USART2_USE															/**< uncomment if usart2 is used */

#ifdef LOWLEVELDRIVERSCONFIG_USART0_USE
#define LOWLEVELDRIVERSCONFIG_USART0_BAUDRATE				115200									/**< BAUDRATE OF THE UART */
#define LOWLEVELDRIVERSCONFIG_USART0_DATABITS				8										/**< Amount of databits */
#define LOWLEVELDRIVERSCONFIG_USART0_PARITY					usart_parity_none						/**< no parity */
#define LOWLEVELDRIVERSCONFIG_USART0_STOPBITS				1										/**< one stop bits */
#define LOWLEVELDRIVERSCONFIG_USART0_HANDLER				NULL									/**< No handler needed */
#define LOWLEVELDRIVERSCONFIG_USART0_RX_BUFFERSIZE			64										/**< RX BUFFER SIZE */
#define LOWLEVELDRIVERSCONFIG_USART0_TX_BUFFERSIZE			64										/**< TX BUFFER SIZE */
#endif

#ifdef LOWLEVELDRIVERSCONFIG_USART1_USE
#define LOWLEVELDRIVERSCONFIG_USART1_BAUDRATE				115200									/**< BAUDRATE OF THE UART */
#define LOWLEVELDRIVERSCONFIG_USART1_DATABITS				8										/**< Amount of databits */
#define LOWLEVELDRIVERSCONFIG_USART1_PARITY					usart_parity_none						/**< no parity */
#define LOWLEVELDRIVERSCONFIG_USART1_STOPBITS				2										/**< two stop bits */
#define LOWLEVELDRIVERSCONFIG_USART1_HANDLER				NULL									/**< No handler needed */
#define LOWLEVELDRIVERSCONFIG_USART1_RX_BUFFERSIZE			128										/**< RX BUFFER SIZE */
#define LOWLEVELDRIVERSCONFIG_USART1_TX_BUFFERSIZE			64										/**< TX BUFFER SIZE */
#endif

#ifdef LOWLEVELDRIVERSCONFIG_USART2_USE
#define LOWLEVELDRIVERSCONFIG_USART2_BAUDRATE				115200									/**< BAUDRATE OF THE UART */
#define LOWLEVELDRIVERSCONFIG_USART2_DATABITS				8										/**< Amount of databits */
#define LOWLEVELDRIVERSCONFIG_USART2_PARITY					usart_parity_none						/**< no parity */
#define LOWLEVELDRIVERSCONFIG_USART2_STOPBITS				2										/**< two stop bits */
#define LOWLEVELDRIVERSCONFIG_USART2_HANDLER				NULL									/**< No handler needed */
#define LOWLEVELDRIVERSCONFIG_USART2_RX_BUFFERSIZE			128										/**< RX BUFFER SIZE */
#define LOWLEVELDRIVERSCONFIG_USART2_TX_BUFFERSIZE			64										/**< TX BUFFER SIZE */
#endif


/*
 * PWM low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_PWM_FREQUENCY	1000					/**< standard frequency for PWM  is 1kHz */
#define LOWLEVELDRIVERSCONFIG_PWM_DUTYCYCLE	50						/**< standard dutycycle for PWM  is 50% */
#define LOWLEVELDRIVERSCONFIG_PWM_USECHANNEL	{\
												false,\
												true,\
												true,\
												true,\
												false,\
												false,\
												}					/**< true: usechannel, false: don't use pwm channel */




/*
 * ADC low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_ADC0_USE			true				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC1_USE			true				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC2_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC3_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC4_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC5_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC6_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC7_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC8_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC9_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC10_USE			false				/**< true: use this adc channel, false: don't use */
#define LOWLEVELDRIVERSCONFIG_ADC11_USE			false				/**< true: use this adc channel, false: don't use */

/*
 * WDT low level startup values
 * ***********************************************************
 */
#define LOWLEVELDRIVERSCONFIG_WDT_TIMEOUT		2					/**< Watchdog timeout (in s) */

/*
 * ***********************************************************************************************************************************************
 * Typedefs and enumerations
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Public Function Prototypes
 * ***********************************************************************************************************************************************
 */



#endif
/* End of file lowleveldriversconfig.h */
