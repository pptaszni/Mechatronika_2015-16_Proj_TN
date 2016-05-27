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
#define enkoderB 3
#define serwo1 9

#define can_predkosc 0x0D
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
unsigned char koncowaPozycja=0
unsigned char jakMocnoHamujemy;
volatile unsigned char stanEnkodera;
enum silaHamowania{
  wogole=0b0,
  troche=0b11,
  srednio=0b1100,
  wgniatamywpodloge=0b110000
};
enum pozycjaSpojlera{
  neutralna=0b0,
  doJazdySzybkiej=0b11,
  hamowanie=0b1100
};
/*---------------------------*/

/*FUNKCJE*/
/*---------------------------*/
void liczEnkoder(){//liczenie sygnalow z enkodera
    stanEnkodera++;
    if(staraPozycja-koncowaPozycja<0){
      aktualnaPozycja=staraPozycja+stanEnkodera;
    }
    else if(staraPozycja-koncowaPozycja>0){
      aktualnaPozycja=staraPozycja-stanEnkodera;
    }
    else{
      
    }
}
void enkoderVSpwm(){//wysyla do serwa sygnal zalezny od aktualnej pozycji
  if(aktualnaPozycja!=koncowaPozycja){
    if(aktualnaPozycja-koncowaPozycja<0){
      Serwo1.write(93);
    }
    else{
      Serwo1.write(87);
    }
  }
  if(aktualnaPozycja==koncowaPozycja){
    stanEnkodera=0;
    Serwo1.write(90);
    staraPozycja=aktualnaPozycja;
  }  
}
void sprawdzJakMocnoHamujemy(){
  jakMocnoHamujemy=wogole;
  if(!digitalRead(stycznik1)){        //NO, pullup i drugi pin podlaczony do GND, wiec gdy wcisniety, to pojawia sie stan niski
    jakMocnoHamujemy=troche;    
    if(!digitalRead(stycznik2)){
      jakMocnoHamujemy=srednio;
      if(!digitalRead(stycznik3)){
        jakMocnoHamujemy=wgniatamywpodloge;
      }
    }
  }  
}
void sterowanieSpojlerem(){
  if(rxId==can_predkosc && jakMocnoHamujemy==wogole){//jesli okreslona predkosc i nie hamujemy
    if(rxBuf[0]>40){          //jezeli predkosc >40 km/h
      ustawSpojler(doJazdySzybkiej);
    }
    else if (rxBuf[0]<=40){   //jezeli predkosc <=40 km/h
      ustawSpojler(neutralna);
    }
  }
  else if(rxId==can_predkosc && !(jakMocnoHamujemy==wogole)){//jesli okreslona predkosc i wcisniety pedal hamulca
    ustawSpojler(hamowanie);
  }
  else{//jesli predkosc nie jest znana (blad komunikacji, brak odczytu)
    ustawSpojler(neutralna);
  }  
}
void ustawSpojler(pozycjaSpojlera poz){
  if(poz==neutralna){
    koncowaPozycja=20;
  }
  else if(poz==doJazdySzybkiej){
    koncowaPozycja=0;
  }
  else if(poz==hamowanie){
    if(jakMocnoHamujemy==troche){
      koncowaPozycja=(50);
    }
    else if(jakMocnoHamujemy==srednio){
      if(rxBuf[0]<=40){//rozne pozycje spojlera przy takim samym hamowaniu przy wiekszej i mniejszej predkosci
        koncowaPozycja=(20);
      }
      else if(rxBuf[0]>40){
        koncowaPozycja=(70);
      }
      else{
        ustawSpojler(neutralna);
      }
    }
    else if(jakMocnoHamujemy==wgniatamywpodloge){
      if(rxBuf[0]<=40){
        koncowaPozycja=(20);
      }
      else if(rxBuf[0]>40){
        Serwo1.write(90);
      }
      else{
        ustawSpojler(neutralna);
      }
    }  
  }
  else{
    ustawSpojler(neutralna);
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
  pinMode(enkoderA,INPUT_PULLUP); //enkoder pin A
  pinMode(enkoderB,INPUT_PULLUP); //enkoder pin B
  pinMode(serwo1,OUTPUT); //serwo
  attachInterrupt(digitalPinToInterrupt(enkoderA), liczEnkoder, FALLING);//funkcja liczEnkoder() wywolywana przez zewn. przerwanie na pinie 2
}
void loop(){
  sprawdzJakMocnoHamujemy();
  sterowanieSpojlerem();
  enkoderVSpwm();
#if TESTY==1
  static unsigned long i=0;
  if(!(i%5000)){
    Serial.println(i);
    Serial.print("predkosc: "); Serial.println(rxBuf[0]);
    Serial.print("licznik enkodera: "); Serial.println(stanEnkodera);
    Serial.print("jakMocnoHamujemy: "); Serial.println(jakMocnoHamujemy,BIN);
    Serial.print("sygnal na serwo1: "); Serial.println(Serwo1.read());
    Serial.print("koncowa pozycja: "); Serial.println(koncowaPozycja);
    Serial.print("aktualna pozycja: "); Serial.println(aktualnaPozycja);
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

