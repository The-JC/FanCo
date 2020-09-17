# FanCo

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/e620460b83344056b6eef0aaacb07cdc)](https://app.codacy.com/manual/The-JC/FanCo?utm_source=github.com&utm_medium=referral&utm_content=The-JC/FanCo&utm_campaign=Badge_Grade_Dashboard)

![GitHub release](https://img.shields.io/github/v/release/The-JC/FanCo?color=brightgreen&include_prereleases)  
FanCo is an advanced wind tunnel fan controller. Monitoring and regulating up to 120 fans individually with its custom UI or the API. 
The UI navigation is simple and intuitive utilizing just a rotary encoder.
FanCo is also capable of logging fan speeds over a longer period for later analyzation. 

FanCo is made up of two fundamental components. The master managing the UI and logging. And second, the slave, measuring actual speeds and compensate fluctuations in speed. Each Slave is capable of regulating up to 4 fans. Further slaves can be connected via cables to expand the maximum amounts of fans and also remove the number of control cables needed. 

## Table of contents
- [Requirements](#Requirements)
- [Installation](#Installation)
- [Usage](#Usage)
- [Principle of operation](#Principle%20of%20operation)
- [Troubleshooting](#Troubleshooting)
- [ToDo](#ToDo)

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
5. Download the latest version of FanCoUI from  
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
> **Requirments**  
>You will need [avrdude](http://www.nongnu.org/avrdude/) for flashing the program to the Slaves.
>Also you will need avr-gcc installed to compile the program.
>Further the build system CMake and make is required.
>You can install both on Ubuntu or Debian by running ```sudo apt-get install avrdude gcc-avr```
1. Connect the programmer and the slave, like shown in the following picture
![Connection](/docs/images/programming.svg)
2. Connect the programmer to the computer.
3. Change your current directory to ```cd programs/slave/FanRegulator_v01```
4. Change the slave address line ```#define ADDRESS 0xXX``` in ```include/config.h``` starting with 0x04
5. Configure the build by running ```cmake -B build .``` in the root directory of *FanRegulator_v01*
6. Go to the build directory by running ```cd build```
7. Run the following command to compile the program and flash it to the microcontroller: ```make flash```
8. Repeat the previous steps for every other microcontroller only changing the I2C address.

>**NOTE:** Currently only a USBasp programmer is the supported. We´re are working on supporting other programmer types like ArduinoISP. If you can't wait, you could also flash the binaries manualy using avrdude.

### III. Cabling
- Connect the rotary encoder, slave(s) and the display to the Raspberry Pi following the below scheme.
![Cabling](/docs/images/cabling.svg)

## Usage
1. Run ```sudo pigpiod```
2. Start program by running ```./fanco```

## Principle of operation
The slave counts pulses(n) received on the tacho pin in a set interval(dt). By calculating f=n/dt you`ll get the frequency(f) of the input signal. Further multiplication x60 converts the frequency from 1/t in s to min which equals RPMs (revolutions per minute).
Previously calculated speed is used for fine-tuning the fans set signal, to achieve a steady and constant speed at the desired speed.  
Further, the acquired data is requested by the master (RaspberyPi) every second. For logging, displaying and synchronization purposes.  
Firstly it´s averaged out to compensate for measurement and transmission errors. It later gets stored for logging and plotting purposes.

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
- Enclosure
- Flow dependent speed control