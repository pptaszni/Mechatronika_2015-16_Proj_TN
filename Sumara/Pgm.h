#ifndef PGM_H
#define PGM_H
#pragma once
#include<stdio.h>


class Pgm
{
	public:
	protected:
};

#endif


struct Obraz
{
	int   wysokosc;
	int   szerokosc;
	int   skalaSzarosci;
	int** wartosci;
	float  srednia;
	char nazwa[5];
};

//funkcja tworzy strukture zawierajaca dane obrazu .pgm w tym wysokosc szerokosc skale szarosci oraz
//tablice dwu wymiarowa wypelniona zerami
struct Obraz* utworzObraz(int wysokosc, int szerokosc, int skalaSzarosci, char nazwaObrazu[]);

//wczytuje obraz .pgm o podanej nazwie przez uzytkownika do programu. Program jest zabezpieczony przed wczytywaniem innych
//plików
struct Obraz* wczytajObraz(char nazwa[]);

// funkcja oblicza prog na podstawie gradientu jasnosci obrazu a nastepnie przypisuje wartosci 0 lub skala szarosci w 
// zaleznosci czy wartosc piksela jest powyrzej czy ponizej progu
struct Obraz* Progowanie(struct Obraz* o);



