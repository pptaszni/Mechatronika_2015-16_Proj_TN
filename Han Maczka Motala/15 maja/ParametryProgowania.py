#Program pozwalajacy na ustawienie odpowiedniej wartosci progowania
import numpy
import cv2
import pygame

#Inicjalizacja wyswietlania obrazu
pygame.init()
screenSize = (640,320)
pygame.mouse.set_visible(0)
screen = pygame.display.set_mode(screenSize)

#Wybor numeru zarejestrowanego filmu
NUMER_FILMU = 3

#Otwarcie filmu testowego w formacie mp4
cap = cv2.VideoCapture('/home/pi/Desktop/test'+str(NUMER_FILMU)+'.mp4')
while not cap.isOpened():
    cap = cv2.VideoCapture('/home/pi/Desktop/test'+str(NUMER_FILMU)+'.mp4');
    cv2.waitKey(1000);
    print ('Otwieranie...');

#Odczyt filmu klatka po klatce
while True:
    flag, frame = cap.read();
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY);
    ret,prog = cv2.threshold(gray,10,255,cv2.THRESH_BINARY);
    
    wysw = numpy.rot90(prog);
    wysw = numpy.flipud(wysw);
    wysw = pygame.surfarray.make_surface(wysw);
    screen.blit(wysw, (0,0));
    pygame.display.update();
    
    pos_frame = cap.get(cv2.cv.CV_CAP_PROP_POS_FRAMES);
    print ('Numer klatki: '+str(pos_frame));

    if cv2.waitKey(10) == 27:
        break;

    #Przerwij, gdy obecna klatka jest ostatnia klatka filmu
    if cap.get(cv2.cv.CV_CAP_PROP_POS_FRAMES) == cap.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT):
        break;
