/*----------------------------------------------------------------------------
 * Name:    CanDemo.c
 * Purpose: CAN example for STM32F407
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stm32f4xx.h>
#include "Serial.h"
#include "CAN.h"
#include "LED.h"
#include "stm32f4xx_hal.h"
#include "init.h"


void wlacz3hz(void);
void wlacz1p5hz(void);
void wylacz(void);



unsigned int val_Tx = 0, val_Rx = 0;              /* Globals used for display */
volatile uint32_t msTicks;                        /* counts 1ms timeTicks     */
unsigned int accZ=0;
unsigned int accY=0;




/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

#ifndef __NO_SYSTICK
  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
#else
  for (curTicks = 0; curTicks < (dlyTicks * 0x1000); curTicks++) __NOP();
#endif
}


/*----------------------------------------------------------------------------
  display transmit and receive values
 *---------------------------------------------------------------------------*/
void val_display (void) {

  LED_Out (val_Rx);                               /* display RX val on LEDs  */
  printf ("Tx: 0x%02X, Rx: 0x%02X\r\n", val_Tx, val_Rx); // send out Printf Viewer
  Delay (10);                                     /* delay for 10ms           */
}

/* System Clock Configuration */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


/*----------------------------------------------------------------------------
  initialize CAN interface
 *----------------------------------------------------------------------------*/
void can_Init (void) {

  CAN_setup (1);                                  /* setup CAN Controller #1  */
#ifndef __TEST
  CAN_setup (2);                                  /* setup CAN Controller #2  */
#endif
  CAN_wrFilter (1, 33, STANDARD_FORMAT);          /* Enable reception of msgs */

#ifdef __TEST
  CAN_testmode(1, CAN_BTR_SILM | CAN_BTR_LBKM);   // Loopback, Silent Mode (self-test)
#endif

  CAN_start (1);                                  /* start CAN Controller #1  */
#ifndef __TEST
  CAN_start (2);                                  /* start CAN Controller #2  */
#endif

  CAN_waitReady (1);                              /* wait til tx mbx is empty */
#ifndef __TEST
  CAN_waitReady (2);                              /* wait til tx mbx is empty */
#endif
}



/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)  {
//  int i;
uint16_t pobrana=5, wyslana=5, x, y, z;
	int s;
	
	
SystemClock_Config();
  LED_Init ();                                    /* initialize the LEDs      */


  SystemCoreClockUpdate();                        /* Get Core Clock Frequency */
#ifndef __NO_SYSTICK
  SysTick_Config(SystemCoreClock /1000);         /* SysTick 1 msec irq       */
#endif
  can_Init ();                                    /* initialize CAN interface */


  
 	start_prog();
 	//PA5-SPI1_SCK, PA6-SPI1_MISO, PA7-SPI1_MOSI, PA1,2,3,4-NSS, PA9-dioda, PE3 - Acc_cs

	CAN_TxMsg[1].id = 0x00000000;                           /* initialize msg to send  tu wpisujemy id wiadomosci (0 dla 3 hz, 1 dla 1,5 hz 0 dla wylacz) */
  //for (i = 0; i < 8; i++) CAN_TxMsg[0].data[i] = 0;
  CAN_TxMsg[1].len = 8;
  CAN_TxMsg[1].format = EXTENDED_FORMAT;
	CAN_TxMsg[1].type = DATA_FRAME;
	
	
		
		
		
	while (1) {
		
		/*dla ustawionej czestotliwosci probkowania 1,6kHz, ponizszy algorytm w osi wykrywa wszystkie punkniecia w blat biurka 
	(przyspieszenie w osi Z), dla 25 Hz probkowania, nie wykrywa zadnego, mozna wiec przyjac ze probkowanie ponizej 25hz bedzie eliminowac rejestrowanie przyspieszen trwajacych krocej niz 1/25s??*/
	
		pobrana=SPI_send(0x8F00); //test who_am_i
		//if(pobrana==0xFF3b) GPIOD->ODR |= 1<<15;
		pobrana=SPI_send(0x2014); //ustawienie czestotliwosc probkowania i wlaczenie zapis	
		
		//pobrana=SPI_send(0xA000);
		//if(pobrana==0xFFC7) GPIOD->ODR |= 1<<14; //test who_am_i
		//for(uint16_t przer=0; przer<256; przer++);
		//x=SPI_send(0xA900);
		//y=SPI_send(0xAB00);
		z=SPI_send(0xAD00);
		y=SPI_send(0xAC00);
		//if(x>0xFFF0) GPIOD->ODR |= 1<<15;
		//else GPIOD->ODR &= ~(1<<14);
		//if(y>0xFFF0) GPIOD->ODR |= 1<<13;
		//else GPIOD->ODR &= ~(1<<13);
		//if(z=0xFFFF) GPIOD->ODR |= 1<<15;
		//else GPIOD->ODR &= ~(1<<15);
		if(z>0xFF47) {GPIOD->ODR |= 1<<15;
			wlacz3hz();
		}
		else {GPIOD->ODR &= ~(1<<15);
			wylacz();
		}
	}	

    //val_Tx = (val_Tx + 1) % 15;

//#ifndef __TEST
//    if (CAN_TxRdy[1]) {                           /* tx msg on CAN Ctrl #2    */
//      CAN_TxRdy[1] = 0;

 //     CAN_TxMsg[1].data[0] = val_Tx;              /* data[0] = ADC value      */
//			for (i = 1; i < 8; i++) CAN_TxMsg[1].data[i] = 0x23;
 //     CAN_wrMsg (2, &CAN_TxMsg[1]);               /* transmit message         */
 //   }
//#else
 //   if (CAN_TxRdy[0]) {                           /* tx msg on CAN Ctrl #2    */
 //     CAN_TxRdy[0] = 0;
///
 //     CAN_TxMsg[1].data[0] = val_Tx;              /* data[0] = ADC value      */
 //     CAN_wrMsg (1, &CAN_TxMsg[1]);               /* transmit message         */
 //   }
//#endif
//
//    Delay (10);                                   /* delay for 10ms           */

    //if (CAN_RxRdy[0]) {                           /* rx msg on CAN Ctrl #1    */
    //  CAN_RxRdy[0] = 0;

     // val_Rx = CAN_RxMsg[0].data[0];
    //}
	

  //  val_display ();                               /* display TX and RX values */
	
	//if (CAN_TxRdy[1]) {                           /* tx msg on CAN Ctrl #2    */
//	CAN_TxRdy[1] = 0;
	//	wlacz3hz;
	
	//}
	
		
		//Delay (100000);
		
  //  Delay (100000);                                  /* delay for 10000ms          */

  }


