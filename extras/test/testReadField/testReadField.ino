#line 2 "testReadField.ino"
/*
  testReadField unit test
  
  Unit Test for the readField function in the ThingSpeak Communication Library for Arduino

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

unsigned long testPublicChannelNumber = 12397;
unsigned long testPrivateChannelNumber = 31461;
const char * testPrivateChannelReadAPIKey = "NKX4Z5JGO4M5I18A";
const char * testPrivateChannelWriteAPIKey = "LD79EOAAWRVYF04Y";

#define WRITE_DELAY_FOR_THINGSPEAK 15000

test(readPublicFieldCase) 
{
  // Test basic value read -- should give anything but 0
  assertNotEqual(0.0,ThingSpeak.readFloatField(testPublicChannelNumber, 4));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());

  // Test read of field out of range
  assertEqual(0.0,ThingSpeak.readFloatField(testPublicChannelNumber, 0));
  assertEqual(ERR_INVALID_FIELD_NUM,ThingSpeak.getLastReadStatus());
  assertEqual(0.0,ThingSpeak.readFloatField(testPublicChannelNumber, 9));
  assertEqual(ERR_INVALID_FIELD_NUM,ThingSpeak.getLastReadStatus());

  // Test read of invalid channel #
  assertEqual(0.0,ThingSpeak.readFloatField(0, 1));
  assertEqual(ERR_BADURL,ThingSpeak.getLastReadStatus());
  assertEqual(0.0,ThingSpeak.readFloatField(4294967295L, 1));
  assertEqual(ERR_BADURL,ThingSpeak.getLastReadStatus());
}

test(readPrivateFieldCase) 
{
  // Test basic value read -- should give anything but 0
  assertNotEqual(0.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());

  // Test write with invalid API key
  assertEqual(0.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 1, "AFAKEAPIKEYFAKEX"));
  // GECK 1235554:  Expect thingspeak to return 404 -- it returns 200.
  //assertEqual(ERR_BADURL,ThingSpeak.getLastReadStatus());
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
}

#ifdef ARDUINO_AVR_MEGA2560 // Only the mega has enough memory for all these tests
test(ReadFloatFieldCase) 
{
  // Always to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  
  assertEqual(OK_SUCCESS,ThingSpeak.setField(1,(float)3.14159));  // float
  assertEqual(OK_SUCCESS,ThingSpeak.setField(2,-47)); // integer
  assertEqual(OK_SUCCESS,ThingSpeak.setField(3,(long)100000L)); // long
  assertEqual(OK_SUCCESS,ThingSpeak.setField(4,(float)NAN)); // Nan
  assertEqual(OK_SUCCESS,ThingSpeak.setField(5,"foobar")); // string
  assertEqual(OK_SUCCESS,ThingSpeak.setField(6,(float)INFINITY)); // + inf
  assertEqual(OK_SUCCESS,ThingSpeak.setField(7,(float)-INFINITY)); // + inf
  assertEqual(OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string

  // Test read as float
  assertLess(abs((float)3.14159 - ThingSpeak.readFloatField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey)),float(0.001));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(-47.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 2, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(100000.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 3, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertTrue(isnan(ThingSpeak.readFloatField(testPrivateChannelNumber, 4, testPrivateChannelReadAPIKey)));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(0.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 5, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(1,isinf(ThingSpeak.readFloatField(testPrivateChannelNumber, 6, testPrivateChannelReadAPIKey)));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // AVR compiler doesn't correctly test for negative infinite (http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__math_1ga18a7409e0b2341afaa41993960961772.html)
  assertEqual(1,isinf(ThingSpeak.readFloatField(testPrivateChannelNumber, 7, testPrivateChannelReadAPIKey)));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
}

test(ReadIntFieldCase) 
{
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  
  ThingSpeak.setField(1,(float)3.14159);  // float
  ThingSpeak.setField(2,-47); // integer
  ThingSpeak.setField(3,(long)100000L); // long
  ThingSpeak.setField(4,(float)NAN); // Nan
  ThingSpeak.setField(5,"foobar"); // string
  ThingSpeak.setField(6,(float)INFINITY); // + inf
  ThingSpeak.setField(7,(float)-INFINITY); // + inf
  assertEqual(OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string

  // Test read as int
  assertEqual(3,ThingSpeak.readIntField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(-47,ThingSpeak.readIntField(testPrivateChannelNumber, 2, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Out of range
  assertEqual(-31072,ThingSpeak.readIntField(testPrivateChannelNumber, 3, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // NAN
  assertEqual(0,ThingSpeak.readIntField(testPrivateChannelNumber, 4, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Text
  assertEqual(0,ThingSpeak.readIntField(testPrivateChannelNumber, 5, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // INF
  assertEqual(0,ThingSpeak.readIntField(testPrivateChannelNumber, 6, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // -INF
  assertEqual(0,ThingSpeak.readIntField(testPrivateChannelNumber, 7, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
}

test(ReadLongFieldCase) 
{
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  
  ThingSpeak.setField(1,(float)3.14159);  // float
  ThingSpeak.setField(2,-47); // integer
  ThingSpeak.setField(3,(long)100000L); // long
  ThingSpeak.setField(4,(float)NAN); // Nan
  ThingSpeak.setField(5,"foobar"); // string
  ThingSpeak.setField(6,(float)INFINITY); // + inf
  ThingSpeak.setField(7,(float)-INFINITY); // + inf
  assertEqual(OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string

  // Test read as long
  assertEqual(3L,ThingSpeak.readLongField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(-47L,ThingSpeak.readLongField(testPrivateChannelNumber, 2, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Out of range
  assertEqual(100000L,ThingSpeak.readLongField(testPrivateChannelNumber, 3, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // NAN
  assertEqual(0L,ThingSpeak.readLongField(testPrivateChannelNumber, 4, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Text
  assertEqual(0L,ThingSpeak.readLongField(testPrivateChannelNumber, 5, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // INF
  assertEqual(0L,ThingSpeak.readLongField(testPrivateChannelNumber, 6, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // -INF
  assertEqual(0L,ThingSpeak.readLongField(testPrivateChannelNumber, 7, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
}

test(ReadStringFieldCase) 
{
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  
  ThingSpeak.setField(1,(float)3.14159);  // float
  ThingSpeak.setField(2,-47); // integer
  ThingSpeak.setField(3,(long)100000L); // long
  ThingSpeak.setField(4,(float)NAN); // Nan
  ThingSpeak.setField(5,"foobar"); // string
  ThingSpeak.setField(6,(float)INFINITY); // + inf
  ThingSpeak.setField(7,(float)-INFINITY); // + inf
  assertEqual(OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string

  // Test read as long
  assertEqual(String("3.14159"),ThingSpeak.readStringField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());

  assertEqual(String("-47"),ThingSpeak.readStringField(testPrivateChannelNumber, 2, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Out of range
  assertEqual(String("100000"),ThingSpeak.readStringField(testPrivateChannelNumber, 3, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // NAN
  assertEqual(String("NAN"),ThingSpeak.readStringField(testPrivateChannelNumber, 4, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Text
  assertEqual(String("foobar"),ThingSpeak.readStringField(testPrivateChannelNumber, 5, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // INF
  assertEqual(String("INF"),ThingSpeak.readStringField(testPrivateChannelNumber, 6, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // -INF
  assertEqual(String("-INF"),ThingSpeak.readStringField(testPrivateChannelNumber, 7, testPrivateChannelReadAPIKey));
  assertEqual(OK_SUCCESS,ThingSpeak.getLastReadStatus());
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

