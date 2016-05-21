/*
 * Diagnostyczne.c
 *	//-U lfuse:w:0xF8:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m
 * Created: 2016-04-28 19:39:58
 * Author : Mati
 */ 


#include "main.h" 

int main(void)
{
	Ustaw_ADC();
	Ustaw_porty();
	LCD_Initalize();

	//	podczas wgrywania kasuje siê EEPROM	
	//	Stosowaæ podczas testów
	/*
	eeprom_write_word((uint16_t *)EEPROM_ADR_CZAS_KONCA,(uint16_t)36000);
	eeprom_write_word((uint16_t *)EEPROM_ADR_CZAS_JEDNOSCI,(uint16_t)3600);
	eeprom_write_word((uint16_t *)EEPROM_ADR_CZAS_PRZERW,(uint16_t)3000);
	czas_konca = (uint16_t)36000;
	czas_jednosci = (uint16_t)3600;
	czas_przerw = (uint16_t)3000;
	*/
	
	//	Wczytanie czasów z eepromu
	///*
	czas_konca = eeprom_read_word((const uint16_t *) EEPROM_ADR_CZAS_KONCA);
	czas_jednosci = eeprom_read_word((const uint16_t *) EEPROM_ADR_CZAS_JEDNOSCI);
	czas_przerw = eeprom_read_word((const uint16_t *) EEPROM_ADR_CZAS_PRZERW);
	 //*/

	 //	Pêtla g³ówna
	while (1) 
    {
		switch (licznik)
		{
		case M_WYBOR_FUNKCJI:
			licznik = Wybor_funkcji();
			break;
			
		case M_ZMIANY_CZASOW:
			licznik = Zmiany_czasow();
			break;
			
		case M_WPROWADZENIE_KODOW:
			licznik = Wprowadzenie_kodow();
			break;
			
		case M_ODCZYT_KODOW:
			licznik = Odczyt_kodow();
			break;
			
		case M_CZY_DEKODOWAC_KODY:
			licznik =  Czy_dekodowac_kody();
			break;
			
		case M_DEKODOWANIE_KODOW:
			licznik = Dekodowanie_kodow();
			break;
			
		case M_ZAKONCZ:
			licznik = Zakoncz();
			break;
			
		default:
			LCD_Clear();
			LCD_Home();
			LCD_WriteText("Blad wyboru instrukcji.");			
			_delay_ms(2000);
			break;
		}
		_delay_ms(1000);
    }
}

//	Funkcja do wyswietlania napisów na zakoñczenie pracy urz¹dzenia 
uint8_t Zakoncz(void)
{
	while(1)
	{	
		
		LCD_Clear();                                         
		LCD_Home();                                          
		LCD_WriteText("**Koniec pracy**          ");
		LCD_GoTo(M_DRUGA_LINIA_P1); 
		LCD_WriteText("1.Wylacz zaplon pojazadu. ");
		_delay_ms(2000);
		
		for(i=0;i<10;i++)
		{
			LCD_WriteCommand((HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT));
			_delay_ms(800);
		}
		
		LCD_Clear();                                                   
		LCD_Home();                                          
		LCD_WriteText("2. Wylacz urzadzenie.     ");
		LCD_GoTo(M_DRUGA_LINIA_P1); 
		LCD_WriteText("3. Odlacz przewody.       ");
		_delay_ms(2000);

		for(i=0;i<10;i++)
		{
			LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT);
			_delay_ms(800);
		}

	}

	return 0;
}

