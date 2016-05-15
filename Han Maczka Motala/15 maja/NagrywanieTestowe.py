#Program do nagrywania filmu testowego
import picamera
from time import sleep
import os

i = 1

with picamera.PiCamera() as kamera:
    kamera.resolution = (640,320)

    #Sprawdzenie istnienia pliku
    while os.path.isfile('/home/pi/Desktop/test'+str(i)+'.mp4') == 1:
        i += 1;
    
    #Nagranie minuty filmu testowego
    kamera.start_recording('/home/pi/Desktop/test'+str(i)+'.h264')
    kamera.wait_recording(60)
    kamera.stop_recording()

    #Przetworzenie filmu na format mp4 oraz usuniecie h264
    os.system('MP4Box -add /home/pi/Desktop/test'+str(i)+'.h264 /home/pi/Desktop/test'+str(i)+'.mp4')
    os.system('rm /home/pi/Desktop/test'+str(i)+'.h264')

