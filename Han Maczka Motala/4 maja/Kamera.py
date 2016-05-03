import io
import time
import picamera
import pygame

import numpy
import cv2

SZEROKOSC = 640
WYSOKOSC = 320

#INICJALIZACJA WYSWIETLACZA OBRAZU - zostanie usuniete przy finalnej implementacji
pygame.init()
rozmiar = (SZEROKOSC,WYSOKOSC)
pygame.mouse.set_visible(0)
ekran = pygame.display.set_mode(rozmiar)

def zdjecia():
    k = 0
    start = time.time()
    stream = io.BytesIO()
    while (1): #Glowna petla pozwalajaca na odczyt kazdego zdjecia z sekwencji
        yield stream
        stream.seek(0)

        #OPERACJE NA ZDJECIU
        tab = numpy.fromstring(stream.getvalue(), dtype=numpy.uint8)
        im = cv2.imdecode(tab,0) #Dekodowanie wraz z przetworzeniem na skale szarosci
        ret,prog = cv2.threshold(im,10,255,cv2.THRESH_BINARY)

        ile_bialych = cv2.countNonZero(prog)

        #WYSWIETLANIE OBRAZU - zostanie usuniete przy finalnej implementacji
        wysw = numpy.rot90(prog) #Poprawienie ulozenia obrazu na ekranie (rotacja i odbicie)
        wysw = numpy.flipud(wysw)
        wysw = pygame.surfarray.make_surface(wysw)
        ekran.blit(wysw, (0,0))
        pygame.display.update()

        #TEST FPS
        if k == 200:
            koniec = time.time()
            print('Wykonano 200 zdjec: %.2ffps' % (200/(koniec - start)))
            print('Ilosc bialych pikseli: %d' % ile_bialych)
            start = time.time()
            k = 0
        k = k+1
        stream.truncate()

with picamera.PiCamera() as kamera:
    kamera.resolution = (SZEROKOSC, WYSOKOSC)
    kamera.framerate = 90
    kamera.capture_sequence(zdjecia(), format='jpeg', use_video_port=True)
