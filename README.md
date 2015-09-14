# Solar-Arduino-Honeybee

## Honeybee Die-Off Background

[Honeybee Die-Off](http://www.theguardian.com/environment/2015/may/13/honeybee-deaths-colonies-beekeepers) is a a serious problem happening around the world. Honeybees which are indigenous to Europe and have been brought over to North America are dying off for a variety of reasons, namely 1) Lack of genetic diversity and robustness, 2) parasites, 3) harsh treatment or mistreatment 4) chemicals, among other potential reasons not listed here. 

### Objective - Parasite Monitoring

The objective of this project is to provide an easy way for honeybee farmers to cheaply and remotely monitor hives for incoming parasites, a major contributor to honeybee die-off. The actionable item for a commercial beekeeper, according to the University of Minnesota extension service, may be to treat a hive which is undergoing incoming mite infestation, to save a hive from further die-off.  One of our hypotheses is that commercial bee-keepers will be highly concerned about mites being carried into the hive on the backs of bees who had visited other infested hives.

### Audience - Hobby / DIY Electroincs

This Readme file is written as a general, "how-to," aimed at a DIY Hobby Electronics Audience for use with either at-home or commercial bee-hives.

### TL;DR

Our primary objective is to give beekeepers the ability to remotely flag mite-infested hives for treatment.

## Equipment

This project uses an Arduino Yun to read data from a ACS712 current meter as well as a voltage divider to read directly from a 12V battery, which is powering the Yun itself via a USB output 12VDC/5VDC voltage converter.

https://www.arduino.cc/en/Main/ArduinoBoardYun?from=Products.ArduinoYUN

### Prototype Circuit Layout

![Honeybee Solar Circuit Layout](https://github.com/pwdel/Solar-Arduino-Honeybee/blob/master/Presentation/Arduino%20Circuit%20Layout.png)


![ACS712 Illustration](https://github.com/pwdel/Solar-Arduino-Honeybee/blob/master/Presentation/ACS712-Illustration.png)


## Automatically Connecting the Yun to a Remote Server

### Overall Objective of Connecting to Remote Server

Our method includes connecting the Yun to a VPS to send initial data files which will be cleaned up, converted & transferred to a heroku server.

### Automatic Authentication & Cron Job

As of the date of authoring this GitHub repository, there is much written online about being able to automatically authenticate an Arduino Yun (Yun) with a laptop or desktop running an IDE, but not as much for connecting the Yun to a remote virtual private server (VPS).

The Arduino Yun has [Dropbear](https://matt.ucc.asn.au/dropbear/dropbear.html) installed, which is a relatively small SSH client.  So, you have to generate a public RSA ID and copy that over to the remote VPS.  That way, the Yun will be able to automatically log on to the VPS without a person having to manually enter a password.

#### Accomplishing the Automatic Authentication

* Generate keys on the Yun via:

> dropbearkey -t rsa -f ~/.ssh/id_rsa 

* Change the format for a standard SSH server

> dropbearkey -y -f ~/.ssh/id_rsa | grep "^ssh-rsa" >> authorized_keys

* SCP the authorized_keys to the VPS

> scp authorized_keys user@webhost: ~/.ssh/authorized_keys

* Set file permissions to 600

* Logged on to the Yun, test the SSH function to see if it asks for a password.  If the following does not ask you for a password, it works!  If not, something is wrong.

> root@user:~#  ssh user@webhost -i /root/.ssh/id_rsa

* Automate the RSYNC function: the following needs to go into a cron job for a regular update.

> rsync -avz -e "ssh -i /root/.ssh/id_rsa" 
> user@webhost:~# somefilehere.txt somefilehere.txt

## Power Requirements & Electronics Warning

### Speaking the 'Language' of Solar Power

#### The 12V Standard - Lead Acid Batteries

Our hypothesis is that the most commonly available solar systems for the hobbyist & DIY (e.g. online sales) market are systems designed around 12V applications.  An additional hypothesis comes from [Battery University](http://batteryuniversity.com/learn/article/whats_the_best_battery), which stipulates as a gross generalization, that Lead Acid batteries typically provide the most economical battery solution for fixed-location systems where weight is not a concern.  That being said, when designing a system for a particular application, it is important to understand the various tradeoffs which occur for a given battery system, including but not limited to:

* Deep Charging Capability
* Battery Memory
* Robustness
* Weight
* Discharge & Storage Effects (e.g. can the battery be stored in a discharged state)
* Number of charge cycles needed
* Thermal Effects
* Environmental, Disposal & Recycling Considerations
* Transport Regulations
* Cost

#### The 12V Standard - Solar Panels

12V solar panels are available for purchase online which have a Voc(max) rating of 24V.  

#### 12V to USB (5V) Power Conversion

##### WARNING! USB OVERVOLTAGE RISK!

It is important to realize that this higher voltage from the solar panel can lead to an overvoltage being fed into the Arduino Yun input USB port, leading to the board being damaged.  Using an off-the-shelf voltage converter going from 12V input to 5V USB output is potentially dangerous, as the 5V USB output could potentially be pulled higher than the rated input value.

![Arduino Yun Block Diagram](https://raw.githubusercontent.com/pwdel/Solar-Arduino-Honeybee/master/1%20Measuring%20Current%20%26%20Voltage%20From%20Battery/Arduino-Yun-Block-Diagram.png)

To understand more about the Arduino Yun board, you can see the documentation [here](https://www.arduino.cc/en/Main/ArduinoBoardYun), which stipulates the following:

> It is recommended to power the board via the micro-USB connection with 5VDC. If you are powering the board though the Vin pin, you must supply a regulated 5VDC. There is no on-board voltage regulator for higher voltages, which will damage the board.

That being said, the [documentation](http://www.atmel.com/Images/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf) for the ATmega32u4 microcontroller stipulates on page 383 what the absolute maximum ratings are, which are as follows:

> MaximumOperatingVoltage ................6.0V

and further that:

> Stresses beyond those listed under “Absolute Maximum Ratings” may cause permanent dam- age to the device. This is a stress rating only and functional operation of the device at these or other conditions beyond those indicated in the operational sections of this specification is not implied. Exposure to absolute maximum rat- ing conditions for extended periods may affect device reliability.

Therefore, going at or beyond 6V will damage the ATmega32u4, which will lead to the entire Yun being useless, as re-soldering a new ATmega32u4 would be a difficult process, taking time that may be beyond what is worth doing so over buying a new board.

#### Quick Note About Terminal Connections

Just as with everything going from a "higher power," electric source to an electronic source, make sure to keep in mind a couple troubleshooting notes which may save you time and hassle:

1. Make sure all of the DC electronics grounds and DC electric grounds are tied together.
2. Make sure all of the connections are tight / strong.  This should go without saying, but if you don't, you may be sitting there wondering why WiFi won't connect or why the system won't work, only to find after several hours of troubleshooting that, "your computer wasn't plugged in," in the sense that there wasn't a strong enough connection at the actual battery, and therefore there was not enough power in one of your leads.  Be careful to make sure everything is well-connected and you don't have open-circuits, or un-necessary resistive effects.

##### Recommended USB 12V to 5V Converters



## Radio-Frequency (RF) Considerations

### USB WiFi Packages

http://downloads.arduino.cc/openwrtyun/1/packages/index.html

http://www.lucadentella.it/en/2014/11/08/yun-adattatore-wifi-usb/


## Vision System

Background:

http://hivetool.org/counter/index.html
