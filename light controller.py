# This code created to run the L298N H bridge
# Put jumper over enable pins (sideways pins)
# Else, use PWM on those pins
# IN1 - IN4 are set high to make motor x go direction y
# 12v is where power goes in. Remember common grounds

# Import required libraries
import sys
import time
import RPi.GPIO as GPIO
import urllib2

# Use BCM GPIO reference (physical pin #)
mode=GPIO.getmode()

# Stop showing errors
GPIO.setwarnings(False)

# Define motor pins
waterLight=38
dirtLight=40

# Set GPIO pins as out
GPIO.setmode(GPIO.BOARD)
GPIO.setup(waterLight, GPIO.OUT)
GPIO.setup(dirtLight, GPIO.OUT)

# Gets time from this website
def getTime():
    pageURL = urllib2.urlopen("http://just-the-time.appspot.com")
    timeDate = pageURL.read()

    timeDate = timeDate.replace('-',' ')
    timeDate = timeDate.replace(':',' ')
    time = timeDate.split()

    return time

# Turn lights on
def onLights():
    GPIO.output(waterLight, GPIO.HIGH)
    GPIO.output(dirtLight, GPIO.HIGH)

# Turn lights off
def offLights():
    GPIO.output(waterLight, GPIO.LOW)
    GPIO.output(dirtLight, GPIO.LOW)

# Iterates forever
while 1:

    timeNow = getTime()

    # Keeps lights on for 18 hrs per day
    if int(timeNow[3]) < 6 :
        offLights()
        print "Lights off"
    else:
        onLights()
        print "Lights on"

    # Wait before checking time again
    time.sleep(100)
    
print "Cleaning up"
GPIO.cleanup()
