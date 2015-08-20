/*
  CheerLights
  
  Reads the latest CheerLights color on ThingSpeak, and sets a common anode RGB LED on digital pins 5, 6, and 9.
  On Spark core, the built in RGB LED is used
  Visit https://www.cheerlights.com for more info.

  ThingSpeak ( https://www.thingspeak.com ) is a free IoT service for prototyping
  systems that collect, analyze, and react to their environments.
  
  Copyright 2015, The MathWorks, Inc.

  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation folder where the library was installed.
  See the accompaning licence file for licensing information.
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

// Make sure that you put a 330 ohm resistor between the arduino
// pins and each of the color pins on the LED.
int pinRed = 9;
int pinGreen = 6;
int pinBlue = 5;

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

#ifdef SPARK
  TCPClient client;
#endif


/*
  This is the ThingSpeak channel number for CheerLights
  https://thingspeak.com/channels/1417.  Field 1 contains a string with
  the latest CheerLights color.
*/
unsigned long cheerLightsChannelNumber = 1417;

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

  #ifdef ARDUINO_ARCH_AVR
      pinMode(pinRed,OUTPUT);
      pinMode(pinGreen,OUTPUT);
      pinMode(pinBlue,OUTPUT);
  #endif
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
int colorRGB[][3] = {     0,  0,  0, // "none"
                        255,  0,  0, // "red"
                        255,192,203, // "pink"
                          0,255,  0, // "green"
                          0,  0,255, // "blue"
                          0, 255,255, // "cyan",
                        255, 50,100, // "white",
                        255, 30, 10, // "warmwhite",
                        255, 30, 10, // "oldlace",
                        128,  0, 25, // "purple",
                        255,  0, 50, // "magenta",
                        255, 100,  0, // "yellow",
                        255, 10,  0}; // "orange"};


void setColor(String color)
{
  // Look through the list of colors to find the one that was requested
  for(int iColor = 0; iColor <= 12; iColor++)
  {
    if(color == colorName[iColor])
    {
    // When it matches, look up the RGB values for that color in the table,
    // and write the red, green, and blue values.
      #ifdef ARDUINO_ARCH_AVR
        analogWrite(pinRed,colorRGB[iColor][0]);
        analogWrite(pinGreen,colorRGB[iColor][1]);
        analogWrite(pinBlue,colorRGB[iColor][2]);
      #endif
      #ifdef SPARK
        RGB.control(true);
        RGB.color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]);
      #endif
      return;
    }
  }
}