//	funkcja wyboru funkcji
uint8_t Wybor_funkcji(void)
{

	licznik = 0;                                            //	ustawienie wstepnej wartosci licznika
	LCD_Clear();                                            //	Czyszczenie Ekranu
	LCD_Home();												//	powrot karetki na poczatek ekranu
	LCD_WriteText("Wybierz funkcje ");                      //	wyœwietlanie pierwszego wiersza

	while(1)	//	du¿a petla z wyswietlaniem
	{
		
		LCD_GoTo(M_DRUGA_LINIA_P1);								// przejscie do 2 lini
		switch (licznik)
		{
			case 0:
				LCD_WriteText("Odczyt kodow    ");                      // wyœwietlanie drugiego wiersza
				break;
			
			case 1:
				LCD_WriteText("Przeglad kodow ");                      // wyœwietlanie drugiego wiersza
				break;
			
			case 2:
				LCD_WriteText("Zmiana czasow   ");                      // wyœwietlanie drugiego wiersza
				break;
			
			case 3:
				LCD_WriteText("Zakoncz         ");                      // wyœwietlanie drugiego wiersza
				break;
			
			default:
				LCD_Clear();
				LCD_Home();
				LCD_WriteText("Blad wyboru instrukcji.");
				_delay_ms(2000);
				licznik =0;
				break;
		}
		
		_delay_ms(500);			// opóŸnienie reakcji
		
		while(1)	//	petla obs³ugi przycisków
		{
			// sprawdzenie czerwonego przycisku
			if(!(BUTTON_PIN & (1<<BUTTON_RED)))
			{
				_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
				if(!(BUTTON_PIN & (1<<BUTTON_RED)))
				{
					switch (licznik)
					{
						case 0:	//	Odczyt kodow 
							return M_ODCZYT_KODOW;
						case 1:	//	Przeglad kodow
							return M_WPROWADZENIE_KODOW;
						case 2:	//	Zmiana czasow
							return M_ZMIANY_CZASOW;
						case 3:	//	Zakoñcz
							return M_ZAKONCZ;
						default:
						LCD_Clear();
						LCD_Home();
						LCD_WriteText("Blad wyboru funkcji.");
						_delay_ms(2000);
						licznik =0;
						break;
					}
				}
			}
	
		   // sprawdzenie niebieskiego przycisku
		   if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
		   {
			   _delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			   if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
			   {
					licznik++;
					licznik = licznik %4;	//	zmiana licznika 
					break;						//	powrót do duzej pêtli
			   }
		   }

		   // sprawdzenie zielonego przycisku
		   if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
		   {
			   _delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			   if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
			   {
				   licznik--;
				   licznik = licznik %4;	//	zmiana licznika
				   break;						//	powrót do duzej pêtli
			   }
		   }
		}
   
	}

	return 0;
}

#ifdef TEST_n_zmiana_czasow			//	  
uint8_t Zmiany_czasow(void){return M_WYBOR_FUNKCJI;}	//kod zastêpczy do szybkiego wgrywania

