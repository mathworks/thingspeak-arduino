#line 2 "testSecureConnect.ino"
/*
  testSecureConnect unit test
  
  Unit Test for the secure connection through HTTPS in the ThingSpeak Communication Library for Arduino
  
  This test use the ArduinoUnit 2.1.0 unit test framework.  Visit https://github.com/mmurdoch/arduinounit to learn more.
  
  ArduinoUnit does not support ESP8266 or ESP32 and therefor these tests will not compile for those platforms.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

//#define USE_WIFI101_SHIELD
//#define USE_ETHERNET_SHIELD


#if !defined(USE_WIFI101_SHIELD) && !defined(USE_ETHERNET_SHIELD) && !defined(ARDUINO_SAMD_MKR1000) && !defined(ARDUINO_AVR_YUN)
  #error "Uncomment the #define for either USE_WIFI101_SHIELD or USE_ETHERNET_SHIELD"
#endif

#include <ArduinoUnit.h>

#if defined(ARDUINO_AVR_YUN)
    #include "YunClient.h"
    YunClient client;
#else
  #if defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000)
    // Use WiFi
    #include <SPI.h>
    #include <WiFi101.h>
    char ssid[] = "<YOURNETWORK>";    //  your network SSID (name) 
    char pass[] = "<YOURPASSWORD>";   // your network password
    int status = WL_IDLE_STATUS;
    WiFiSSLClient  secureClient;
  #elif defined(USE_ETHERNET_SHIELD)
    // Use wired ethernet shield
    #include <SPI.h>
    #include <Ethernet.h>
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    EthernetClient client;
  #endif
#endif

#define TS_ENABLE_SSL
#include <ThingSpeak.h> // always include thingspeak header file after other header files and custom macros
#define FIELD1 1
#define WRITE_DELAY_FOR_THINGSPEAK 15000 // Data write limit for a free user (15 sec).

unsigned long testChannelNumber = 1070863;
const char * testChannelWriteAPIKey = "UI7FSU4O8ZJ5BM8O";

#if defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000)
  test(secureConnectMKR1000Case)
  {
    assertTrue(ThingSpeak.begin(secureClient));

    int val = 25;
  
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    assertTrue(ThingSpeak.begin(secureClient));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));  
  }
#endif

#if defined(USE_ETHERNET_SHIELD) || defined(ARDUINO_AVR_UNO)
  test(secureConnectUnoEthernetCase)
  {
    assertTrue(ThingSpeak.begin(client));

    int val = 25;
  
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    assertTrue(ThingSpeak.begin(client));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));  
  }
#endif

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only
  Serial.println("Starting test...");
  #ifdef ARDUINO_AVR_YUN
    Bridge.begin();
  #else   
    #if defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000)
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
