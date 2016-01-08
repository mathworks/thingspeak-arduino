/*
  ReadPrivateChannel
  
  Reads the latest voltage value from a private channel on ThingSpeak every 30 seconds
  and prints to the serial port debug window.

  For an example of how to read from a public channel, see ReadChannel example
  
  ThingSpeak ( https://www.thingspeak.com ) is a free IoT service for prototyping
  systems that collect, analyze, and react to their environments.
  
  Copyright 2016, The MathWorks, Inc.
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation folder where the library was installed.
  See the accompaning licence file for licensing information.
*/

#ifdef SPARK
	#include "ThingSpeak/ThingSpeak.h"
#else
	#include "ThingSpeak.h"
#endif

// ***********************************************************************************************************
// This example selects the correct library to use based on the board selected under the Tools menu in the IDE.
// Yun, Wired Ethernet shield, wi-fi shield, esp8266, and Spark are all supported.
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
      // ESP8266 USERS -- YOU MUST COMMENT OUT THE LINE BELOW.  There's a bug in the Arduino IDE that causes it to not respect #ifdef when it comes to #includes
      // If you get "multiple definition of `WiFi'" -- comment out the line below.
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

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
  char ssid[] = "<YOURNETWORK>";          //  your network SSID (name) 
  char pass[] = "<YOURPASSWORD>";   // your network password
  int status = WL_IDLE_STATUS;
  WiFiClient  client;
#endif

// Particle Core, Photon, and Electron the results are published to the Particle dashboard using events.
// Go to http://dashboard.particle.io, click on the logs tab, and you'll see the events coming in. 
#ifdef SPARK
  TCPClient client;
#endif

/*
  *****************************************************************************************
  **** Visit https://www.thingspeak.com to sign up for a free account and create
  **** a channel.  The video tutorial http://community.thingspeak.com/tutorials/thingspeak-channels/ 
  **** has more information. You need to change this to your channel, and your read API key
  **** IF YOU SHARE YOUR CODE WITH OTHERS, MAKE SURE YOU REMOVE YOUR READ API KEY!!
  *****************************************************************************************

  This is the ThingSpeak channel used in the write examples (31461).  It is private, and requires a
  read API key to access it.  We'll read from the first field.
*/
unsigned long myChannelNumber = 31461;
const char * myReadAPIKey = "NKX4Z5JGO4M5I18A";

void setup() {
  #if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP8266)
    Serial.begin(9600);
    #ifdef ARDUINO_AVR_YUN
      Bridge.begin();
    #else
      #if defined(USE_WIFI_SHIELD) || defined(ARDUINO_ARCH_ESP8266)
        WiFi.begin(ssid, pass);
      #else
        Ethernet.begin(mac);
      #endif
    #endif
  #endif

  ThingSpeak.begin(client);
}

void loop() {
  // Read the latest value from field 1 of channel 31461
  float voltage = ThingSpeak.readFloatField(myChannelNumber, 1, myReadAPIKey);
  #if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP8266)
    Serial.print("Latest voltage is: "); 
    Serial.print(voltage);
    Serial.println("V"); 
  #endif
  #ifdef SPARK
    Particle.publish("thingspeak-readvoltage", "Latest voltage is: " + String(voltage) + "V",60,PRIVATE);
  #endif
  delay(30000);
}
