#Testowanie dzialania czujnikow
import RPi.GPIO as GPIO
import Adafruit_DHT

GPIO.setmode(GPIO.BCM)

#Ustawienie pinow
pinWilg = 4
pinOpady = 17

GPIO.setup(pinOpady, GPIO.IN, pull_up_down=GPIO.PUD_UP)
#Po wykryciu opadow przechodzi ze stanu wysokiego na niski
#Regulowane potencjometrem

wilgotnosc, temperatura = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, pinWilg)

if wilgotnosc is not None and temperatura is not None:
    print('Temp={0:0.1f}*C  Wilg={1:0.1f}%'.format(temperatura,wilgotnosc))
else:
    print('Blad odczytu.')

if GPIO.input(pinOpady) == 1:
    print('Nie wykryto opadow.');
else:
    print('Wykryto opady.');
