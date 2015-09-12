# Solar-Arduino-Honeybee

## Honeybee Collapse Disorder Background

[Honeybee Die-Off](http://www.theguardian.com/environment/2015/may/13/honeybee-deaths-colonies-beekeepers) is a a serious problem happening around the world. Honeybees which are indigenous to Europe and have been brought over to North America are dying off for a variety of reasons, namely 1) Lack of genetic diversity and robustness, 2) parasites, 3) harsh treatment or mistreatment 4) chemicals, among other potential reasons not listed here. 

### Objective - Parasite Monitoring

The objective of this project is to provide an easy way for honeybee farmers to cheaply and remotely monitor hives for incoming parasites, a major contributor to honeybee die-off. The actionable item for a commercial beekeeper, according to the University of Minnesota extension service, may be to treat a hive which is undergoing incoming mite infestation, to save a hive from further die-off.  

### TL;DR

Our primary objective is to give beekeepers the ability to remotely flag mite-infested hives for treatment.

## Equipment

This project uses an Arduino Yunto read data from a ACS712 current meter as well as a voltage divider to read directly from a 12V battery, which is powering the Yun itself via a USB output 12VDC/5VDC voltage converter.

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




