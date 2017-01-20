/*
  ReadWeatherStation
  
  Reads the latest weather data every 60 seconds from the public MathWorks
  weather station in Natick, MA https://thingspeak.com/channels/12397 on ThingSpeak.
  
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
  https://thingspeak.com/channels/12397.  It senses a number of things and puts them in the eight
  field of the channel:
  Field 1 - Wind Direction (degrees where 0 is North)
  Field 2 - Wind Speed (MPH)
  Field 3 - Humidity (%RH)
  Field 4 - Temperature (Degrees F)
  Field 5 - Rainfall (inches since last measurement)
  Field 6 - Atmospheric Pressure (inHg)
*/
unsigned long weatherStationChannelNumber = 12397;

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
  float windDirection = ThingSpeak.readFloatField(weatherStationChannelNumber,1);
  float windSpeed = ThingSpeak.readFloatField(weatherStationChannelNumber,2);
  float humidity = ThingSpeak.readFloatField(weatherStationChannelNumber,3);
  float temperature = ThingSpeak.readFloatField(weatherStationChannelNumber,4);
  float rainfall = ThingSpeak.readFloatField(weatherStationChannelNumber,5);
  float pressure = ThingSpeak.readFloatField(weatherStationChannelNumber,6);

  Serial.println("======================================"); 
  Serial.println("Current weather conditions in Natick: "); 
  Serial.print(temperature);
  Serial.print(" degrees F, "); 
  Serial.print(humidity);
  Serial.println("% humidity"); 
  Serial.print("Wind at ");
  Serial.print(windSpeed);
  Serial.print(" MPH at "); 
  Serial.print(windDirection);
  Serial.println(" degrees"); 
  Serial.print("Pressure is ");
  Serial.print(pressure);
  Serial.print(" inHg");
  if(rainfall > 0)
  {
  	Serial.print(", and it's raining"); 
  }
  Serial.println(); 

  delay(60000); // Note that the weather station only updates once a minute

}