void wlacz3hz(void)
{//if(g>0,7&&hamulec == 1 && v>>50km/h)
	int i;
	CAN_TxMsg[1].id = 0x00000028;                           /* initialize msg to send  tu wpisujemy id wiadomosci (0 dla 3 hz, 1 dla 1,5 hz 2 dla wylacz (nizsze ID dla wylacz niz dla wlacz)) */
	for (i = 0; i < 8; i++) CAN_TxMsg[1].data[i] = 0;
	CAN_TxMsg[1].len = 8;
  CAN_TxMsg[1].format = EXTENDED_FORMAT;
	CAN_TxMsg[1].type = DATA_FRAME;
		if (CAN_TxRdy[1]) {                           /* tx msg on CAN Ctrl #2    */
	CAN_TxRdy[1] = 0;
	
  CAN_wrMsg (2, &CAN_TxMsg[1]);
}
}	
void wlacz1p5hz(void)
{//if(g<<0,1 && hamowanie bylo==1)
	int i;
	CAN_TxMsg[1].id = 0x00000001;                           /* initialize msg to send  tu wpisujemy id wiadomosci (0 dla 3 hz, 1 dla 1,5 hz 2 dla wylacz) */
  for (i = 0; i < 8; i++) CAN_TxMsg[1].data[i] = 0;
  CAN_TxMsg[1].len = 8;
  CAN_TxMsg[1].format = EXTENDED_FORMAT;
	CAN_TxMsg[1].type = DATA_FRAME;
}
void wylacz(void)
{//if(hamowanie bylo==1 && czaspohamowaniu==1 && hamulec !=0)
	int i;
	CAN_TxMsg[1].id = 0x00000077;                           /* initialize msg to send  tu wpisujemy id wiadomosci (0 dla 3 hz, 1 dla 1,5 hz 2 dla wylacz) */
	for (i = 0; i < 8; i++) CAN_TxMsg[1].data[i] = 0;
	CAN_TxMsg[1].len = 8;
  CAN_TxMsg[1].format = EXTENDED_FORMAT;
	CAN_TxMsg[1].type = DATA_FRAME;
	if (CAN_TxRdy[1]) {                           /* tx msg on CAN Ctrl #2    */
	CAN_TxRdy[1] = 0;
	
  CAN_wrMsg (2, &CAN_TxMsg[1]);
}

}

