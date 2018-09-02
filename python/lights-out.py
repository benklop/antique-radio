#!/usr/bin/env python

import pixel_ring/pixel_ring
import wiringpi

#configure wiringpi with GPIO numbering
wiringpi.wiringPiSetupGpio()

#this script is responsible for initial setup and initialization of the enclosure's hardware.

#first, disable the ring LEDs, since they are enabled by default at power up
pixel_ring.off()
#disable the center LED too
pixel_ring.set_vad_led(0)
