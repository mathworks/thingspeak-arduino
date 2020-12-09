#line 2 "testReadField.ino"
/*
  testReadField unit test
  
  Unit Test for the readField function in the ThingSpeak Communication Library for Arduino
  
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

#include <ThingSpeak.h> // always include thingspeak header file after other header files and custom macros

unsigned long testPublicChannelNumber = 209617;
const char * testPublicChannelWriteAPIKey = "514SX5OBP2OFEPL2";

unsigned long testPrivateChannelNumber = 209615;
const char * testPrivateChannelReadAPIKey = "D3MJBCYVNFX4Z2A8";
const char * testPrivateChannelWriteAPIKey = "KI8B7DJTWXLZ6EBV";

#define WRITE_DELAY_FOR_THINGSPEAK 15000

test(readPrivateFieldCase) 
{
  // Test basic value read -- should give anything but 0
  assertNotEqual(0.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());

  // Test read with invalid API key
  // * Using the wrong API key causes a connection failure on the next attempt to connect to ThingSpeak
  // * The cause is unknown, disable this test for now
  //assertEqual(0.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 1, "AFAKEAPIKEYFAKEX"));
  //assertEqual(TS_ERR_BADAPIKEY,ThingSpeak.getLastReadStatus()); 
}


#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_SAMD_MKR1000)  // Only the mega and mkr1000 has enough memory for all these tests
test(readPublicFieldCase) 
{ 

  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testPublicChannelNumber, 4, (float)1.0, testPublicChannelWriteAPIKey));
  
  // Test basic value read -- should give anything but 0
  assertNotEqual(0.0,ThingSpeak.readFloatField(testPublicChannelNumber, 4));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());

  // Test read of field out of range
  assertEqual(0.0,ThingSpeak.readFloatField(testPublicChannelNumber, 0));
  assertEqual(TS_ERR_INVALID_FIELD_NUM,ThingSpeak.getLastReadStatus());
  assertEqual(0.0,ThingSpeak.readFloatField(testPublicChannelNumber, 9));
  assertEqual(TS_ERR_INVALID_FIELD_NUM,ThingSpeak.getLastReadStatus());
  
  #if defined(USE_WIFI101_SHIELD)
    #define TS_ERR_BAD TS_ERR_CONNECT_FAILED
  #else
    #define TS_ERR_BAD TS_ERR_BADURL  
  #endif

  // Test read of invalid channel #
  // * Using the wrong API key causes a connection failure on the next attempt to connect to ThingSpeak
  // * The cause is unknown, disable this test for now
  //assertEqual(0.0,ThingSpeak.readFloatField(0, 1));
  //assertEqual(TS_ERR_BADURL,ThingSpeak.getLastReadStatus());
  //assertEqual(0.0,ThingSpeak.readFloatField(4294967295L, 1));
  //assertEqual(TS_ERR_BAD,ThingSpeak.getLastReadStatus());  
}


test(ReadFloatFieldCase) 
{
  // Always to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(1,(float)3.14159));  // float
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(2,-47)); // integer
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(3,(long)100000L)); // long
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(4,(float)NAN)); // Nan
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(5,"foobar")); // string
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(6,(float)INFINITY)); // + inf
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(7,(float)-INFINITY)); // + inf
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string

  // Test read as float
  assertLess(abs((float)3.14159 - ThingSpeak.readFloatField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey)),float(0.001));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(-47.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 2, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(100000.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 3, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertTrue(isnan(ThingSpeak.readFloatField(testPrivateChannelNumber, 4, testPrivateChannelReadAPIKey)));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(0.0,ThingSpeak.readFloatField(testPrivateChannelNumber, 5, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(1,isinf(ThingSpeak.readFloatField(testPrivateChannelNumber, 6, testPrivateChannelReadAPIKey)));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // AVR compiler doesn't correctly test for negative infinite (http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__math_1ga18a7409e0b2341afaa41993960961772.html)
  assertEqual(1,isinf(ThingSpeak.readFloatField(testPrivateChannelNumber, 7, testPrivateChannelReadAPIKey)));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
}

test(ReadIntFieldCase) 
{
  
  #if defined(ARDUINO_SAMD_MKR1000)
    #define OUT_OF_RANGE_IN "10000000000"
    #define OUT_OF_RANGE_OUT 2147483647
  #else
    #define OUT_OF_RANGE_IN (long)100000L
    #define OUT_OF_RANGE_OUT -31072
  #endif
   
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  
  ThingSpeak.setField(1,(float)3.14159);  // float
  ThingSpeak.setField(2,-47); // integer
  ThingSpeak.setField(3,OUT_OF_RANGE_IN); // long
  ThingSpeak.setField(4,(float)NAN); // Nan
  ThingSpeak.setField(5,"foobar"); // string
  ThingSpeak.setField(6,(float)INFINITY); // + inf
  ThingSpeak.setField(7,(float)-INFINITY); // + inf
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string

  // Test read as int
  assertEqual(3,ThingSpeak.readIntField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(-47,ThingSpeak.readIntField(testPrivateChannelNumber, 2, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Out of range
  assertEqual(OUT_OF_RANGE_OUT,ThingSpeak.readIntField(testPrivateChannelNumber, 3, testPrivateChannelReadAPIKey));  
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // NAN
  assertEqual(0,ThingSpeak.readIntField(testPrivateChannelNumber, 4, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Text
  assertEqual(0,ThingSpeak.readIntField(testPrivateChannelNumber, 5, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // INF
  assertEqual(0,ThingSpeak.readIntField(testPrivateChannelNumber, 6, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // -INF
  assertEqual(0,ThingSpeak.readIntField(testPrivateChannelNumber, 7, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
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
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string

  // Test read as long
  assertEqual(3L,ThingSpeak.readLongField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(-47L,ThingSpeak.readLongField(testPrivateChannelNumber, 2, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Out of range
  assertEqual(100000L,ThingSpeak.readLongField(testPrivateChannelNumber, 3, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // NAN
  assertEqual(0L,ThingSpeak.readLongField(testPrivateChannelNumber, 4, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Text
  assertEqual(0L,ThingSpeak.readLongField(testPrivateChannelNumber, 5, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // INF
  assertEqual(0L,ThingSpeak.readLongField(testPrivateChannelNumber, 6, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // -INF
  assertEqual(0L,ThingSpeak.readLongField(testPrivateChannelNumber, 7, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
}

test(ReadStringFieldCase) 
{
  
  #if defined(ARDUINO_SAMD_MKR1000)
    #define NAN_STR "nan"
    #define POS_INF_STR "inf"
    #define NEG_INF_STR "-inf"
  #else
    #define NAN_STR "NAN"
    #define POS_INF_STR "INF"
    #define NEG_INF_STR "-INF"
  #endif
  
  
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  
  ThingSpeak.setField(1,(float)3.14159);  // float
  ThingSpeak.setField(2,-47); // integer
  ThingSpeak.setField(3,(long)100000L); // long
  ThingSpeak.setField(4,(float)NAN); // Nan
  ThingSpeak.setField(5,"foobar"); // string
  ThingSpeak.setField(6,(float)INFINITY); // + inf
  ThingSpeak.setField(7,(float)-INFINITY); // + inf
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string

  // Test read as long
  assertEqual(String("3.14159"),ThingSpeak.readStringField(testPrivateChannelNumber, 1, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());

  assertEqual(String("-47"),ThingSpeak.readStringField(testPrivateChannelNumber, 2, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Out of range
  assertEqual(String("100000"),ThingSpeak.readStringField(testPrivateChannelNumber, 3, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // NAN 
  assertEqual(String(NAN_STR),ThingSpeak.readStringField(testPrivateChannelNumber, 4, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // Text
  assertEqual(String("foobar"),ThingSpeak.readStringField(testPrivateChannelNumber, 5, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // INF
  assertEqual(String(POS_INF_STR),ThingSpeak.readStringField(testPrivateChannelNumber, 6, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  // -INF
  assertEqual(String(NEG_INF_STR),ThingSpeak.readStringField(testPrivateChannelNumber, 7, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
}

test(readStatusPublicCase)
{
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  ThingSpeak.setStatus("abcdef");
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPublicChannelNumber, testPublicChannelWriteAPIKey)); // string
	//assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(String("abcdef"),ThingSpeak.readStatus(testPublicChannelNumber));		
}

test(readStatusPrivateCase)
{
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  ThingSpeak.setStatus("abcdef");
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string
	//assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(String("abcdef"),ThingSpeak.readStatus(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
}

test(readCreatedAtPublicCase)
{
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  ThingSpeak.setCreatedAt("2016-12-21T11:11:11Z");
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPublicChannelNumber, testPublicChannelWriteAPIKey)); // string
	//assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(String("2016-12-21T11:11:11Z"),ThingSpeak.readCreatedAt(testPublicChannelNumber));
}

test(readCreatedAtPrivateCase)
{
  // Always wait 15 seconds to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  ThingSpeak.setCreatedAt("2016-12-21T11:11:11Z");
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string
	//assertEqual(TS_OK_SUCCESS,ThingSpeak.getLastReadStatus());
  assertEqual(String("2016-12-21T11:11:11Z"),ThingSpeak.readCreatedAt(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
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
