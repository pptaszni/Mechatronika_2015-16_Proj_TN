//predkosc: rxId(0D),rxBuf[0](1bajt - 0-255)
//zmodyfikowany przyklad "receive" z biblioteki
//nasz modul to MCP2515+TJA1050, kwarc 8MHz

#include "MCP2515_lib-master/mcp_can.h"
#include <SPI.h>

long unsigned int rxId; //(=INT32U)
unsigned char len = 0;
unsigned char rxBuf[8]; //predkosc pojazdu to tylko 1 bajt 0-255, wiec wystarczylaby zwykla zmienna

MCP_CAN CAN(10);                               // Set CS (slave select) to pin 10

void setup()
{
  Serial.begin(115200);
  CAN.begin(CAN_500KBPS,MCP_8MHz);  //metoda zmodyfikowana o drugi parametr z wyborem częstotliwości, bo na naszej płytce jest kwarc 8MHz, a biblioteka oryginalnie była pod 16 MHz
  CAN.init_Mask(0, 0, 0xFFFFFFF); //CAN 11b, jesli 29b - to drugi argument=1
  CAN.init_Mask(1, 0, 0xFFFFFFF);
  CAN.init_Filt(0, 0, 0x0D); //tylko predkosc, 0=rejestr MCP_RXF0
  Serial.println("Dzien dobry");
  pinMode(2, INPUT); //wyjście INT modułu (przerwanie)
  delay(500);
}

void loop()
{
    if(!digitalRead(2))                         // If pin 2 is low, read receive buffer
    {
      CAN.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
      rxId = CAN.getCanId();                    // Get message ID
      //w tym momencie mamy juz PID i odpowiadajaca mu wartosc, wiec znamy predkosc lub inne wybrane parametry
      Serial.print("ID: ");
      Serial.print(rxId, HEX);
      Serial.print("  Data: ");
      for(int i = 0; i<len; i++)                // Print each byte of the data
      {
        if(rxBuf[i] < 0x10)                     // If data byte is less than 0x10, add a leading zero
        {
          Serial.print("0");
        }
        Serial.print(rxBuf[i], DEC);
        Serial.print(" ");
      }
      Serial.println();
    }
}
