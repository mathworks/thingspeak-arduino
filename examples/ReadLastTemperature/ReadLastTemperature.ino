/*
  ReadLastTemperature
  
  Reads the latest temperature from the MathWorks weather station in Natick, MA
  https://thingspeak.com/channels/12397 on ThingSpeak, and prints to
  the serial port debug window every 30 seconds.
  
  ThingSpeak ( https://www.thingspeak.com ) is a free IoT service for building
  systems that collect, analyze, and react to their environments.
  
  Copyright 2015, The MathWorks, Inc.
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation folder where the library was installed.
  See the accompaning licence.txt file for licensing information.
*/

#include "ThingSpeak.h"

// ***********************************************************************************************************
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
#endif

// On the Particle Core and Photon, the results are published to the Particle dashboard using events.
// Go to http://dashboard.particle.io, click on the logs tab, and you'll see the events coming in. 
#ifdef SPARK
  TCPClient client;
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
  #ifdef ARDUINO_ARCH_AVR
    Serial.begin(9600);
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
  // Read the latest value from field 4 of channel 12397
  float temperatureInF = ThingSpeak.readFloatField(weatherStationChannelNumber, temperatureFieldNumber);
  #ifdef ARDUINO_ARCH_AVR
    Serial.print("Current temp is: "); 
    Serial.print(temperatureInF);
    Serial.println(" degrees F"); 
  #endif
  #ifdef SPARK
    Spark.publish("thingspeak-lasttemp", "Current temp " + String(temperatureInF) + " degrees F",60,PRIVATE);
  #endif
  delay(30000); // Note that the weather station only updates once a minute
}
