/* 
 * Kod programu do Arduino Uno do sterowania pozycją spojlera.
 */

/*---------------------------*/
#include <Servo.h>
#define TESTY 1

/*PINY, aliasy*/
/*---------------------------*/
#define stycznik1 7
#define stycznik2 4
#define stycznik3 5
#define enkoderA 2
#define przycisk 3
#define serwo1 9

#define can_predkosc 0x0D

#define predkoscjazdyszbkiej 120
#define predkoscdzialaniahamulcaaero 90

#define pozycjaneutralna 10*2
#define pozycjadojazdyszybkiej 0
#define pozycjazwiekszonegodocisku 15*2

#define slabehamowanie 30*2
#define sredniehamowanie 60*2
#define mocnehamowanie 90*2
/*---------------------------*/

/*ZMIENNE*/
/*---------------------------*/
//zmienne powiazane z CAN
long unsigned int rxId=0x0D; //PID predkosci
unsigned char rxBuf[8];
//pozostale zmienne
Servo Serwo1;
unsigned char aktualnaPozycja=0;
unsigned char staraPozycja=0;
unsigned char koncowaPozycja=0;
unsigned char jakMocnoHamujemy;
volatile unsigned char stanEnkodera;
bool docisk=0;
enum silaHamowania{
  wogole=0,
  troche=30,
  srednio=60,
  wgniatamywpodloge=90
};
enum pozycjaSpojlera{
  neutralna=0b0,
  doJazdySzybkiej=0b11,
  hamowanie=0b1100
};
/*---------------------------*/

/*FUNKCJE*/
/*---------------------------*/
void zwiekszonyDocisk(){
  delay(1);
  if(!digitalRead(przycisk)) ustawDocisk();
}
bool ustawDocisk(){
  delay(1);
  if(digitalRead(przycisk)) return 1;
  if(!docisk){
    docisk=1;
    return 0;
  }
  else if(docisk){
    docisk=0;
    return 0;
  }
}
bool sprawdzJakMocnoHamujemy(){
  jakMocnoHamujemy=wogole;
  if(digitalRead(stycznik1)) return 0;   
  jakMocnoHamujemy=troche;    
  if(digitalRead(stycznik2)) return 0;
  jakMocnoHamujemy=srednio;
  if(digitalRead(stycznik3)) return 0;
  jakMocnoHamujemy=wgniatamywpodloge;
  return 0;
}
bool sterowanieSpojlerem(){
  if(docisk){
    Serwo1.write(pozycjazwiekszonegodocisku);
    return 0;
  }
  if(rxId==can_predkosc && jakMocnoHamujemy==wogole){//jesli okreslona predkosc i nie hamujemy
    if(rxBuf[0]>predkoscjazdyszbkiej){          //jezeli predkosc >120 km/h
      ustawSpojler(doJazdySzybkiej);
      return 0;
    }
    else if (rxBuf[0]<=predkoscjazdyszbkiej){   //jezeli predkosc <=120 km/h
      ustawSpojler(neutralna);
      return 0;
    }
  }
  else if(rxId==can_predkosc && rxBuf[0]>predkoscdzialaniahamulcaaero && !(jakMocnoHamujemy==wogole)){//jesli predkosc>90 i wcisniety pedal hamulca
    ustawSpojler(hamowanie);
    return 0;
  }
  else if(rxId!=can_predkosc){//jesli predkosc nie jest znana (blad komunikacji, brak odczytu)
    ustawSpojler(neutralna);
    return 1;
  } 
  return 0; 
}
bool ustawSpojler(pozycjaSpojlera poz){
  if(poz==neutralna){
    Serwo1.write(pozycjaneutralna);
    return 0;
  }
  if(poz==doJazdySzybkiej){
    Serwo1.write(pozycjadojazdyszybkiej);
    return 0;
  }
  if(poz==hamowanie){
    if(jakMocnoHamujemy==troche){
      Serwo1.write(slabehamowanie);
      return 0;
    }
    else if(jakMocnoHamujemy==srednio){
      Serwo1.write(sredniehamowanie);
      return 0;
    }
    else if(jakMocnoHamujemy==wgniatamywpodloge){
      Serwo1.write(mocnehamowanie);
      return 0;
    }  
  }
  else{
    ustawSpojler(neutralna);
    return 1;
  }
}
/*---------------------------*/

/*---------------------------*/
void setup(){
  Serial.begin(115200);
#if TESTY==1
  Serial.println("\tTO SA TESTY! ");
#endif
#if TESTY==0
  Serial.println("\tTESTY=0 "); 
#endif
  Serwo1.attach(9);
  pinMode(stycznik1,INPUT_PULLUP); //stycznik1
  pinMode(stycznik2,INPUT_PULLUP); //stycznik2
  pinMode(stycznik3,INPUT_PULLUP); //stycznik3
  pinMode(przycisk,INPUT_PULLUP); //enkoder pin B
  pinMode(serwo1,OUTPUT); //serwo
  attachInterrupt(digitalPinToInterrupt(przycisk), zwiekszonyDocisk, FALLING);
}
void loop(){
  if(!docisk){//jesli docisk==1 to pozycja spojlera stala i rowna 15st.
    sprawdzJakMocnoHamujemy();
  }
  sterowanieSpojlerem();
#if TESTY==1
  static unsigned long i=0;
  if(!(i%5000)){
    Serial.print("predkosc: "); Serial.println(rxBuf[0]);
    Serial.print("jakMocnoHamujemy: "); Serial.println(jakMocnoHamujemy,DEC);
    Serial.print("sygnal na serwo1: "); Serial.println(Serwo1.read());
    Serial.print("docisk: "); Serial.println(docisk);
    i=0;
  }
  i++;
#endif
}
void serialEvent(){//wartosc predkosci 0-255 wpisuje sie w konsoli, "symulacja" dzialania MCP2515
  while (Serial.available()>0){
    rxBuf[0]=Serial.parseInt();
  }
  Serial.flush();
}

