#!/usr/bin/python
import io
import time
import picamera
import pygame
import numpy
import cv2
import spidev
import RPi.GPIO as GPIO
#import Adafruit_DHT

SZEROKOSC = 640
WYSOKOSC = 640

pygame.init()
screenSize = (SZEROKOSC,WYSOKOSC)
pygame.mouse.set_visible(0)
screen = pygame.display.set_mode(screenSize)

#Ustaw xhost +

##
def OtwarcieFilmu():
    NUMER_FILMU = 1
    cap = cv2.VideoCapture('/home/pi/Desktop/test'+str(NUMER_FILMU)+'.mp4')
    while not cap.isOpened():
        cap = cv2.VideoCapture('/home/pi/Desktop/test'+str(NUMER_FILMU)+'.mp4');
        cv2.waitKey(1000);
        print ('Otwieranie...');
    return cap
##

#Funkcja odczytujaca wartosc z fotorezystora przez SPI, podpietego pod ch0 ADC
#Wartosc zwracana to 0-4095 (12-bitowy), gdzie 0 oznacza calkowita ciemnosc
def readadc0():
    r = spi.xfer2([6, 0, 0])
    adcout = ((r[1] & 15) << 8) + r[2]
    return adcout

cap = OtwarcieFilmu()
stan_drogowe = 0
stan_mijania = 0

#Konfiguracja GPIO
GPIO.setmode(GPIO.BCM)

#Ustawienie nazw pinow wyj
ZLE_WARUNKI = 5
DZIENNE = 6
DROGOWE = 13
MIJANIA = 19
#...

#Ustawienie nazw pinow wej
ZALACZ_DROGOWE = 12
ZALACZ_MIJANIA = 16
#PIN_WILG = 4
PIN_OPADY = 17
#...

