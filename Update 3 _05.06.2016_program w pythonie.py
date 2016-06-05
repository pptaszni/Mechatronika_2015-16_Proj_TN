from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
import cPickle as pickle
import sys
#import RPi.GPIO as GPIO

#GPIO.setmode(GPIO.BOARD) #uzycie numeracji  pinow wg raspberry

def blob_coordinates(keypoints):
    i = 0
    temp_array = []
    for point in keypoints:
        temp = (point.pt[0], point.pt[1])     
        temp_array.append(temp)
        if (point.pt[0]<160):  print "zgas lewe swiatlo drogowe"
        if (120<point.pt[0]<240): print "zgas obydwa swiatla drogowe"
        if (240<point.pt[0]): print "zgas prawe swiatlo drogowe"
            #if (point.pt[1]<120):   print "LEWO_GORA"
            #else:            print "LEWO_DOL"
        #if(point.pt[0]>160):        
            #if (point.pt[1]<120):   print "PRAWO_GORA"
            #else:            print "PRAWO_DOL"
    return temp_array


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
    # grab the raw NumPy array representing the image, then initialize the timestamp
    # and occupied/unoccupied text
    image = frame.array

    #parametry bloba
    params=cv2.SimpleBlobDetector_Params()

        #params.minThreshold = 150
        #params.maxThreshold = 255
    params.filterByColor = True
    params.blobColor=255

    #filtracja po powierzchni
    params.filterByArea = True
    params.minArea = 200
    params.maxArea = 7500

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

    
    temp_array = []
    temp = blob_coordinates(keypoints)
    temp_array.append(temp)
    
    for i in temp_array:
        print i
    
    # wywietlanie klatek
    cv2.imshow("Keypoints", im_with_keypoints)
    key = cv2.waitKey(1) & 0xFF
 
    # wyczyszczenie stumienia dla nastpnej klatki
    rawCapture.truncate(0)
    


    
 
    # nacisnij q aby zakonczyc
    if key == ord("q"):
	    break

