#line 2 "testBegin.ino"
/*
  testWriteField unit test
  
  Unit Test for the writeField function in the ThingSpeak Communication Library for Arduino

  ThingSpeak ( https://www.thingspeak.com ) is a free IoT service for prototyping
  systems that collect, analyze, and react to their environments.
  
  This test use the ArduinoUnit 2.1.0 unit test framework.  Visit https://github.com/mmurdoch/arduinounit to learn more.
  
  Copyright 2015, The MathWorks, Inc.
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation folder where the library was installed.
  See the accompaning licence.txt file for licensing information.
*/

#include <ArduinoUnit.h>

#ifdef ARDUINO_AVR_YUN
  #include <SPI.h>
  #include "YunClient.h"
  YunClient client;
#else
  // Assume that we're using a wired Ethernet shield on a Mega
  #include <SPI.h>
  #include <Ethernet.h>
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  EthernetClient client;
#endif

#include <ThingSpeak.h>

unsigned long testChannelNumber = 31461;
const char * testChannelWriteAPIKey = "LD79EOAAWRVYF04Y";

test(beginCase) 
{
  assertTrue(ThingSpeak.begin(client));

  assertTrue(ThingSpeak.begin(client,IPAddress(1,2,3,4),80));

  assertTrue(ThingSpeak.begin(client,"www.mathworks.com",80));
}


test(badAddresses) 
{
  // Test for valid, but incorrect, URL (en.wikipedia.org) that gives a 404 response
  assertTrue(ThingSpeak.begin(client,"en.wikipedia.org",80));
  assertEqual(ERR_BADURL, ThingSpeak.writeField(testChannelNumber, 1, (float)1.0, testChannelWriteAPIKey));
  
  // Test for non-existant URL (http://www.iwishthiswebsitewerereal.com/)
  assertTrue(ThingSpeak.begin(client,"www.iwishthiswebsitewerereal.com",80));
  #ifdef ARDUINO_AVR_YUN
    // Yun gives -301 response
    int badURLResponse = ERR_CONNECT_FAILED;
  #else
    // Ethernet shield gives -302 response
    int badURLResponse = ERR_UNEXPECTED_FAIL;
  #endif
  assertEqual(badURLResponse, ThingSpeak.writeField(testChannelNumber, 1, (float)2.0, testChannelWriteAPIKey));

  // Test for non-existant IP 192.168.1.234
  assertTrue(ThingSpeak.begin(client,IPAddress(192,168,1,234),80));
  assertEqual(-301, ThingSpeak.writeField(testChannelNumber, 1, (float)2.0, testChannelWriteAPIKey));

  //Test for bad suburl (badapi.thingspeak.com)
  assertTrue(ThingSpeak.begin(client,"invalid.thingspeak.com",80));
  assertEqual(badURLResponse, ThingSpeak.writeField(testChannelNumber, 1, (float)4.0, testChannelWriteAPIKey));
}

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only
  #ifdef ARDUINO_AVR_YUN
    Bridge.begin();
  #else
    Ethernet.begin(mac);
  #endif
}

void loop()
{
  Test::run();
}

