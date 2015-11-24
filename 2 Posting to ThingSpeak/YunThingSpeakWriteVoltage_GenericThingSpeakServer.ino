/*
YunThingSpeakWriteVoltage_GenericThingSpeakServer

  Reads analog voltages from pin 0 and writes them to 1 field of a channel on ThingSpeak every 15 minutes (600k milliseconds).

  ThingSpeak ( https://www.thingspeak.com ) is a free IoT service for prototyping
  systems that collect, analyze, and react to their environments.

  Adapted from Software Copyright 2015, The MathWorks, Inc.

  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation.
*/


// Note!  You must include ThingSpeak.h for functions in this file to work properly.
#include "ThingSpeak.h"

/// ***********************************************************************************************************
// This example selects the correct library to use based on the board selected under the Tools menu in the IDE.
// Yun, Wired Ethernet shield, wi-fi shield, and Spark are all supported.
// With Uno and Mega, the default is that you're using a wired ethernet shield (http://www.arduino.cc/en/Main/ArduinoEthernetShield)
// If you're using a wi-fi shield (http://www.arduino.cc/en/Main/ArduinoWiFiShield), uncomment the line below
// ***********************************************************************************************************
//#define USE_WIFI_SHIELD
#ifdef ARDUINO_ARCH_AVR

  #ifdef ARDUINO_AVR_YUN
    #include "YunClient.h"
    YunClient client;
  #else

    #ifdef USE_WIFI_SHIELD
      #include <SPI.h>
      #include <WiFi.h>
      char ssid[] = "<YOURNETWORK>";          //  your network SSID (name)
      char pass[] = "<YOURPASSWORD>";   // your network password
      int status = WL_IDLE_STATUS;
      WiFiClient  client;
    #else
      // Use wired ethernet shield
      #include <SPI.h>
      #include <Ethernet.h>
//      byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
      EthernetClient client;
    #endif
  #endif
  // On Arduino:  0 - 1023 maps to 0 - 5 volts
  #define VOLTAGE_MAX 5.0
  #define VOLTAGE_MAXCOUNTS 1023.0
#endif

#ifdef SPARK
    TCPClient client;
    #define VOLTAGE_MAX 3.3
    #define VOLTAGE_MAXCOUNTS 4095.0
#endif

/*
  *****************************************************************************************
  **** Visit https://www.thingspeak.com to sign up for a free account and create
  **** a channel.  The video tutorial http://community.thingspeak.com/tutorials/thingspeak-channels/
  **** has more information. You need to change this to your channel, and your write API key
  **** IF YOU SHARE YOUR CODE WITH OTHERS, MAKE SURE YOU REMOVE YOUR WRITE API KEY!!
  *****************************************************************************************/

// Put your channel number below.
unsigned long myChannelNumber = XXXXX;

// Put yoru API Key below.
const char * myWriteAPIKey = "XXXXXXXXXXXXXXXX";

void setup() {
  #ifdef ARDUINO_ARCH_AVR
    #ifdef ARDUINO_AVR_YUN
      Bridge.begin();
    #else
      #ifdef USE_WIFI_SHIELD
        WiFi.begin(ssid, pass);
      #else
        Ethernet.begin(mac);
      #endif
    #endif
  #endif

  ThingSpeak.begin(client);
}

void loop() {
  // Read the input on each pin and convert the reading
  // On Arduino:  0 - 1023 maps to 0 - 5 volts
  // On Particle: 0 - 4095 maps to 0 - 3.3 volts

  // The following scaling factor was calibrated manually with a power supply and a multimeter.
  // In this example, we used a voltage divider and scaled the voltage down from up to 24V to the range of 0V-5V
  // WARNING!  If connecting an Arduino to high voltage, you are at risk of frying your board.
  // Please ensure you take proper circuit precautions to ensure that it will be protected from inrush current and overvoltage.

  float scale = 5.3539;
  float pin0Voltage = analogRead(A0) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS)*scale;

  // Write to ThingSpeak. There are up to 8 fields in a channel, and we're writing to one of them.
  // First, you set each of the fields you want to send
  ThingSpeak.setField(1,pin0Voltage);

  // Then you write the fields that you've set all at once.
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  delay(600000);
}
