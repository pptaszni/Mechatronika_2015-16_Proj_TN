#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdlib.h> 
#include<string.h>
#include<time.h>
#include"Pgm.h"

using namespace std;


int main()
{
	int wysokosc, szerokosc, skalaSzarosci;
	int d=25;
	char nazwaObrazu[100];
	char nazwa[5] = "Znak";
	char nazwa2From[5] = ".pgm";
	struct Obraz* o = NULL; 

	//wartoœci otrzymywanych obrazów
    wysokosc = 640;
    szerokosc = 480;
    skalaSzarosci = 255;


	srand((unsigned int)time(NULL));

	//wczytanie zdjecia znaku do buffora
	strcat (nazwa, nazwa2From); //po³¹czenie nazwy z rozszerzeniem pliku
	o = wczytajObraz(nazwa);

	//progowanie obrazu
	if(o != NULL)
	{
		o = Progowanie(o);
	}

	//usuniecie obrabianego zdjecia
	if(o != NULL)
	{
		o = NULL;
	}
	
return 0;	
}
