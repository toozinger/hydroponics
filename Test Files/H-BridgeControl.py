# This code created to run the L298N H bridge
# Put jumper over enable pins (sideways pins)
# Else, use PWM on those pins
# IN1 - IN4 are set high to make motor x go direction y
# 12v is where power goes in. Remember common grounds

# Import required libraries
import sys
import time
import RPi.GPIO as GPIO

# Use BCM GPIO reference (physical pin #)
mode=GPIO.getmode()

# Define motor pins
StepPinForward=38
StepPinBackward=40
sleeptime=1

# Set GPIO pins as out
GPIO.setmode(GPIO.BOARD)
GPIO.setup(StepPinForward, GPIO.OUT)
GPIO.setup(StepPinBackward, GPIO.OUT)

# Simple def to go forward
def forward(x):
    GPIO.output(StepPinForward, GPIO.HIGH)
    print "forwarding running  motor "
    time.sleep(x)
    GPIO.output(StepPinForward, GPIO.LOW)

def reverse(x):
    GPIO.output(StepPinBackward, GPIO.HIGH)
    print "backwarding running motor"
    time.sleep(x)
    GPIO.output(StepPinBackward, GPIO.LOW)

# Flip flop 1000 times
for i in range(100):
    print "forward motor "
    forward(2)
    print "reverse motor"
    reverse(2)

print "Stopping motor"
GPIO.cleanup()
