#line 2 "testBegin.ino"
/*
  testWriteField unit test
  
  Unit Test for the writeField function in the ThingSpeak Communication Library for Arduino

  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize and
  analyze live data streams in the cloud.
  
  This test use the ArduinoUnit 2.1.0 unit test framework.  Visit https://github.com/mmurdoch/arduinounit to learn more.
  
  Copyright 2017, The MathWorks, Inc.
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation folder where the library was installed.
  See the accompaning licence.txt file for licensing information.
*/

//#define USE_WIFI101_SHIELD
//#define USE_ETHERNET_SHIELD


#if !defined(USE_WIFI101_SHIELD) && !defined(USE_ETHERNET_SHIELD) && !defined(ARDUINO_SAMD_MKR1000) && !defined(ARDUINO_AVR_YUN) && !defined(ARDUINO_ARCH_ESP8266)
  #error "Uncomment the #define for either USE_WIFI101_SHIELD or USE_ETHERNET_SHIELD"
#endif

#include <ArduinoUnit.h>
#include <ThingSpeak.h>

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



unsigned long testChannelNumber = 209617;
const char * testChannelWriteAPIKey = "514SX5OBP2OFEPL2";

test(beginCase) 
{
  assertTrue(ThingSpeak.begin(client));

  assertTrue(ThingSpeak.begin(client,IPAddress(1,2,3,4),80));

  assertTrue(ThingSpeak.begin(client,"www.mathworks.com",80));
}


test(badAddresses) 
{
  // Test for valid, but incorrect, URL (www.mathworks.com)
  assertTrue(ThingSpeak.begin(client,"www.mathworks.com",80));
  // www.mathworks.com will sometimes sometimes return a 404 or a 301 depending on server settings.  Test the negative case instead.
  assertNotEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)1.0, testChannelWriteAPIKey) );
  
  // Test for non-existant URL (http://www.iwishthiswebsitewerereal.com/)
  #ifdef USE_ETHERNET_SHIELD
    int badDomainResponse = ERR_UNEXPECTED_FAIL;
  #else
    int badDomainResponse = ERR_CONNECT_FAILED;
  #endif
  assertTrue(ThingSpeak.begin(client,"www.iwishthiswebsitewerereal.com",80));
  assertEqual(badDomainResponse, ThingSpeak.writeField(testChannelNumber, 1, (float)2.0, testChannelWriteAPIKey));
 
  // Test for non-existant IP 192.168.1.234
  assertTrue(ThingSpeak.begin(client,IPAddress(192,168,1,234),80));
  assertEqual(ERR_CONNECT_FAILED, ThingSpeak.writeField(testChannelNumber, 1, (float)2.0, testChannelWriteAPIKey));

  //Test for bad suburl (badapi.thingspeak.com)
  #ifdef USE_ETHERNET_SHIELD
    int badURLResponse = ERR_UNEXPECTED_FAIL;
  #else
    int badURLResponse = ERR_CONNECT_FAILED;
  #endif
  assertTrue(ThingSpeak.begin(client,"invalid.thingspeak.com",80));
  assertEqual(badURLResponse, ThingSpeak.writeField(testChannelNumber, 1, (float)4.0, testChannelWriteAPIKey));

}

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only
  Serial.println("Starting test...");
  #ifdef ARDUINO_AVR_YUN
    Bridge.begin();
  #else   
    #if defined(ARDUINO_ARCH_ESP8266) || defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000)
      WiFi.begin(ssid, pass);
    #else
      Ethernet.begin(mac);
    #endif
  #endif
}

void loop()
{
  Test::run();
}

