//piny stycznikow: 3,4,5. Styczniki podpiete jako NO
//enkoder: pin 2
//PWM do sterowania silownikiem/ serwmomechanizmem - pin 
//na razie zalozenie, ze:
//                    sterownik silownika jest sterowany analogowo napieciem 0-5 V
//                      0V pozycja najnizsza/ do szybkiej jazdy (PWM 0)
//                      5V - maksymalne hamowanie (PWM 255)
//                    za pinem 6 w takim przypadku nalezaloby dać filtr dolnoprzepustowy, parametry 4kOm, >=10uF (czestotliwosc na pinie 6 to 977 Hz)

/*---------------------------*/

/*ZMIENNE*/
/*---------------------------*/
long unsigned int rxId=0x0D; //PID predkosci
unsigned char rxBuf[8];
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
}
void enkoderVSpwm(){//sprawdzenie czy pozycja odczytana na podst. enkodera zgadza sie z wynikajaca z wymuszenia przez napiecie
                    //bedzie wywolywane w ustawSpojler() po analogWrite()
  //...
}
//void korygujPozycjeSpojlera()//funkcja majaca korygowac ewentualny blad-wynik funkcji enkoderVSpwm()
void sprawdzJakMocnoHamujemy(){
  jakMocnoHamujemy=wogole;
  if(!digitalRead(3)){        //NO, pullup i drugi pin podlaczony do GND, wiec gdy wcisniety, to pojawia sie stan niski
    jakMocnoHamujemy=troche;
    if(!digitalRead(4)){
      jakMocnoHamujemy=srednio;
      if(!digitalRead(5)){
        jakMocnoHamujemy=wgniatamywpodloge;
      }
    }
  }  
}
void sterowanieSpojlerem(){
  if(rxId==0x0D && jakMocnoHamujemy==wogole){//jesli okreslona predkosc i nie hamujemy
    if(rxBuf[0]>90){          //jezeli predkosc >90 km/h
      ustawSpojler(doJazdySzybkiej);
    }
    else if (rxBuf[0]<=90){   //jezeli predkosc <=90 km/h
      ustawSpojler(neutralna);
    }
  }
  else if(rxId==0x0D && !(jakMocnoHamujemy==wogole)){//jesli okreslona predkosc i wcisniety pedal hamulca
    ustawSpojler(hamowanie);
  }
  else{//jesli predkosc nie jest znana (blad komunikacji, brak odczytu)
    ustawSpojler(neutralna);
  }  
}
void ustawSpojler(pozycjaSpojlera poz){
  if(poz==neutralna){
    analogWrite(6,10);
  }
  else if(poz==doJazdySzybkiej){
    analogWrite(6,50);
  }
  else if(poz==hamowanie){
    if(jakMocnoHamujemy==troche){
      analogWrite(6,90);
    }
    else if(jakMocnoHamujemy==srednio){
      if(rxBuf[0]<=90){//rozne pozycje spojlera przy takim samym hamowaniu przy wiekszej i mniejszej predkosci
        analogWrite(6,180);
      }
      else if(rxBuf[0]>90){
        analogWrite(6,180);
      }
      else{
        ustawSpojler(neutralna);
      }
    }
    else if(jakMocnoHamujemy==wgniatamywpodloge){
      if(rxBuf[0]<=90){
        analogWrite(6,255);
      }
      else if(rxBuf[0]>90){
        analogWrite(6,255);
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
  pinMode(3,INPUT_PULLUP); //stycznik1
  pinMode(4,INPUT_PULLUP); //stycznik2
  pinMode(5,INPUT_PULLUP); //stycznik3
  pinMode(2,INPUT_PULLUP); //enkoder
  pinMode(6,OUTPUT); //sygnal PWM sterujacy
  attachInterrupt(digitalPinToInterrupt(2), liczEnkoder, RISING);//funkcja liczEnkoder() wywolywana przez zewn. przerwanie na pinie 2
}
void loop(){
  sprawdzJakMocnoHamujemy();
  sterowanieSpojlerem();
  delay(100);
}
void serialEvent(){//wartosc predkosci 0-255 wpisuje sie w konsoli, "symulacja" dzialania MCP2515
  while (Serial.available()>0){
    rxBuf[0]=Serial.parseInt();
  }
  Serial.flush();
}