#else
uint8_t Zmiany_czasow(void)
{
	float wartosc_s;
	licznik = 0;
	ADMUX &=~ (1<<ADLAR);					//	prawe wyjustowanie wyniku konwersji ADC
	                                       
	while(1)		//	du¿a pêtla z wyswietlaniem
	{
		LCD_Clear();                                                     //czyszczenie lcd
		LCD_Home();
		switch (licznik)
		{
			case 0:
			LCD_WriteText("Czas:      KONCA");                      // wyœwietlanie pierwszego wiersza
			LCD_GoTo(M_DRUGA_LINIA_P1);
			dtostrf(((float)czas_konca)/3600,4,2,string_16_znakow);
			LCD_WriteText(string_16_znakow);
			LCD_GoTo(5,1);
			LCD_WriteText("s");
			break;
			
			case 1:
			LCD_WriteText("Czas:   JEDNOSCI");                      //	wyœwietlanie pierwszego wiersza
			LCD_GoTo(M_DRUGA_LINIA_P1);
			dtostrf(((float)czas_jednosci)/3600,4,2,string_16_znakow);
			LCD_WriteText(string_16_znakow);
			LCD_GoTo(5,1);
			LCD_WriteText("s");
			break;
			
			case 2:
			LCD_WriteText("Czas:     PRZERW");                      //	wyœwietlanie pierwszego wiersza
			LCD_GoTo(M_DRUGA_LINIA_P1);
			dtostrf(((float)czas_przerw)/3600,4,2,string_16_znakow);
			LCD_WriteText(string_16_znakow);
			LCD_GoTo(5,1);
			LCD_WriteText("s");
			break;
			
			case 3:
			LCD_WriteText("Zapisac?        ");                      //	wyœwietlanie pierwszego wiersza
			LCD_GoTo(M_DRUGA_LINIA_P1);								//	przejscie do 2 lini
			LCD_WriteText("Zielony = TAK   ");                      // wyœwietlanie drugiego wiersza
			break;
			
			default:
			LCD_Clear();
			LCD_Home();
			LCD_WriteText("Blad wyboru instrukcji.");
			_delay_ms(2000);
			licznik =0;
			break;
		}
		
		while(1)	//	petla obs³ugi przycisków
		{
			if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
			{
				_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
				if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
				{
					if(licznik == 3)
					{
						eeprom_write_word((uint16_t *)EEPROM_ADR_CZAS_KONCA,czas_konca);
						eeprom_write_word((uint16_t *)EEPROM_ADR_CZAS_JEDNOSCI,czas_jednosci);
						eeprom_write_word((uint16_t *)EEPROM_ADR_CZAS_PRZERW,czas_przerw);	
						LCD_Clear();
						LCD_Home();
						LCD_WriteText("    ZAPISANO    ");
						_delay_ms(2000);
						return M_WYBOR_FUNKCJI;
					}
					else
					{
						ADCSRA |= (1<<ADSC);			//	start konwersji
						while(ADCSRA &(1<<ADSC)) ;		//	czekanie na koniec konwersji
						//pomiar = ADC;						//	odczyt wyniku konwersji
												
						//	przypisanie do odpowiedniej zmiennej
						switch (licznik)
						{
							case 0:
								czas_konca =(ADC<<6);						//	odczyt wyniku konwersji
								wartosc_s = (float)czas_konca;
								break;
					
							case 1:
								czas_jednosci = (ADC<<4);						//	odczyt wyniku konwersji
								wartosc_s = (float)czas_jednosci;
								break;
					
							case 2:
								czas_przerw = (ADC<<4);						//	odczyt wyniku konwersji                      
								wartosc_s = (float)czas_przerw;
								break;
					
							
							default:
							wartosc_s = (float)-11;
							LCD_Clear();
							LCD_Home();
							LCD_WriteText("Blad wyboru    .");
							_delay_ms(2000);
							licznik = 0;
							break;
						}
						
						//	wyswietlenie zmierzonej wartosci
						LCD_GoTo(10,1);					
						dtostrf(wartosc_s/3600,4,2,string_16_znakow);
						LCD_WriteText(string_16_znakow);
						LCD_GoTo(15,1);
						LCD_WriteText("s");
						
					}
				}
			}
		
			
			// sprawdzenie niebieskiego przycisku
			if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
			{
				_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
				if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
				{
					licznik++;
					licznik = licznik %4;	//	zmiana licznika
					break;						//	powrót do duzej pêtli
				}
			}

			// sprawdzenie czerwonego przycisku
			if(!(BUTTON_PIN & (1<<BUTTON_RED)))
			{
				_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
				if(!(BUTTON_PIN & (1<<BUTTON_RED)))
				{	
					i=1;
					LCD_Clear();
					LCD_Home();
					LCD_WriteText("Wyjsce bez zapisu?");                      //	wyœwietlanie pierwszego wiersza
					LCD_GoTo(M_DRUGA_LINIA_P1);								//	przejscie do 2 lini
					LCD_WriteText("Zielony = TAK  Niebieski = ANULUJ   ");                      // wyœwietlanie drugiego wiersza
					_delay_ms(1000);
					while(1)
					{
						if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
						{
							_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
							if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
							{
								czas_konca = eeprom_read_word((const uint16_t *) EEPROM_ADR_CZAS_KONCA);
								czas_jednosci = eeprom_read_word((const uint16_t *) EEPROM_ADR_CZAS_JEDNOSCI);
								czas_przerw = eeprom_read_word((const uint16_t *) EEPROM_ADR_CZAS_PRZERW);
								return M_WYBOR_FUNKCJI;
							}
						}
						

						if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
						{
							_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
							if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
							{
								break;
							}
						}

						LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT);
						_delay_ms(500);
					}
				}

			if (i==1){break;}
			}

			

		}
	}
	return M_WYBOR_FUNKCJI;
}

#endif

#ifdef TEST_n_wprowadzenie_kodow	//	
uint8_t Wprowadzenie_kodow(void){return M_CZY_DEKODOWAC_KODY;}	//kod zastêpczy do szybkiego wgrywania

