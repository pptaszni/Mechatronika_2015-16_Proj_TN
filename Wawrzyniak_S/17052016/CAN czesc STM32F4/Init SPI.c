/*
 * init.c
 *
 *  Created on: 13-07-2013
 *      Author: Adrian
 */

#include "init.h"

void GPIO_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN;        /* Enable GPIOB i GPIOA clock         */
	GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1; //PA5,6,7 na AF
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1 | GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR7_1;  /* 50 MHz HIGH speed            */
	GPIOA->AFR[0] |= 0x00500000;//1<<(5*4) | 1<<(5*4+2); //nazwy w tym rejestrze sa skaszanione - ustawienie PA5 na AF5
	GPIOA->AFR[0] |= 0x05000000;
	GPIOA->AFR[0] |= 0x50000000;//1<<(6*4) | 1<<(6*4+2) | 1<<(7*4) | 1<<(7*4+2); //PA6, PA7 na AF5
	//GPIOA->PUPDR |= GPIO_PUPDR_PUPDR7_1;
	GPIOD->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_1 | GPIO_OSPEEDER_OSPEEDR13_1 | GPIO_OSPEEDER_OSPEEDR14_1 | GPIO_OSPEEDER_OSPEEDR15_1;
	GPIOE->MODER |= GPIO_MODER_MODER3_0;
	GPIOE->ODR |= 1<<3;
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;
}

void SPI_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // wlaczenie zegara
	SPI1->CR1 |= SPI_CR1_BR_2; // tutaj musze ustalic baud rate - narazie f/256
	SPI1->CR1 |= SPI_CR1_CPOL; // w stanie normalnym linia jest 1
	SPI1->CR1 |= SPI_CR1_CPHA; // zaczyna nadawac/odbierac od razu (dla zera od polowy cyklu)
	SPI1->CR1 |= SPI_CR1_DFF; // 16-bitowa ramka
	//SPI1->CR1 &= ~(SPI_CR1_LSBFIRST); //najpierw najbardziej znaczacy
	//SPI1->CR2 &= ~SPI_CR2_FRF; //SPI Motorola
	//SPI1->CR2 |= SPI_CR2_SSOE; //SS output
	SPI1->CR1 |= SPI_CR1_SSM; // sprzetowe zarzadzanie slavem
	SPI1->CR1 |= SPI_CR1_SSI; // slave sprzetowy wlaczony
	SPI1->CR1 |= SPI_CR1_MSTR; // urzadzenie jest masterem
	//SPI1->CR1 &= ~(SPI_CR1_BIDIMODE); //transmisja w obu kierunkach
	SPI1->CR1 |= SPI_CR1_SPE;
	//NVIC_EnableIRQ(SPI1_IRQn);
}

uint16_t SPI_send(uint16_t wysylana)
{
	uint32_t err=0;
	uint16_t pobrana=0;
	GPIOE->ODR &= ~(1<<3);

	SPI1->DR=wysylana; //wpisuje same zera do pierwszej ramki i osemka ze jest to 1 ramka na pewno
	while(!(SPI1->SR & SPI_SR_RXNE) /*&& err!=50000*/) err++; //czekam az odbierze pierwsza wartosc
	pobrana=SPI1->DR;
	while(!(SPI1->SR & SPI_SR_TXE));
	if(SPI1->SR > 0x0002) GPIOD->ODR |= 1<<14; //zapalenie diody
	while((SPI1->SR & SPI_SR_BSY) /*&& err!=50000*/) err++;
	if(SPI1->SR & SPI_SR_RXNE) err=SPI1->DR;
	GPIOE->ODR |= (1<<3);
	return pobrana;
}

void start_prog(void)
{
	GPIO_init();
	SPI_init();
}

