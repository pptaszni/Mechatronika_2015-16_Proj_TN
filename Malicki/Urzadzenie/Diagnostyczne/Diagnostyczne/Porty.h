/*
 * Porty.h
 *
 * Created: 2016-04-29 20:07:44
 *  Author: Mati
 */ 


#ifndef PORTY_H_
#define PORTY_H_

#include <avr/io.h>

//	definicje sta�ych dla TIMER1 dla TCCR1B
#define START_TIMER1 5	//	W��czenie timera, ustawienie prescalera na 1024
#define STOP_TIMER1 0	//	wyl�czenie timera


// definicje port�w wejsciowych
#define IF_CARD_DIR		DDRB
#define IF_CARD_PIN 	PINB
#define IF_CARD_PORT 	PORTB
#define IF_CARD			PINB1


#define BUTTON_DIR			DDRD
#define BUTTON_PIN 			PIND
#define BUTTON_PORT 		PORTD
#define BUTTON_RED			PIND7
#define BUTTON_GREEN		PIND6
#define BUTTON_BLUE			PIND5

#define COMPERE_DIR			DDRD
#define COMPERE_PIN 		PIND
#define COMPERE_1			PIND3
#define COMPERE_2			PIND2

//definicje port�w wyjsciowych
#define SIGNAL_ENABLE_DIR		DDRB
#define SIGNAL_ENABLE_PORT 		PORTB
#define SIGNAL_ENABLE			PORTB0

//definicje port�w ADC
#define POT			PORTB0



//deklaracje funkcji
void Ustaw_porty(void);
void Ustaw_ADC(void);
void Ustaw_TIMER1(uint16_t czas_sprawdzania);
void Ustaw_przerwania_zew_INT(void);

#endif /* PORTY_H_ */