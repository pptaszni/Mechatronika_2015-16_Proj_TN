/*
 * Zmienne_globalne.h
 *
 * Created: 2016-05-02 10:58:10
 *  Author: Mati
 */ 


#ifndef ZMIENNE_GLOBALNE_H_
#define ZMIENNE_GLOBALNE_H_

#define MAX_ILOSC_KODOW 12					//	maksymalnie 13 z powodu ograniczenia sterownika wyœwietlacza

// deklaracje zmiennych
uint8_t licznik = 0;
uint8_t i;
uint8_t kody_bledow[MAX_ILOSC_KODOW];	//={7,8, 23, 25, 79,80,88};
uint8_t ilosc_kodow_bledow = 0;
char string_16_znakow[16];
uint16_t czas_zmierzony;
uint16_t czas_konca;
uint16_t czas_jednosci;
uint16_t czas_przerw;
char bajty_z_karty[512];
uint16_t zmienna_16b;
const uint8_t adres_bof[4] = {0x02, 0x02, 0x02,0x00};	  // pocz¹tek pliku z kodami
uint8_t adres_dodatek[4];
uint8_t adres_tablicy[4];




#endif /* ZMIENNE_GLOBALNE_H_ */