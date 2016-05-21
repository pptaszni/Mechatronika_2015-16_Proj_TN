/*
 * Obsluga_SD.c
 *
 * Created: 2016-05-02 08:32:26
 *  Author: Mati
 */ 
 #include "Obsluga_SD.h"
 


 void Inicjalizacja_SPI(void)
 {
	 //	ustawienie wyjsc
	 SPI_DIR = (1<<SPI_MOSI) | (1<<SPI_SCK) |(0<<SPI_MISO) | (1<<SPI_SS);
	 SPI_PORT = (1<<SPI_MOSI) | (1<<SPI_SCK) |(0<<SPI_MISO) | (1<<SPI_SS);
	 // w³aczenie SPI; tryb MASTER; MODE 0; prescaler segara procesora 16(SPR0=1) lub 4(SPCR0=0)
	 SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR0);
	 return;
 }


 uint8_t Wyslij_bajt_SPI(uint8_t bajt)
 {
	 //	Za³adowanie bufora
	 SPDR = bajt;
	 // czekanie na koniec transmisji
	 while(!(SPSR & (1<<SPIF))){}
	 //	odczyt bufora
	// _delay_us(200);
	 return SPDR;
 }

 uint16_t Inicjalizacja_SD(void)
 {
	uint8_t i;
	while(1)
	{
		for (i=0;i<10;i++)
		{
			Wyslij_bajt_SPI(0xFF);
		}

		
		sdss();
		//	wys³anie komendy resetu
		Wyslij_bajt_SPI(SD_GO_IDLE_STATE);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		i=Wyslij_bajt_SPI(0x95);

		//	pusta pêtla czekania na odpowiedŸ
		while(i==0xff)
		{
			
			i=Wyslij_bajt_SPI(0xFF);
		}

		sdus();
		
		//	sprawdzenie odpowiedzi karty
		if (i==0x01)
		{
			//return (0x4100 +i);
			break;
		}
	}

	
	Wyslij_bajt_SPI(0xFF);
	//i=0x00;
	while(i!=0x00)
	{
		sdss();
		// proba inicjalizacji sd
		Wyslij_bajt_SPI(SD_APP_CMD);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		i=Wyslij_bajt_SPI(0xFF);

		//	pusta pêtla czekania na odpowiedŸ
		while(i==0xFF)
		{
			i=Wyslij_bajt_SPI(0xFF);
		}

		sdus();
		Wyslij_bajt_SPI(0xFF);

		
		//	sprawdzenie odpowiedzi karty
		//return (0x4200 +i);
		if ((i==0x05)|(i==0x04))
		{
			//	karta MMC
			break;
		}
		else
		{
			sdss();
			// proba inicjalizacji sd
			Wyslij_bajt_SPI(SD_INI_SDC);
			Wyslij_bajt_SPI(0);
			Wyslij_bajt_SPI(0);
			Wyslij_bajt_SPI(0);
			Wyslij_bajt_SPI(0);
			i=Wyslij_bajt_SPI(0xFF);

			//	pusta pêtla czekania na odpowiedŸ
			while(i==0xFF)
			{
				i=Wyslij_bajt_SPI(0xFF);
			}
			
			sdus();
			Wyslij_bajt_SPI(0xFF);
		}
	}
	

	// inicjalizacja karty MMC
	i=0xff;
	while(i!=0x00)
	{
		sdss();
		// proba inicjalizacji sd
		Wyslij_bajt_SPI(SD_SEND_OP_COND);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0xFF);

		//	pusta pêtla czekania na odpowiedŸ
		while(i==0xFF)
		{
			i=Wyslij_bajt_SPI(0xFF);
		}
		sdus();

		Wyslij_bajt_SPI(0xFF);
	}

	//return (0x4300 +i);
	i=0xff;
	while(i!=0x00)
	{
		sdss();
		// proba inicjalizacji sd
		Wyslij_bajt_SPI(SD_SET_BLOCKLEN);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0x02);
		Wyslij_bajt_SPI(0);
		Wyslij_bajt_SPI(0xFF);

		//	pusta pêtla czekania na odpowiedŸ
		while(i==0xFF)
		{
			i=Wyslij_bajt_SPI(0xFF);
		}
		sdus();

		Wyslij_bajt_SPI(0xFF);
	}
	
	return (0x4400 +i);
 }


 uint16_t Odczyt_danych_z_SD(uint8_t* adres_odczytu, char* adres_tablicy)
 {	
	uint16_t l;
	uint8_t i;
	Wyslij_bajt_SPI(0xFF);
	sdss();
	Wyslij_bajt_SPI(SD_READ_SINGLE_BLOCK);
	for(i=0;i<4;i++)
	{
		Wyslij_bajt_SPI(*(adres_odczytu+i));
	}
	i = Wyslij_bajt_SPI(0xff);

	while (i==0xff)
	{	
		i = Wyslij_bajt_SPI(0xff);
	}
	
	if (i!=0x00)
	{
		return (0x6100 +i);	//	zwrucenie miejsca i kodu bledu
	}
	i = Wyslij_bajt_SPI(0xff);
	while (i==0xff)
	{
		i = Wyslij_bajt_SPI(0xff);
	}

	//	sprawdzenie pocz¹tkowego tokenu
	if (i!=0xfe)
	{
		return (0x6200 +i);	//	zwrucenie miejsca i kodu bledu
	}
	for(l=0;l<512;l++)
	{
		(* (adres_tablicy+l)) =Wyslij_bajt_SPI(0xff);
	}
	Wyslij_bajt_SPI(0xff);
	Wyslij_bajt_SPI(0xff);
	sdus();
	Wyslij_bajt_SPI(0xff);
	return	(0x6300 );	//	koniec odczytu
 }