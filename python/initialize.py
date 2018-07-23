#!/usr/bin/env python

from pixel_ring import pixel_ring
import wiringpi

#configure wiringpi with GPIO numbering
wiringpi.wiringPiSetupGpio()

#this script is responsible for initial setup and initialization of the enclosure's hardware.

#first, disable the LEDs, since they are enable by default at power up
pixel_ring.off()

#next, reset the LCD, since it gets gibberish written to it during boot
#reset pin is hooked up to GPIO 24.
wiringpi.pinMode(24, wiringpi.GPIO.OUTPUT)
wiringpi.pinMode(24, wiringpi.GPIO.INPUT)
