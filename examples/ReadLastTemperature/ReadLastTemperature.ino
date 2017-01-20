/*
  ReadLastTemperature
  
  Reads the latest temperature from the MathWorks weather station in Natick, MA
  https://thingspeak.com/channels/12397 on ThingSpeak, and prints to
  the serial port debug window every 30 seconds.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize and
  analyze live data streams in the cloud.
  
  Copyright 2017, The MathWorks, Inc.
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation folder where the library was installed.
  See the accompaning licence file for licensing information.
*/

#include "ThingSpeak.h"


// ***********************************************************************************************************
// This example selects the correct library to use based on the board selected under the Tools menu in the IDE.
// Yun, Ethernet shield, WiFi101 shield, esp8266, and MXR1000 are all supported.
// With Yun, the default is that you're using the Ethernet connection.
// If you're using a wi-fi 101 or ethernet shield (http://www.arduino.cc/en/Main/ArduinoWiFiShield), uncomment the corresponding line below
// ***********************************************************************************************************

//#define USE_WIFI101_SHIELD
//#define USE_ETHERNET_SHIELD


#if !defined(USE_WIFI101_SHIELD) && !defined(USE_ETHERNET_SHIELD) && !defined(ARDUINO_SAMD_MKR1000) && !defined(ARDUINO_AVR_YUN) && !defined(ARDUINO_ARCH_ESP8266)
  #error "Uncomment the #define for either USE_WIFI101_SHIELD or USE_ETHERNET_SHIELD"
#endif


#if defined(ARDUINO_AVR_YUN)
    #include "YunClient.h"
    YunClient client;
#else
  #if defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_ARCH_ESP8266)
    // Use WiFi
    #ifdef ARDUINO_ARCH_ESP8266
      #include <ESP8266WiFi.h>
    #else
      #include <SPI.h>
      #include <WiFi101.h>
    #endif
    char ssid[] = "<YOURNETWORK>";    //  your network SSID (name) 
    char pass[] = "<YOURPASSWORD>";   // your network password
    int status = WL_IDLE_STATUS;
    WiFiClient  client;
  #elif defined(USE_ETHERNET_SHIELD)
    // Use wired ethernet shield
    #include <SPI.h>
    #include <Ethernet.h>
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    EthernetClient client;
  #endif
#endif


/*
  This is the ThingSpeak channel number for the MathwWorks weather station
  https://thingspeak.com/channels/12397.  It senses a number of things, including 
  Wind Direction, Wind Speed, Humidity, Temperature, Rainfall, and Atmospheric Pressure.

  Temperature is stored in field 4
*/

unsigned long weatherStationChannelNumber = 12397;
unsigned int temperatureFieldNumber = 4;

void setup() {

  Serial.begin(9600);
  #ifdef ARDUINO_AVR_YUN
    Bridge.begin();
  #else   
    #if defined(ARDUINO_ARCH_ESP8266) || defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000)
      WiFi.begin(ssid, pass);
    #else
      Ethernet.begin(mac);
    #endif
  #endif

  ThingSpeak.begin(client);
    
}

void loop() {
  // Read the latest value from field 4 of channel 12397
  float temperatureInF = ThingSpeak.readFloatField(weatherStationChannelNumber, temperatureFieldNumber);

  Serial.print("Current temp is: "); 
  Serial.print(temperatureInF);
  Serial.println(" degrees F"); 

  delay(30000); // Note that the weather station only updates once a minute
}





