#! /bin/bash

sudo chmod 777 /dev/ttyUSB0
sudo chmod 777 /dev/ttyUSB1
cp components/DMX_Manager/modified_drivers/* $IDF_PATH/components/driver/
make flash && make monitor
