/*
  WriteMultipleVoltages
  
  Reads analog voltages from pins 0-7 and writes them to the 8 fields of a channel on ThingSpeak every 20 seconds.
  
  ThingSpeak ( https://www.thingspeak.com ) is a free IoT service for prototyping
  systems that collect, analyze, and react to their environments.
  
  Copyright 2015, The MathWorks, Inc.
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation folder where the library was installed.
  See the accompaning licence.txt file for licensing information.
*/

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
      byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
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
int myChannelNumber = 31461;
const char * myWriteAPIKey = "LD79EOAAWRVYF04Y";

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
  float pin0Voltage = analogRead(A0) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);
  float pin1Voltage = analogRead(A1) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);
  float pin2Voltage = analogRead(A2) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);
  float pin3Voltage = analogRead(A3) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);
  float pin4Voltage = analogRead(A4) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);
  float pin5Voltage = analogRead(A5) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);
  float pin6Voltage = analogRead(A6) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);
  float pin7Voltage = analogRead(A7) * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);

  // Write to ThingSpeak. There are up to 8 fields in a channel, and we're writing to all of them.  
  // First, you set each of the fields you want to send
  ThingSpeak.setField(1,pin0Voltage);
  ThingSpeak.setField(2,pin1Voltage);
  ThingSpeak.setField(3,pin2Voltage);
  ThingSpeak.setField(4,pin3Voltage);
  ThingSpeak.setField(5,pin4Voltage);
  ThingSpeak.setField(6,pin5Voltage);
  ThingSpeak.setField(7,pin6Voltage);
  ThingSpeak.setField(8,pin7Voltage);

  // Then you write the fields that you've set all at once.
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  

  delay(20000); // ThingSpeak will only accept updates every 15 seconds. 
}