#else
uint8_t Wprowadzenie_kodow(void)
{
	licznik = 0;
	
	ADMUX |= (1<<ADLAR);	//	lewe wyjustowanie wyniku konwersji ADC

	while(1)	
	{
		LCD_Clear();
		LCD_Home();
		for (i=0;i<ilosc_kodow_bledow;i++)
		{
				
				if ((i+1)*3>16)
				{
					_delay_ms(800);
					LCD_WriteCommand((HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT));
					LCD_WriteCommand((HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT));
					LCD_WriteCommand((HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT));
				}
				
				if(kody_bledow[i]<10)
				{
					LCD_WriteText(" ");
				}
				LCD_WriteText(utoa(kody_bledow[i],string_16_znakow,10));
				LCD_WriteText(" ");
		}
		
		if (ilosc_kodow_bledow>=MAX_ILOSC_KODOW)
		{
			LCD_Clear();
			LCD_Home();
			LCD_WriteText("Maksymalna      ");
			LCD_GoTo(M_DRUGA_LINIA_P1);
			LCD_WriteText("ilosc kodow     ");
			_delay_ms(3000);
			return M_CZY_DEKODOWAC_KODY;
		}

		if (ilosc_kodow_bledow*3>16)
		{
			i=ilosc_kodow_bledow*3-15;
		}
		else
		{
			i=0;
		}
		LCD_GoTo(i,1);
		LCD_WriteText("Nowy kod:");

		ADCSRA |= (1<<ADSC);			//	start konwersji
		while(ADCSRA &(1<<ADSC)) ;		//	czekanie na koniec konwersji
		licznik = (ADCH>>1)+1;						//	odczyt wyniku konwersji //	dodanie 1 jako wartosci minimalnej
		
		if (licznik>99)
		{
			licznik=99;		//	ograniczenie wartosci kodu
		}
		
		//	wyswietlenie zmierzonej wartosci
		
		LCD_GoTo(i+13,1);
		LCD_WriteText(utoa(licznik,string_16_znakow,10));
		LCD_WriteText("  ");

		while(1)
		{
			if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
			{
				_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
				if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
				{
					
						ADCSRA |= (1<<ADSC);			//	start konwersji
						while(ADCSRA &(1<<ADSC)) ;		//	czekanie na koniec konwersji
						licznik = (ADCH>>1)+1;						//	odczyt wyniku konwersji //	dodanie 1 jako wartosci minimalnej
					
						if (licznik>99)
						{
							licznik=99;		//	ograniczenie wartosci kodu
						}
						
						//	wyswietlenie zmierzonej wartosci
						
						LCD_GoTo(i+13,1);
						LCD_WriteText(utoa(licznik,string_16_znakow,10));
						LCD_WriteText("  ");
				}
			}
		
		
			// sprawdzenie niebieskiego przycisku
			if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
			{
				_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
				if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
				{
					kody_bledow[ilosc_kodow_bledow] = licznik;
					ilosc_kodow_bledow=ilosc_kodow_bledow+1;
					break;						//	powrót do du¿ej pêtli
				}
			}

			// sprawdzenie czerwonego przycisku
			if(!(BUTTON_PIN & (1<<BUTTON_RED)))
			{
				_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
				if(!(BUTTON_PIN & (1<<BUTTON_RED)))
				{
					return M_CZY_DEKODOWAC_KODY;
					
				}
			}
			
		}
	}
	return M_CZY_DEKODOWAC_KODY;
}
#endif

#ifdef TEST_n_odczyt_kodow
uint8_t Odczyt_kodow(void){return M_CZY_DEKODOWAC_KODY;}	//kod zastêpczy do szybkiego wgrywania

