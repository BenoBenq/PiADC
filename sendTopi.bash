#!/bin/bash
rsync -zvr -ssh /home/bent/Coding/C/Pi/Project/ pi@192.168.0.142:/home/pi/Desktop/Bent/Project --exclude={'sendTopi.bash','pi','pc','dataAq','recieve.d','.git','timingTest','timingResult.dat'}
