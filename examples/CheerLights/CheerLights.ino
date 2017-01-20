/*
  CheerLights
  
  Reads the latest CheerLights color on ThingSpeak, and sets a common anode RGB LED on digital pins 5, 6, and 9.
  On Spark core, the built in RGB LED is used
  Visit http://www.cheerlights.com for more info.

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


// Make sure that you put a 330 ohm resistor between the arduino
// pins and each of the color pins on the LED.
int pinRed = 9;
int pinGreen = 6;
int pinBlue = 3;

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
  This is the ThingSpeak channel number for CheerLights
  https://thingspeak.com/channels/1417.  Field 1 contains a string with
  the latest CheerLights color.
*/
unsigned long cheerLightsChannelNumber = 1417;

void setup() {
  
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

  pinMode(pinRed,OUTPUT);
  pinMode(pinGreen,OUTPUT);
  pinMode(pinBlue,OUTPUT);
}

void loop() {
  // Read the latest value from field 1 of channel 1417
  String color = ThingSpeak.readStringField(cheerLightsChannelNumber, 1);
  setColor(color);
  
  // Check again in 5 seconds
  delay(5000);
}

// List of CheerLights color names
String colorName[] = {"none","red","pink","green","blue","cyan","white","warmwhite","oldlace","purple","magenta","yellow","orange"};

// Map of RGB values for each of the Cheerlight color names
int colorRGB[][3] = {     0,  0,  0,  // "none"
                        255,  0,  0,  // "red"
                        255,192,203,  // "pink"
                          0,255,  0,  // "green"
                          0,  0,255,  // "blue"
                          0,255,255,  // "cyan",
                        255,255,255,  // "white",
                        255,223,223,  // "warmwhite",
                        255,223,223,  // "oldlace",
                        128,  0,128,  // "purple",
                        255,  0,255,  // "magenta",
                        255,255,  0,  // "yellow",
                        255,165,  0}; // "orange"};

void setColor(String color)
{
  // Look through the list of colors to find the one that was requested
  for(int iColor = 0; iColor <= 12; iColor++)
  {
    if(color == colorName[iColor])
    {
      // When it matches, look up the RGB values for that color in the table,
      // and write the red, green, and blue values.

      analogWrite(pinRed,colorRGB[iColor][0]);
      analogWrite(pinGreen,colorRGB[iColor][1]);
      analogWrite(pinBlue,colorRGB[iColor][2]);

      return;
    }
  }
}
