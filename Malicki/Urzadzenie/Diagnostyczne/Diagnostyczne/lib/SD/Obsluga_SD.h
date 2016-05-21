/*
 * Obsluga_SD.h
 *
 * Created: 2016-05-02 08:31:46
 *  Author: Mati
 */ 


#ifndef OBSLUGA_SD_H_
#define OBSLUGA_SD_H_


//#define F_CPU 3.8E6							//	czêstotliwoœæ mikrokontrolera // zwiekszona z powodu problemów z wyœwietlaczem, rzeczywista 3.6864E6

#include <avr/io.h>
#include <util/delay.h>
//#include "../../Zmienne_globalne.h"

//	deklaracja trybu testu spi; wyswietlanie na lcd
#define SPI_INPUT_LCD_TEST

//	deklaracje portów
#define SPI_DIR		DDRB
#define SPI_PIN 	PINB
#define SPI_PORT 	PORTB
#define SPI_SS		PINB2
#define SPI_MOSI	PINB3 
#define SPI_MISO	PINB4
#define SPI_SCK		PINB5


//	komendy SD
#define SD_GO_IDLE_STATE			64	//	Software reset.
#define SD_SEND_OP_COND				65	//	Initiate initialization process.
#define SD_SEND_IF_COND				72	//	For only SDC V2. Check voltage range.
#define SD_SEND_CSD					73	//	Read CSD register.
#define SD_SEND_CID					74	//	Read CID register.
#define SD_STOP_TRANSMISSION		76	//	Stop to read data.
#define SD_SET_BLOCKLEN				80	//	Change R/W block size.
#define SD_READ_SINGLE_BLOCK		81	//	Read a block.
#define SD_READ_MULTIPLE_BLOCK		82	//	Read multiple blocks.
#define SD_SET_BLOCK_COUNT			87	//	For only MMC. Define number of blocks to transfer with next multi-block read/write command.
#define SD_WRITE_BLOCK				88	//	Write a block.
#define SD_WRITE_MULTIPLE_BLOCK		89	//	Write multiple blocks.
#define SD_INI_SDC					105	//	Initiate initialization process for SDC.
#define SD_APP_CMD					119	//	Leading command of ACMD<n> command.
#define SD_READ_OCR					122	//	Read OCR.


//uint8_t sd_komenda[]

//	funkcje

void Inicjalizacja_SPI(void);
uint8_t Wyslij_bajt_SPI(uint8_t bajt);
uint16_t Inicjalizacja_SD(void);
inline void sdss(void){asm("cbi 0x05,2");}
inline void sdus(void){asm("sbi 0x05,2");}
uint16_t Odczyt_danych_z_SD(uint8_t* adres_odczytu, char* adres_tablicy);

#endif /* OBSLUGA_SD_H_ */