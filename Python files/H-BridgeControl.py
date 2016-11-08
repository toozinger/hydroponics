# Import required libraries

import sys
import time
import RPi.GPIO as GPIO

# Use BCM GPIO references
# instead of physical pin numbers
#GPIO.setmode(GPIO.BCM)
mode=GPIO.getmode()
print " mode ="+str(mode)
#GPIO.cleanup()

# Define GPIO signals to use
# Physical pins 11,15,16,18
# GPIO17,GPIO22,GPIO23,GPIO24

StepPinForward=16
StepPinBackward=18
goPin = 11
sleeptime=1

GPIO.setmode(GPIO.BOARD)
GPIO.setup(StepPinForward, GPIO.OUT)
GPIO.setup(StepPinBackward, GPIO.OUT)
GPIO.setup(goPin, GPIO.OUT)


def forward(x):
    GPIO.output(goPin, GPIO.HIGH)
    GPIO.output(StepPinForward, GPIO.HIGH)
    GPIO.output(StepPinBackward, GPIO.LOW)
    print "forwarding running  motor "
    time.sleep(x)
    GPIO.output(StepPinForward, GPIO.LOW)
    GPIO.output(goPin, GPIO.LOW)

def reverse(x):
    GPIO.output(goPin, GPIO.HIGH)
    GPIO.output(StepPinBackward, GPIO.HIGH)
    GPIO.output(StepPinForward, GPIO.LOW)
    print "backwarding running motor"
    time.sleep(x)
    GPIO.output(StepPinBackward, GPIO.LOW)
    GPIO.output(goPin, GPIO.LOW)

for i in range(2):
    print "forward motor "
    forward(2)
    print "reverse motor"
    reverse(2)

print "Stopping motor"
GPIO.cleanup()
