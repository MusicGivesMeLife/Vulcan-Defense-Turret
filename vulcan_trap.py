import serial
import picamera
from time import sleep
from datetime import datetime
import pygame
import os

inputstr = ''

def RelayCtrl(relay, state):
    inputstr = ""
    ser.write(str(relay))
    ser.write(str(int(state)))
    ser.write("\n")
    while inputstr != 'ACK\n':
        inputstr = ser.readline()
        if inputstr == 'ERR\n':
            print "ERROR!"
            return False
        print inputstr
    return True

def GetPin(in_pin):
    inputstr = ""
    ser.write("?")
    ser.write(str(in_pin))
    ser.write("\n")
    while inputstr != '1\n':
        inputstr = ser.readline()
        if inputstr == '0\n':
            return False
        if inputstr == 'ERR\n':
            print 'ERROR!'
            return False
    return True

def GetAPin(in_pin):
    inputstr = ""
    ser.write("?A")
    ser.write(str(in_pin))
    ser.write("\n")
    if inputstr == 'ERR\n':
    	print 'ERROR!'
    	return False
    return float(inputstr)

def Fire(time=5):
    t = datetime.utcnow()
    pygame.mixer.music.play()
    RelayCtrl(1, True)
    cam.capture("shooting" + str(t.date()) + str(t.time()) + ".jpg")
    sleep(time-1.5)
    RelayCtrl(1, False)





cam = picamera.PiCamera()
cam.brightness = 58
cam.resolution = (2592, 1944)
pygame.mixer.init()
pygame.mixer.music.load("dalek-exterminate.wav")
os.system('amixer set PCM 400')

#Open the serial link to the Arduino
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
print ser

#Verify that the Arduino is ready
c = 0
while inputstr != 'READY\n':
    inputstr = ser.readline()
    if c >= 5:
        c = 0
        ser.write("C\n") #Say "Hey, are you there?!?!"
    else:
        c += 1
    print inputstr

#Ensure motion sensor has warmed up properly
sleep(10)
while GetPin(1):
    sleep(1)

#Check that all relays are off
for r in range(1, 5):
    print "Pulling relay #" + str(r) + " low..."
    RelayCtrl(r, False)
print "All relays off, beginning main loop..."

#Just being OCD that the motion sensor is functioning properly
while GetPin(1):
    sleep(1)

#Main Loop
run = True
i = 0
while run:
    if GetPin(1) == True:
        RelayCtrl(2,True)
        Fire(2)
        print 'F'
        sleep(3)
        RelayCtrl(2,False)
    if i >= 5*60*100:
        print '.'
        i = 0
    else:
        i += 1
    sleep(.01)
    
