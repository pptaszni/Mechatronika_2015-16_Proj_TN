/*
 * main.h
 *
 * Created: 2016-05-02 10:55:56
 *  Author: Mati
 */ 


#ifndef MAIN_H_
#define MAIN_H_

// makra deklarowane do trybu tesów, w³anczaj¹ one krótkie funkcje nie zajmujace pamieci programu (d³ugie czasy wgrywania)
//#define TEST_n_zmiana_czasow
//#define TEST_n_wprowadzenie_kodow
//#define TEST_n_odczyt_kodow
//#define TEST_n_dekodowanie_kodow

//	makra
#define F_CPU 4E6							//	czêstotliwoœæ mikrokontrolera // zwiekszona z powodu problemów z wyœwietlaczem, rzeczywista 3.6864E6
#define M_DRUGA_LINIA_P1 0,1
#define	M_WYBOR_FUNKCJI 0
#define M_ZMIANY_CZASOW 1
#define M_WPROWADZENIE_KODOW 2
#define M_ODCZYT_KODOW 3
#define M_CZY_DEKODOWAC_KODY 4
#define M_DEKODOWANIE_KODOW 5
#define M_ZAKONCZ 6
#define EEPROM_ADR_CZAS_KONCA 0x02
#define EEPROM_ADR_CZAS_JEDNOSCI 0x04
#define EEPROM_ADR_CZAS_PRZERW 0x06
#define CZAS_ZWLOKI_PRZYCISKOW_MS 100

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib\SD\Obsluga_SD.h"
#include "Zmienne_globalne.h"
#include "lib\lcd\HD44780.h" // biblioteki do obs³ugi lcd, porty lcd ju¿ ustawione
#include "Porty.h"	//	ustawienia portów i ich inicjalizacja


// deklaracje funkcji
uint8_t Zakoncz(void);
uint8_t Wybor_funkcji(void);
uint8_t Zmiany_czasow(void);
uint8_t Wprowadzenie_kodow(void);
uint8_t Odczyt_kodow(void);
uint8_t Czy_dekodowac_kody(void);
uint8_t Dekodowanie_kodow(void);


#endif /* MAIN_H_ */