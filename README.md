# PiADC

To run this with a Raspberry Pi:
Connect the ADC to the SPI pins on the Pi and connect the LED to GPIO pin 5.
Change IP address in pc.c file and run make.

To run the program execute the following programs.
On the Pi:
```
./dataAq
```
```
sudo ./pi
```
On the local pc:
```
./pc on local pc
```
Then you should see the readings of the ADC in the dataAq terminal und nothing happening in the other both.
To start the plotting of the data simply type ```p``` into the pc terminal and hit enter.
Gnuplot should start and plot the incoming data. To trigger the LED for one second type ```s``` into the pc terminal and hit enter.

To quit type ```q``` and hit enter.
You have to manually stop the dataAq programm with cntrl+c.
