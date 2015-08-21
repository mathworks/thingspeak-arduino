#line 2 "testWriteField.ino"
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
#include <ThingSpeak.h>

#ifdef ARDUINO_AVR_YUN
  #include "YunClient.h"
  YunClient client;
#else
  // Assume that we're using a wired Ethernet shield on a Mega
  #include <SPI.h>
  #include <Ethernet.h>
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  EthernetClient client;
#endif

unsigned long testChannelNumber = 31461;
const char * testChannelWriteAPIKey = "LD79EOAAWRVYF04Y";

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
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(ERR_BADAPIKEY, ThingSpeak.writeField(testChannelNumber, 1, (float)1.0, "AFAKEAPIKEYFAKEX"));
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

#ifdef ARDUINO_AVR_MEGA2560 // Only the mega has enough memory for all these tests
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
#endif // Mega only tests

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only
  #ifdef ARDUINO_AVR_YUN
    Bridge.begin();
  #else
    Ethernet.begin(mac);
  #endif
  ThingSpeak.begin(client);
}

void loop()
{
  Test::run();
}

