from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
import cPickle as pickle
import sys
import RPi.GPIO as GPIO

#definicje pinow
Lewe_mijania=2
Prawe_mijania=3
Lewe_drogowe=4
Prawe_drogowe=17
#ustawienie wyjsc na gpio
GPIO.setmode(GPIO.BCM) #uzycie numeracji  pinow wg raspberry
GPIO.setup(Lewe_mijania,GPIO.OUT)
GPIO.setup(Prawe_mijania,GPIO.OUT)
GPIO.setup(Lewe_drogowe,GPIO.OUT)
GPIO.setup(Prawe_drogowe,GPIO.OUT)

#ustawienie swiatel dziennych na on caly czas
GPIO.output(Lewe_mijania,GPIO.HIGH)
GPIO.output(Prawe_mijania,GPIO.HIGH)

def automatic_high_beam(keypoints):

    GPIO.output(Prawe_drogowe,GPIO.HIGH)
    GPIO.output(Lewe_drogowe,GPIO.HIGH)
    
    for point in keypoints:
        
        if (point.pt[0]<120):
            GPIO.output(Prawe_drogowe,GPIO.HIGH)
            GPIO.output(Lewe_drogowe,GPIO.LOW)
            print "zgas lewe swiatlo drogowe"     
    
        elif (120<point.pt[0]<220):
            GPIO.output(Prawe_drogowe,GPIO.LOW)
            GPIO.output(Lewe_drogowe,GPIO.LOW)     
            print "zgas obydwa swiatla drogowe"
        
        elif (220<point.pt[0]):
            GPIO.output(Prawe_drogowe,GPIO.LOW)
            GPIO.output(Lewe_drogowe,GPIO.HIGH)
            print "zgas prawe swiatlo drogowe"

# inicjalizacja kamery, referencja do przechwyconego obrazu

camera = PiCamera()
camera.resolution = (320, 240)
camera.framerate = 40
camera.color_effects = (128, 128)
rawCapture = PiRGBArray(camera, size=(320, 240))

# chwila na rozgrzanie kamerki
time.sleep(0.1)
 
# przyechwytywanie klatek
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    image = frame.array

    #parametry bloba
    params=cv2.SimpleBlobDetector_Params()
    params.filterByColor = True
    params.blobColor=255

    #filtracja po powierzchni
    params.filterByArea = True
    params.minArea = 50
    params.maxArea = 10000

    #filtracja po ksztalcie (okraglosc)
    params.filterByInertia
    params.minInertiaRatio=0.1

    
    #filtracja po okregu
    params.filterByCircularity= True
    params.minCircularity = 0.1
    
    #detekcja bloba

    detector = cv2.SimpleBlobDetector_create(params)
    keypoints = detector.detect(image)
    im_with_keypoints = cv2.drawKeypoints(image, keypoints, image, (0,0,255) , cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    #uruchomienie automatyczych swiatel drogowych
    automatic_high_beam(keypoints)
        
    # wywietlanie klatek
    cv2.imshow("Keypoints", im_with_keypoints)
    key = cv2.waitKey(1) & 0xFF
 
    # wyczyszczenie stumienia dla nastpnej klatki
    rawCapture.truncate(0)
    


    
 
    # nacisnij q aby zakonczyc
    if key == ord("q"):
	    break

