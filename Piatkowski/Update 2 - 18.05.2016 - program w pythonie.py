from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
 
# inicjalizacja kamery, referencja do przechwyconego obrazu
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))
 
# chwila na rozgrzanie kamerki
time.sleep(0.1)
 
# przyechwytywanie klatek
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):

    image = frame.array

    #parametry bloba
    params=cv2.SimpleBlobDetector_Params()

        #params.minThreshold = 150
        #params.maxThreshold = 255
    params.filterByColor = True
    params.blobColor=255

    #detekcja bloba
    detector = cv2.SimpleBlobDetector_create(params)
    keypoints = detector.detect(image)
    im_with_keypoints = cv2.drawKeypoints(image, keypoints, image, (0,0,255) , cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    surf = cv2.xfeatures2d.SURF_create(400)
    kp, des = surf.detectAndCompute(image,None)

# wywietlanie klatek
    cv2.imshow("Keypoints", im_with_keypoints)
    key = cv2.waitKey(1) & 0xFF
 
# wyczyszczenie stumienia dla nastpnej klatki
    rawCapture.truncate(0)

    
 
# nacisnij q aby zakonczyc
    if key == ord("q"):
	    break