#else
uint8_t Odczyt_kodow(void)
{	
	//	zerowanie kodow bledow
	for (i=0;i<MAX_ILOSC_KODOW;i++)
	{
		kody_bledow[i]=0;
	}
	Ustaw_przerwania_zew_INT();
	Ustaw_TIMER1((uint16_t)(((uint16_t)10)*((uint16_t)3600)));	//	dla uzyskania sekund pomno¿yc przez 3600
	licznik = 0;			//	informacja o opuszczeniu sygnalu startu
	ilosc_kodow_bledow = 0;
	TCNT1 = 0;				//	wyzerowanie licznika timera1

	//	wybranie trybu testu
	i=0;
	LCD_Clear();
	LCD_Home();
	LCD_WriteText("Który tryb testu?    ");                      //	wyœwietlanie pierwszego wiersza
	LCD_GoTo(M_DRUGA_LINIA_P1);								//	przejscie do 2 lini
	LCD_WriteText("Zielony = KOEO  Niebieski = KOER  ");                      // wyœwietlanie drugiego wiersza
	_delay_ms(1000);
	while(i==0)
	{
		if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
			{
				i=1;
				break;
			}
		}
		

		if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
			{
				i=2;
				break;
			}
		}

		if(!(BUTTON_PIN & (1<<BUTTON_RED)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_RED)))
			{
				return M_WYBOR_FUNKCJI;
			}
		}

		LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT);
		_delay_ms(500);
	}

	
	
	
	
	SIGNAL_ENABLE_PORT |= (1<<SIGNAL_ENABLE);	//	sygna³ dla ECU o chêci odczytu kodów
	
	if (i==2)	  //	tryb KOER
	{
		LCD_Clear();
		LCD_Home();
		LCD_WriteText("Wlacz zaplon pojaz.");                      //	wyœwietlanie pierwszego wiersza
		LCD_GoTo(M_DRUGA_LINIA_P1);								//	przejscie do 2 lini
		LCD_WriteText("Czekaj na  \"KONIEC\"");                   // wyœwietlanie drugiego wiersza
		while(licznik==0)
		{
			
			LCD_Clear();
			LCD_Home();
			LCD_WriteText("1. Wlacz zaplon pojazdu.  ");
			LCD_GoTo(M_DRUGA_LINIA_P1);
			LCD_WriteText("2. Przeczekaj kody KOEO.  ");
			_delay_ms(2000);
			
			for(i=0;i<10;i++)
			{
				LCD_WriteCommand((HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT));
				_delay_ms(800);
				if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
				{
					_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
					if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
					{
						licznik =1;
						break;
					}
				}
			}
			if (licznik==1)
			{
				break;
			}
			LCD_Clear();
			LCD_Home();
			LCD_WriteText("3. Wcisnij niebieski przycisk.");
			LCD_GoTo(M_DRUGA_LINIA_P1);
			LCD_WriteText("4. Uruchom silnik i czekaj na  \"KONIEC\" ");
			_delay_ms(2000);

			for(i=0;i<24;i++)
			{
				LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT);
				_delay_ms(800);
			
				if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
				{
					_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
					if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
					{
						licznik =1;
						break;
					}
				}
			}
		}
		LCD_Clear();
		LCD_Home();
		LCD_WriteText("Uruchom silnik. ");                      //	wyœwietlanie pierwszego wiersza
		LCD_GoTo(M_DRUGA_LINIA_P1);								//	przejscie do 2 lini
		LCD_WriteText("Czekaj na  \"KONIEC\"");                   // wyœwietlanie drugiego wiersza
	} 
	else		//	tryb KOEO
	{
		LCD_Clear();
		LCD_Home();
		LCD_WriteText("Wlacz zaplon pojaz.");                      //	wyœwietlanie pierwszego wiersza
		LCD_GoTo(M_DRUGA_LINIA_P1);								//	przejscie do 2 lini
		LCD_WriteText("Czekaj na  \"KONIEC\"");                   // wyœwietlanie drugiego wiersza
		for(i=0;i<4;i++)
		{
			LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT);
			_delay_ms(800);
		}
	}

	i=-1;
	licznik =0;		
	
	TCCR1B = START_TIMER1;	//	start timera1
	sei();


	//LCD_Clear();
	
	while((licznik!=1) & (licznik!=3) )
	{
		
		
		cli();
		_delay_ms(10);
		sei();
	}
	while((licznik!=2) & (licznik!=3))
	{
		cli();
		_delay_ms(10);
		sei();
		
	}
	
	sei();

	while(licznik!=3)
	{
		if (licznik ==1)
		{
			cli();
			if (czas_zmierzony>czas_przerw)
			{
				i++;
				if (i==MAX_ILOSC_KODOW)	//	sprawdzenie czy nie przekraczamy tablicy kodów
				{
					break;
				}
				ilosc_kodow_bledow = i+1;
				
			}
			licznik = 0;
			sei();
		}
		else if(licznik== 2)
		{
			cli();
			if (czas_zmierzony>czas_jednosci)
			{
				kody_bledow[i] = kody_bledow[i] + 10;
			} 
			else
			{
				kody_bledow[i] = kody_bledow[i] + 1;
			}
			
			licznik = 0;
			sei();
		}
		
	}
	cli();
	
	//ilosc_kodow_bledow++;

	SIGNAL_ENABLE_PORT &=~ (1<<SIGNAL_ENABLE);	//	sygna³ dla ECU o chêci odczytu kodów

	LCD_Clear();
	LCD_Home();
	LCD_WriteText("    ***KONIEC***   ");                      //	wyœwietlanie pierwszego wiersza
	LCD_GoTo(M_DRUGA_LINIA_P1);
	_delay_ms(800);

	if (ilosc_kodow_bledow==0)
	{
		LCD_WriteText("Brak blendow    ");
	} 
	else
	{
		for (i=0;i<ilosc_kodow_bledow;i++)
		{
				if(kody_bledow[i]<10)
				{
					LCD_WriteText(" ");
				}
				LCD_WriteText(utoa(kody_bledow[i],string_16_znakow,10));
				LCD_WriteText(" ");
		}
	}
	
	while(1)
	{

		// sprawdzenie niebieskiego przycisku
		if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
			{
				if ((ilosc_kodow_bledow)*3>16)
				{
					LCD_WriteCommand((HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT));
					LCD_WriteCommand((HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT));
					LCD_WriteCommand((HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT));
				}
			}
		}

		// sprawdzenie czerwonego przycisku
		if(!(BUTTON_PIN & (1<<BUTTON_RED)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_RED)))
			{
				return M_CZY_DEKODOWAC_KODY;
			
			}
		}
	}

	return M_CZY_DEKODOWAC_KODY;
}
#endif

