# FanCo
![GitHub release](https://img.shields.io/github/v/release/The-JC/FanCo?color=brightgreen&include_prereleases)  
FanCo is an advanced wind tunnel fan controller. Monitoring and regulating up to 120 fans individually with its custom UI or the API. 
The UI navigation is simple and intuitive utilizing just a rotary encoder.
FanCo is also capable of logging fan speeds over a longer period for later analyzation. 

FanCo is made up of two fundamental components. The master managing the UI and logging. And second, the slave, measuring actual speeds and compensate fluctuations in speed. Each Slave is capable of regulating up to 4 fans. Further slaves can be connected via cables to expand the maximum amounts of fans and also remove the number of control cables needed. 

## Requirements
- RaspberryPi Zero (W) or 1 (2 or later only works with custom build)
- Slave PCB manufactured and soldered from your favorite Chinese manufacturer (see [Parts List](/circuits/slave/bom.pdf)).
- USBasp programmer (An Arduino can be used as a programmer).
- Any HDMI compatible display (we used a small 5" HDMI display)
- A rotary encoder as an input
- One or more 4-pin fans
- A 5V power supply to provide power to the Raspberry Pi and the Display


## Installation
### I. RaspberryPi  
1. Download the latest Raspbian Version from https://www.raspberrypi.org/downloads/ and flash it to the SD card.
2. Insert SD card into the pi. Start it check if it boots.
3. Run ```sudo apt-get update & upgrade ``` to update all packages.
4. Make sure you've downloaded and installed pigpiod from http://abyz.me.uk/rpi/pigpio/download.html.
5. Download the latest version of FanCo from  
https://github.com/The-JC/Rflw/releases/latest
    > You can use ```wget``` to directly download the binary to the RaspberryPi.
6. To run the FanCo UI run ```./fanco``` in the installation directory.
7. When using an 800x480 display change or add the following lines in  
**/boot/config.txt**
    ```
    hdmi_group=2
    hdmi_mode=87
    hdmi_cvt=800 480 60 0 0 0
    ```
8. To autostart FanCoUI at boot. Add the following lines to your rc.local before the exit 0 line.
```sudo nano /etc/rc.local```
    ```
    pigpiod
    PATH_TO_FANCO/fanco
    ```

### II. Slave
>You will need avrdude for flashing the program to the Slaves

### III. Cabling
- Connect the rotary encoder, slave(s) and the display to the Raspberry Pi following the below scheme.
![Cabling](/docs/images/cabling.svg)

## Usage
1. Run ```sudo pigpiod```
2. Start program by running ```./fanco```

## Principle of operation
The slave counts pulses(n) received on the tacho pin in a set interval(dt). By calculating f=n/dt you`ll get the frequency(f) of the input signal. Further multiplication x60 converts the frequency from 1/t in s to min which equals RPMs (revolutions per minute).

![Schema](/docs/images/schema.svg)

## Troubleshooting
#### Rotary Encoder is not recognized
- Check if cables are connected correctly
- Make sure you have pull-up resistors on all 3 data lines

#### FanCoUI shows invalid or random data
- Make sure all slaves are powered up
- Check the data bus for wiring errors
- Check if all slaves are recognized by running ```i2cdetect -y 1```
- Try running ```i2cdump -y -r 0-2 1 0x04 c```
    - Reading devices fails -> restart all slaves & try updating slaves
    - Reads X X X as byte-value -> restart all slaves
    - Reads some byte-value -> all should be good try updating FanCoUI to the latest version.


## ToDo
- Interval programming
- Extended logs
- Remote control API
- init.d start script