#Ustawienia wej/wyj
GPIO.setup(ZLE_WARUNKI, GPIO.OUT)
GPIO.setup(DZIENNE, GPIO.OUT)
GPIO.setup(DROGOWE, GPIO.OUT)
GPIO.setup(MIJANIA, GPIO.OUT)
GPIO.setup(ZALACZ_MIJANIA, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(ZALACZ_DROGOWE, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #Sygnal informujacy o aktywowaniu systemu swiatel drogowych przez kierowce
GPIO.setup(PIN_OPADY, GPIO.IN, pull_up_down=GPIO.PUD_UP) #Po wykryciu opadow przechodzi ze stanu wysokiego na niski, regulowane potencjometrem

GPIO.output(DZIENNE, 0)
GPIO.output(MIJANIA, 0)
GPIO.output(DROGOWE, 0)
GPIO.output(ZLE_WARUNKI, 0)
#...

#Aktywowanie SPI dla ADC
spi = spidev.SpiDev()
spi.open(0, 0)

#Funkcja odczytujaca wartosc z fotorezystora przez SPI, podpietego pod ch0 ADC
#Wartosc zwracana to 0-4095 (12-bitowy), gdzie 0 oznacza calkowita ciemnosc
def readadc0():
    r = spi.xfer2([6, 0, 0])
    adcout = ((r[1] & 15) << 8) + r[2]
    return adcout

#GLOWNY PROGRAM
try:
    while True:        
        #Odczytanie wartosci z czujnikow
        fotorezystor = readadc0()
        #wilgotnosc, temperatura = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, PIN_WILG)
        opady = GPIO.input(PIN_OPADY)

        if GPIO.input(ZALACZ_DROGOWE) == 1:
            if stan_drogowe == 0:
                stan_drogowe = 1
            else:
                stan_drogowe = 0
            time.sleep(0.2)

        if GPIO.input(ZALACZ_MIJANIA) == 1:
            if stan_mijania == 0:
                stan_mijania = 1
            else:
                stan_mijania = 0
            time.sleep(0.2)

        if stan_drogowe == 1:
            if fotorezystor > 200: #Sprawdzenie, czy kierowca wlaczyl swiatla drogowe i czy jest jasno
                GPIO.output(DZIENNE, 0)
                GPIO.output(MIJANIA, 1)
                GPIO.output(DROGOWE, 1)
            else:
                #Jezeli kierowca ustawil na drogowe i jasnosc spadla ponizej okreslonego poziomu, to wejdz do petli i uruchom odczyt z kamery   
                ##
                while True:
                    fotorezystor = readadc0()
                    #wilgotnosc, temperatura = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, PIN_WILG)
                    opady = GPIO.input(PIN_OPADY)

                    if GPIO.input(ZALACZ_DROGOWE) == 1:
                        if stan_drogowe == 0:
                            stan_drogowe = 1
                        else:
                            stan_drogowe = 0
                        time.sleep(0.2)

                    #Jezeli kierowca wylaczyl swiatla drogowe lub jasnosc wzrosla powyzej okreslonego progu, to wyjdz z petli kamery
                    if stan_drogowe == 0 or fotorezystor > 200:
                        break

                    flag, frame = cap.read(); #Odczytanie pojedynczej klatki filmu

                    #Jezeli wilgotnosc ponad 50% lub wykryto opady, to wlacz diode ostrzegajaca
                    #if wilgotnosc >= 50 or opady == 0:
                    if opady == 0:
                        GPIO.output(ZLE_WARUNKI, 1)
                    else:
                        GPIO.output(ZLE_WARUNKI, 0)
                    
                    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY);
                    ret,prog = cv2.threshold(gray,230,255,cv2.THRESH_BINARY); #Do ustawienia wartosc progowania (obecnie 100)

                    wysw1 = numpy.rot90(prog)
                    wysw2 = numpy.rot90(frame)
                    wysw1 = numpy.flipud(wysw1)
                    wysw2 = numpy.flipud(wysw2)

                    wyc = wysw1[0:640,160:320]
                    ile_bialych = cv2.countNonZero(wyc)
                    
                    wysw1 = pygame.surfarray.make_surface(wysw1)
                    wysw2 = pygame.surfarray.make_surface(wysw2)
                    screen.blit (wysw1, (0,0))
                    screen.blit (wysw2, (0,320))
                    pygame.display.update()

                    #Sprawdzone wczesniej, ze kierowca ma wlaczone drogowe i nie jest za jasno
                    if ile_bialych <= 100: #Kiedy malo jasnych pikseli, to znaczy, ze nie wykryto swiatel (wlacz drogowe i mijania, wylacz dzienne)
                        GPIO.output(DROGOWE, 1)
                        GPIO.output(DZIENNE, 0)
                        GPIO.output(MIJANIA, 1)
                    else: #Jesli kamera wykryla, ze jest duzo bialych pikseli, to zalacz tylko mijania
                        GPIO.output(DROGOWE, 0)
                        GPIO.output(DZIENNE, 0)
                        GPIO.output(MIJANIA, 1)
                        for i in range(0, 40):
                            flag, frame = cap.read()
                            cv2.waitKey(10)
                        #time.sleep(1) #Ograniczenie czasu minimalnego przygaszenia drogowych = 1 sekunda

                    cv2.waitKey(10) #Zatrzymanie klatki
                        
                    #Gdy obecna klatka jest ostatnia klatka filmu, otworz ponownie
                    if cap.get(cv2.cv.CV_CAP_PROP_POS_FRAMES) == cap.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT):
                        cap = OtwarcieFilmu()
                ##
        else:
            if stan_mijania == 1: #Kierowca wlacza swiatla mijania
                GPIO.output(DZIENNE, 0)
                GPIO.output(MIJANIA, 1)
                GPIO.output(DROGOWE, 0)
            else: #Kierowca wylacza swiatla mijania, system przechodzi w tryb automatyczny
                if fotorezystor >= 1600: #Za jasno na mijania, wlaczane sa swiatla dzienne zaraz po uruchomieniu silnika
                    GPIO.output(DZIENNE, 1)
                    GPIO.output(MIJANIA, 0)
                    GPIO.output(DROGOWE, 0)
                else: #Wlacza swiatla mijania, bo jest ciemno
                    GPIO.output(DZIENNE, 0)
                    GPIO.output(MIJANIA, 1)
                    GPIO.output(DROGOWE, 0)
                    #Pomijanie 1 sekundy filmu

        #Jezeli wilgotnosc ponad 50% lub wykryto opady, to wlacz diode ostrzegajaca
        #if wilgotnosc >= 50 or opady == 0:
        if opady == 0:
            GPIO.output(ZLE_WARUNKI, 1)
        else:
            GPIO.output(ZLE_WARUNKI, 0)
except KeyboardInterrupt: #Zamkniecie programu Ctrl+C     
    GPIO.cleanup()
            