uint8_t Czy_dekodowac_kody(void)
{	
	LCD_Clear();
	LCD_Home();
	LCD_WriteText("Dekodowac kody?    ");                      //	wyœwietlanie pierwszego wiersza
	LCD_GoTo(M_DRUGA_LINIA_P1);								//	przejscie do 2 lini
	LCD_WriteText("Zielony = TAK  Niebieski = NIE  ");                      // wyœwietlanie drugiego wiersza
	_delay_ms(1000);
	while(1)
	{
		if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
			{
				return M_DEKODOWANIE_KODOW;
			}
		}
		

		if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
			{
				return M_ZAKONCZ;
			}
		}

		if(!(BUTTON_PIN & (1<<BUTTON_RED)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_RED)))
			{
				return M_WYBOR_FUNKCJI;
			}
		}

		LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_SHIFT_LEFT);
		_delay_ms(500);
	}
	
	return M_ZAKONCZ;
}

#ifdef TEST_n_dekodowanie_kodow
uint8_t Dekodowanie_kodow(void)	// kod zastêpczy do szybkiego wgrywania
{	
	PORTB = 0;
	while(1)
	{
		sdss();
		PORTB = 0xFF;
		_delay_ms(1500);
		sdus();
		PORTB = 0x00;
		_delay_ms(1500);
	}
	return M_ZAKONCZ;
}	

