#line 2 "testWriteField.ino"
/*
  testWriteField unit test
  
  Unit Test for the writeField function in the ThingSpeak Communication Library for Arduino
  
  This test use the ArduinoUnit 2.1.0 unit test framework.  Visit https://github.com/mmurdoch/arduinounit to learn more.
  
  ArduinoUnit does not support ESP8266 or ESP32 and therefor these tests will not compile for those platforms.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2018, The MathWorks, Inc.
*/

//#define USE_WIFI101_SHIELD
//#define USE_ETHERNET_SHIELD

#if !defined(USE_WIFI101_SHIELD) && !defined(USE_ETHERNET_SHIELD) && !defined(ARDUINO_SAMD_MKR1000) && !defined(ARDUINO_AVR_YUN)
  #error "Uncomment the #define for either USE_WIFI101_SHIELD or USE_ETHERNET_SHIELD"
#endif

#include <ArduinoUnit.h>
#include <ThingSpeak.h>

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

#define WRITE_DELAY_FOR_THINGSPEAK 15000

test(writeFieldCase) 
{
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test basic value write
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)1.0, testChannelWriteAPIKey));

  // Test write when not enough time has elapsed
  assertEqual(ERR_NOT_INSERTED, ThingSpeak.writeField(testChannelNumber, 1, (float)1.0, testChannelWriteAPIKey));

  // Allow enough time to pass to make sure that it would work 
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test write to field out of range
  assertEqual(ERR_INVALID_FIELD_NUM, ThingSpeak.writeField(testChannelNumber, 0, (float)1.0, testChannelWriteAPIKey));
  assertEqual(ERR_INVALID_FIELD_NUM, ThingSpeak.writeField(testChannelNumber, 9, (float)1.0, testChannelWriteAPIKey));

  // Test write to invalid channel #
  // ThingSpeak accepts this, since it only uses the API key to write.  The Channel number is ignored.
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(0, 1, (float)1.0, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(4294967295L, 1, (float)1.0, testChannelWriteAPIKey));
 
  // Test write with invalid API key
  // * Using the wrong API key causes a connection failure on the next attempt to connect to ThingSpeak
  // * The cause is unknown, disable this test for now
  //delay(WRITE_DELAY_FOR_THINGSPEAK);
  //assertEqual(ERR_BADAPIKEY, ThingSpeak.writeField(testChannelNumber, 1, (float)1.0, "AFAKEAPIKEYFAKEX"));

}

test(writeFieldFloatCase) 
{
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test float write
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)1.0, testChannelWriteAPIKey));

  // Test max range values
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)-999999000000, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)999999000000, testChannelWriteAPIKey));
  
  // Test high precision values
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)3.14159, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)-3.14159, testChannelWriteAPIKey));
  
  // Test passing NaN and Inf
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)NAN, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)INFINITY, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (float)-INFINITY, testChannelWriteAPIKey));
  
  // Test out of range values
  assertEqual(ERR_OUT_OF_RANGE, ThingSpeak.writeField(testChannelNumber, 1, (float)-1000000000000.0, testChannelWriteAPIKey));
  assertEqual(ERR_OUT_OF_RANGE, ThingSpeak.writeField(testChannelNumber, 1, (float)1000000000000.0, testChannelWriteAPIKey));
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_SAMD_MKR1000) // Only the mega and mkr1000 has enough memory for all these tests
test(writeFieldIntCase) 
{
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test int write
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (int)1, testChannelWriteAPIKey));

  // Test max range values
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (int)-32768, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (int)32767, testChannelWriteAPIKey));
}

test(writeFieldLongCase) 
{
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test long write
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, 1000000L, testChannelWriteAPIKey));

  // Test max range values
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (long)-2147483648L, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, (long)2147483647L, testChannelWriteAPIKey));
}

test(writeFieldCharStarCase) 
{
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test char * write
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, "The Rain in Spain", testChannelWriteAPIKey));

  // Test empty string
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, "", testChannelWriteAPIKey));

  char longString[300];

  // Test max string
  memset(longString, '0',255);
  longString[255] = 0;
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, longString, testChannelWriteAPIKey));

  // Test long string
  memset(longString, '0',sizeof(longString)/sizeof(longString[0]) - 1);
  longString[sizeof(longString)] = 0;
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(ERR_OUT_OF_RANGE, ThingSpeak.writeField(testChannelNumber, 1, longString, testChannelWriteAPIKey));
}

test(writeFieldStringCase) 
{
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test String write
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, String("The Rain in Spain"), testChannelWriteAPIKey));

  // Test empty string
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, String(), testChannelWriteAPIKey));

  unsigned int numChar = 300;
  String longString;
  longString.reserve(numChar);

  // Test max string
  for(unsigned int i = 0; i < 255; i++)
  {
    longString += '0';
  }
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, longString, testChannelWriteAPIKey));

  // Test long string
  longString.reserve(numChar);
  for(unsigned int i = 0; i < numChar; i++)
  {
    longString += '0';
  }
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(ERR_OUT_OF_RANGE, ThingSpeak.writeField(testChannelNumber, 1, longString, testChannelWriteAPIKey));
}
#endif // Mega and MKR1000 only tests

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
  ThingSpeak.begin(client);
}

void loop()
{
  Test::run();
}

