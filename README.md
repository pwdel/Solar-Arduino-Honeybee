# Solar-Arduino-Honeybee

## Honeybee Collapse Disorder Background

Honeybee Colony Collpase Disroder is a a serious problem happening around the world. Honeybees which are indigenous to Europe and have been brought over to North America are dying off for a variety of reasons, namely 1) Lack of genetic diversity and robustness, 2) Parasites, 3) Harsh treatment or mistreatment 4) chemicals, among other potential reasons not listed here. The objective of this project is to provide an easy way for honeybee farmers to cheaply and remotely monitor hives and predict oncoming colony collapse. The actionable item for a beekeeper, according to the University of Minnesota extension service, would be to quarantine a collapsing hive to prevent whatever effects from spreading to surrounding hives. As such our primary objective is to give beekeepers the ability to remotely track and quarantine hives, as well as give other useful information about the hive such as temperature and weight.

## Equipment

This project uses an Arduino Yunto read data from a ACS712 current meter as well as a voltage divider to read directly from a 12V battery, which is powering the Yun itself via a USB output 12VDC/5VDC voltage converter.

https://www.arduino.cc/en/Main/ArduinoBoardYun?from=Products.ArduinoYUN

![alt tag](https://github.com/pwdel/Solar-Arduino-Honeybee/blob/master/Presentation/Arduino%20Circuit%20Layout.png)

## Automatically Connecting the Yun to a Remote Server

### Authentication