#else
uint8_t Dekodowanie_kodow(void)
{	int8_t j=0;
	uint16_t k;
	LCD_Clear();
	LCD_Home();
	if(!(IF_CARD_PIN & (1<<IF_CARD)))
	{
		LCD_WriteText("Brak Karty");
		_delay_ms(3000);
		return M_ZAKONCZ;
	}
	Inicjalizacja_SPI();

	Inicjalizacja_SD();
	
	//	adres pocz¹tku spisu tablic z kodami od poczatku pliku
	adres_dodatek[0] = 0x00;
	adres_dodatek[1] = 0x00;
	adres_dodatek[2] = 0x04;
	adres_dodatek[3] = 0x00;
	
	for (i=0;i<4;i++)
	{
		adres_dodatek[i] = adres_dodatek[i] + adres_bof[i];
	}
	zmienna_16b = Odczyt_danych_z_SD(adres_dodatek,bajty_z_karty);

	
	j =  bajty_z_karty[31];
	licznik = 0;
	LCD_Clear();
	LCD_Home();
	LCD_WriteText("Wybierz pojazd ");
	LCD_GoTo(M_DRUGA_LINIA_P1);
	for(k=licznik*32;k<16+licznik*32;k++)
	{
		if (bajty_z_karty[k]==':')
		{
			break;
		}
		LCD_WriteData(bajty_z_karty[k]);
	}
	
	while(licznik!=-1)
	{
		if(!(BUTTON_PIN & (1<<BUTTON_RED)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_RED)))
			{
				 adres_tablicy[0] = bajty_z_karty[32*licznik+27]+ adres_bof[0];
				 adres_tablicy[1] = bajty_z_karty[32*licznik+28]+ adres_bof[1];
				 adres_tablicy[2] = bajty_z_karty[32*licznik+29]+ adres_bof[2];
				 adres_tablicy[3] = bajty_z_karty[32*licznik+30]+ adres_bof[3];
				licznik = -1;
				break;
			}
		}
		
		if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
			{
				licznik = (++licznik)%j;
				
				LCD_GoTo(M_DRUGA_LINIA_P1);
				for(k=licznik*32;k<16+licznik*32;k++)
				{
					if (bajty_z_karty[k]==':')
					{
						LCD_WriteData(0x20);
						LCD_WriteData(0x20);
						LCD_WriteData(0x20);
						LCD_WriteData(0x20);
						break;
					}
					LCD_WriteData(bajty_z_karty[k]);
				}  
			}
		}
		if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
			{
				licznik = licznik-1;
				if (licznik==0xff)
				{
					licznik = j-1;
				}
				
				
				LCD_GoTo(M_DRUGA_LINIA_P1);
				LCD_GoTo(M_DRUGA_LINIA_P1);
				for(k=licznik*32;k<16+licznik*32;k++)
				{
					if (bajty_z_karty[k]==':')
					{
						LCD_WriteData(0x20);
						LCD_WriteData(0x20);
						LCD_WriteData(0x20);
						LCD_WriteData(0x20);
						break;
					}
					LCD_WriteData(bajty_z_karty[k]);
				}
				
			}
		}
	}
	LCD_Home();
	LCD_WriteText("*Wybrano pojazd*") ;
	_delay_ms(2000);

	//////////////////////////////////////////////////////////////////////////
	//	wyswietlanie kodow wg wybranej tablicy
	j =  0;
	licznik = 0;
	
	zmienna_16b = (kody_bledow[licznik]*64/512)*512;		//	uzyskanie odsuniecia, 64 znaki na kod i  512 na odczyt. dzielimy 512 aby uzyskac numer klasteru i potem mnozymy w celu otrzymania jego adresu
	adres_dodatek[0] = adres_tablicy[0]+(uint8_t)(zmienna_16b>>24);
	adres_dodatek[1] = adres_tablicy[1]+(uint8_t)(zmienna_16b>>16);
	adres_dodatek[2] = adres_tablicy[2]+(uint8_t)(zmienna_16b>>8);
	adres_dodatek[3] = adres_tablicy[3]+(uint8_t)(zmienna_16b);

	
	Odczyt_danych_z_SD(adres_dodatek,bajty_z_karty);
	LCD_Clear();
	LCD_Home();
	for(k=kody_bledow[licznik]*64%512;k<16+kody_bledow[licznik]*64%512;k++)
	{
		if (bajty_z_karty[k]<0x20)
		{
			j=-1;
			break;
		}
		LCD_WriteData(bajty_z_karty[k]);
	}
	if (j!=-1)
	{
		LCD_GoTo(M_DRUGA_LINIA_P1);
		for(k=16+kody_bledow[licznik]*64%512;k<32+kody_bledow[licznik]*64%512;k++)
		{
			if (bajty_z_karty[k]<0x20)
			{
				j=-1;
				break;
			}
			LCD_WriteData(bajty_z_karty[k]);
		}
	}
	_delay_ms(500);
	
	
	while(1)
	{
		if(!(BUTTON_PIN & (1<<BUTTON_RED)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_RED)))
			{
				return M_ZAKONCZ;
			}
		}
		
		if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_GREEN)))
			{
				LCD_Clear();
				LCD_Home();
				j=j+1;
				
				for(k=16*j+kody_bledow[licznik]*64%512;k<16+16*j+kody_bledow[licznik]*64%512;k++)
				{
					if (bajty_z_karty[k]<0x20)
					{
						j=-1;
						break;
					}
					LCD_WriteData(bajty_z_karty[k]);
				}
				if (j!=-1)
				{
					LCD_GoTo(M_DRUGA_LINIA_P1);
					for(k=16+16*j+kody_bledow[licznik]*64%512;k<32+16*j+kody_bledow[licznik]*64%512;k++)
					{
						if (bajty_z_karty[k]<0x20)
						{
							j=-1;
							break;
						}
						LCD_WriteData(bajty_z_karty[k]);
					}
				}
				_delay_ms(500);
			}
		}
		if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
		{
			_delay_ms(CZAS_ZWLOKI_PRZYCISKOW_MS);
			if(!(BUTTON_PIN & (1<<BUTTON_BLUE)))
			{
				j=0;
				licznik = (++licznik)%ilosc_kodow_bledow;
				zmienna_16b = (kody_bledow[licznik]*64/512)*512;		//	uzyskanie odsuniecia, 64 znaki na kod i  512 na odczyt. dzielimy 512 aby uzyskac numer klasteru i potem mnozymy w celu otrzymania jego adresu
				adres_dodatek[0] = adres_tablicy[0]+(uint8_t)(zmienna_16b>>24);
				adres_dodatek[1] = adres_tablicy[1]+(uint8_t)(zmienna_16b>>16);
				adres_dodatek[2] = adres_tablicy[2]+(uint8_t)(zmienna_16b>>8);
				adres_dodatek[3] = adres_tablicy[3]+(uint8_t)(zmienna_16b);

				
				Odczyt_danych_z_SD(adres_dodatek,bajty_z_karty);
				LCD_Clear();
				LCD_Home();
				for(k=kody_bledow[licznik]*64%512;k<16+kody_bledow[licznik]*64%512;k++)
				{
					if (bajty_z_karty[k]<0x20)
					{
						j=-1;
						break;
					}
					LCD_WriteData(bajty_z_karty[k]);
				}

				if (j!=-1)
				{
					LCD_GoTo(M_DRUGA_LINIA_P1);
					for(k=16+kody_bledow[licznik]*64%512;k<32+kody_bledow[licznik]*64%512;k++)
					{
						if (bajty_z_karty[k]<0x20)
						{
							j=-1;
							break;
						}
						LCD_WriteData(bajty_z_karty[k]);
					}
				}
				_delay_ms(500);
			}
		}
	}




	return M_ZAKONCZ;
}
#endif

