#!/bin/sh

sudo install 60-radio-devices.rules /etc/udev/rules.d/60-radio-devices.rules
sudo udevadm control --reload-rules