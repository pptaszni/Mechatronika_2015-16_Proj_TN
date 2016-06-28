#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include "Pgm.h"

using namespace std;


struct Obraz* utworzObraz(int wysokosc, int szerokosc, int skalaSzarosci, char nazwa[])
{
	int k; 
	struct Obraz* o;
	o = (struct Obraz*)malloc(sizeof(struct Obraz));
	o->wysokosc = wysokosc;
	o->szerokosc = szerokosc;
	o->skalaSzarosci = skalaSzarosci;
	o->srednia = 0; 
	for(k=0; k<25; k++)
	{
		o->nazwa[k] = nazwa[k];
	}
	o->wartosci = (int**)calloc((o->wysokosc),sizeof(int*));
	for(k=0; k < o->wysokosc; k++)
	{
		o->wartosci[k] = (int*)calloc((o->szerokosc),sizeof(int));
	}
	return o;
}

struct Obraz* wczytajObraz(char nazwa[])
{
	int i,j;
	int suma = 0;
	char c, tekst[20];
	int szerokosc, wysokosc, skalaSzarosci;
	char buf[8];
	struct Obraz* o;
	FILE *fp ;

	fp = fopen(nazwa, "r");
	if(fp != NULL)
	{
		fgets(buf,7,fp);
		if(buf[0] =! 'P')
		{
			printf("to nie jest plik .pgm");
		}
		else
		{
			if(buf[1] != '2')
			{
				printf("to nie jest plik .pgm");
			}
			else
			{
				do
				{
					c = getc(fp);
					if(c != '#')
					{
						fseek(fp, -1, 1);
						fscanf(fp, "%d %d", &szerokosc,  &wysokosc);
					}
					else
					{
						fscanf(fp, "%s", &tekst);
						printf("# %s\n", tekst);
					}
				}
				while(c == '#' );
				fscanf(fp, "%d", &skalaSzarosci);

				o = utworzObraz(wysokosc, szerokosc, skalaSzarosci, nazwa);

				for(i=0; i<o->wysokosc; i++)
				{
					for(j=0; j<o->szerokosc; j++)
					{	
						fscanf(fp, "%d", &o->wartosci[i][j]);
						suma = suma + o->wartosci[i][j];
					}
				}
			}
			fclose( fp );
			o->srednia = (float)suma/(o->szerokosc*o->wysokosc);
			printf("obraz wczytano poprawnie\n");
		}
	}
	else
	{
		printf("nie udalo sie otworzyc pliku, obraz nie zostal wczytany\n");
	}
	return o;
}

struct Obraz* Progowanie(struct Obraz* o)
{
	int i,j , k, sumaL, sumaM, Gx, Gy, G;
	// sumaL oraz sumaM to odpowiednio sumy mianownika i licznika
	// zmienne Gx Gy oraz G to zmienne gradientu jasnoœci 
	int** tab;
	float prog;

	sumaL = 0;
	sumaM = 0;

	tab = (int**)malloc((o->wysokosc+2)*sizeof(int*));
	for(k=0; k < (o->wysokosc+2); k++)
	{
		tab[k] = (int*)malloc((o->szerokosc+2)*sizeof(int));
	}

	tab[0][0] = o->wartosci[0][0];
	tab[0][o->szerokosc+1] = o->wartosci[0][o->szerokosc-1];
	tab[o->wysokosc+1][0] = o->wartosci[o->wysokosc-1][0];
	tab[o->wysokosc+1][o->szerokosc+1] = o->wartosci[o->wysokosc-1][o->szerokosc-1];

	for(i=0; i< o->wysokosc; i++)
	{
		tab[i+1][0]=o->wartosci[i][0];
		tab[i+1][o->szerokosc+1]=o->wartosci[i][o->szerokosc-1];
	}

	for(i=0; i< o->szerokosc; i++)
	{
		tab[0][i+1]=o->wartosci[0][i];
		tab[o->wysokosc+1][i+1]= o->wartosci[o->wysokosc-1][i];
	}

	for(i=0; i< o->wysokosc; i++)
	{
		for(j=0; j< o->szerokosc; j++)
		{
			tab[i+1][j+1]=o->wartosci[i][j];
		}
	}
	for(i=0; i< o->wysokosc; i++)
	{
		for(j=0; j< o->szerokosc; j++)
		{
			Gx= tab[i+1][j+2] - tab[i+1][j];
			Gy= tab[i+2][j+1] - tab[i][j+1];

			if(Gx < 0)
			{
				Gx = -Gx;
			}

			if(Gy < 0)
			{
				Gy = -Gy;
			}

			if(Gx > Gy)
			{
				G = Gx;
			}
			else
			{
				G = Gy;
			}

			sumaL= sumaL + (tab[i+1][j+1] * G );
			sumaM = sumaM + G ;
		}
	}
	prog = (float)(sumaL/sumaM);

	for(i=0; i< o->wysokosc; i++)
	{
		for(j=0; j< o->szerokosc; j++)
		{ 
			if(o->wartosci[i][j] <= prog)
			{
				o->wartosci[i][j] = o->skalaSzarosci;
			}
			else
			{
				o->wartosci[i][j]= 0;
			}
		}
	}
	cout << "Operacja zakonczona" ;
	return o;
}