//przerwanie startu sygnalu
ISR(INT0_vect)
{
	TCCR1B = STOP_TIMER1;	//	stop timera1
	czas_zmierzony = TCNT1;	//	odczytanie licznika timera1
	TCNT1 = 0;				//	wyzerowanie licznika timera1
	TCCR1B = START_TIMER1;	//	start timera1
	licznik = 1;			//	inforamcja o typie czasu; 0 dla czasu sprawdzonego; 1 dla przerwy; 2 dla sygnalu; 3 dla konca; 
	_delay_ms(3);
}


//	przerwanie koñca sygnalu
ISR(INT1_vect)
{
	TCCR1B = STOP_TIMER1;	//	stop timera1
	czas_zmierzony = TCNT1;	//	odczytanie licznika timera1
	TCNT1 = 0;				//	wyzerowanie licznika timera1
	TCCR1B = START_TIMER1;	//	start timera1
	licznik = 2;			//	inforamcja o typie czasu; 0 dla czasu sprawdzonego; 1 dla przerwy; 2 dla sygnalu; 3 dla konca;
	_delay_ms(3);
}


//	przerwanie koñca czasu sprawdzania
ISR(TIMER1_COMPA_vect)
{
	TCCR1B = STOP_TIMER1;	//	stop timera1
	licznik = 3;			//	inforamcja o typie czasu; 0 dla czasu sprawdzonego; 1 dla przerwy; 2 dla sygnalu; 3 dla konca; 
}


