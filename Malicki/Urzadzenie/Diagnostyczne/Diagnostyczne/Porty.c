/*
 * Porty.c
 *
 * Created: 2016-04-29 20:27:12
 *  Author: Mati
 */ 
#include "Porty.h"

void Ustaw_porty(void)
{
	
	IF_CARD_DIR	&=~	(1<<IF_CARD);	//	jako wejscie
	IF_CARD_PORT |= (1<<IF_CARD);	//	pullup
	
	BUTTON_DIR &=~	(1<<BUTTON_RED) | (1<<BUTTON_GREEN) | (1<<BUTTON_BLUE);		//	jako wejscie
	BUTTON_PORT |=	(1<<BUTTON_RED) | (1<<BUTTON_GREEN) | (1<<BUTTON_BLUE);		//	pullup
	  		
	
	COMPERE_DIR	 &=~ (1<<COMPERE_1) | (1<<COMPERE_2);	//	jako wejscie
	
	SIGNAL_ENABLE_DIR	|= (1<<SIGNAL_ENABLE);	//	jako wyjscie
	return;
}




void Ustaw_ADC(void)
{
	//ADMUX = (uint8_t)0;				//	napiecie odniesienia zewnetrzne AREF, prawe wyjustowanie wyniku,odczyt z kanalu ADC0 
	ADCSRA |= (1<<ADEN);			//	&B10000000 = w³¹czenie ADC, pojedyncze przetwarzanie,wy³¹czone przerwanie prescaler 2
	if (POT<=5) DIDR0 |= (1<<POT);	//	wy³¹czenie bufora cyfrowego na wejœciu analogowym
	return;
}




void Ustaw_TIMER1(uint16_t czas_sprawdzania)
{
	OCR1A = czas_sprawdzania;			//	ustawienie czasu koñca sprawdzania od ostatniego zbocza narastaj¹cego
	TIMSK1 |= (1<<OCIE1A);				//	w³aczenie przerwania dla porownania
	return;
}



void Ustaw_przerwania_zew_INT(void)
{
	EICRA |= (1<<ISC11) | (0<<ISC10) | (1<<ISC01) | (1<<ISC00); // wywo³anie przerwañ: INT0 narastaj¹ce zbocze; INT1 zbocze opadajace
	EIMSK |= (1<<INT1) | (1<<INT0);				//	w³¹czenie przerwañ
	return;
}