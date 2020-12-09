#line 2 "testReadMultipleFields.ino"
/*
  testReadField unit test

  Unit Test for the readMultipleFields functionality in the ThingSpeak Communication Library for Arduino

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

unsigned long testPublicChannelNumber = 12397; // public channel number

unsigned long testPrivateChannelNumber = 1070863; // private channel number
const char * testPrivateChannelWriteAPIKey = "UI7FSU4O8ZJ5BM8O"; // write API key for the private channel
const char * testPrivateChannelReadAPIKey = "I0QKRG8MU0HEFKYQ"; // read API key for the private channel


#define WRITE_DELAY_FOR_THINGSPEAK 15000 // Data write limit for a free user (15 sec).

// Field number macros
#define FIELD0 0 // invalid
#define FIELD1 1
#define FIELD2 2
#define FIELD3 3
#define FIELD4 4
#define FIELD5 5
#define FIELD6 6
#define FIELD7 7
#define FIELD8 8
#define FIELD9 9 // invalid

/* This test case checks for the following:
    - read multiple with right channel number and read API key
    - read multiple with invalid channel number
    - read multiple with invalid read API key
*/
test(readBasicCase)
{
  // Test basic feed read
  assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
  assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPublicChannelNumber));

  // Test read with invalid API key from a private channel
  assertEqual(TS_ERR_BADAPIKEY, ThingSpeak.readMultipleFields(testPrivateChannelNumber, "AFAKEAPIKEYFAKEX"));

  // Test read with invalid channel number
  assertEqual(TS_ERR_BADURL, ThingSpeak.readMultipleFields(00000, testPrivateChannelReadAPIKey));
  assertEqual(TS_ERR_BADURL, ThingSpeak.readMultipleFields(00000));


}


#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_SAMD_MKR1000)  // Only the mega and mkr1000 has enough memory for all these tests

  
  /* This test case checks for the following:
      - Number of characters that can be read at a time per field
  */
  test(fieldLimitsCase)
  {
    unsigned int numChar = 110;
    String longString;
    String longStatus;
    float latitude = 3.1415926535;
    float longitude = 3.1415926535;
    float elevation = 3.1415926535;

    // Always to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test field character limits via string
    for(unsigned int i = 0; i < numChar; i++)
    {
      longString += '0';
    } 

    // status message will have 255 characters
    for(unsigned int i = 0; i < 255; i++)
    {
      longStatus += '0';
    }

    // Test multiple write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, longString));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, longString));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD3, longString));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD4, longString));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD5, longString));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD6, longString));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD7, longString));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD8, longString));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setStatus(longStatus));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setLatitude(latitude));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setLongitude(longitude));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setElevation(elevation));
    assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey));

    // Test basic feed read
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));

    // Test fetching data
    assertEqual(longString, ThingSpeak.getFieldAsString(FIELD1));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());

    assertEqual(longString, ThingSpeak.getFieldAsString(FIELD2));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());

    assertEqual(longString, ThingSpeak.getFieldAsString(FIELD3));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());

    assertEqual(longString, ThingSpeak.getFieldAsString(FIELD4));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());

    assertEqual(longString, ThingSpeak.getFieldAsString(FIELD5));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());

    assertEqual(longString, ThingSpeak.getFieldAsString(FIELD6));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());

    assertEqual(longString, ThingSpeak.getFieldAsString(FIELD7));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());

    assertEqual(longString, ThingSpeak.getFieldAsString(FIELD8));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
    
  }
  
  
  /* This test case checks for the following:
      - set values for all 8 fields
      - read multiple
      - get values for all 8 fields as float
      - invalid field number
  */
  test(getFieldAsFloatCase)
  {
    float floatVal = 3.14159;
    int intVal = -47;
    long longVal = 100000L;
    float nanVal = NAN;
    float infVal = INFINITY;
    const char * constCharVal = "foobar";
    String stringVal = "barfoo";
  
    // Always to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test basic multi-field write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, floatVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, intVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD3, longVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD4, nanVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD5, constCharVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD6, stringVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD7, infVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD8, NULL));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey));
  
    // Test basic feed read
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
  
    // Test fetching data as float
    assertEqual(floatVal, ThingSpeak.getFieldAsFloat(FIELD1));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual((float)intVal, ThingSpeak.getFieldAsFloat(FIELD2));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual((float)longVal, ThingSpeak.getFieldAsFloat(FIELD3));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertTrue(isnan( ThingSpeak.getFieldAsFloat(FIELD4)));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(0.0, ThingSpeak.getFieldAsFloat(FIELD5));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(0.0, ThingSpeak.getFieldAsFloat(FIELD6));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertTrue(isinf(ThingSpeak.getFieldAsFloat(FIELD7)));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual((float)NULL, ThingSpeak.getFieldAsFloat(FIELD8));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    // Test invalid field numbers
    assertNotEqual(floatVal, ThingSpeak.getFieldAsFloat(FIELD0));
    assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.getLastReadStatus());
  
    assertNotEqual(floatVal, ThingSpeak.getFieldAsFloat(FIELD9));
    assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.getLastReadStatus());
  }
  
  
  /* This test case checks for the following:
      - set values for all 8 fields
      - read multiple
      - get values for all 8 fields as int
      - invalid field number
  */
  test(getFieldAsIntCase)
  {
    float floatVal = 3.14159;
    int intVal = -47;
    long longVal = 100000L;
    const char * constCharVal = "foobar";
    String stringVal = "barfoo";
  
    // Always to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test basic multi-field write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, floatVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, intVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD3, longVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD4, constCharVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD5, stringVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD6, NULL));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey));
  
    // Test basic feed read
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
  
    // Test fetching data as int
    assertEqual((int)floatVal, ThingSpeak.getFieldAsInt(FIELD1));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(intVal, ThingSpeak.getFieldAsInt(FIELD2));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual((int)longVal, ThingSpeak.getFieldAsInt(FIELD3));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(0, ThingSpeak.getFieldAsInt(FIELD4));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(0, ThingSpeak.getFieldAsInt(FIELD5));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual((int)NULL, ThingSpeak.getFieldAsInt(FIELD6));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    // Test invalid field numbers
    assertNotEqual(intVal, ThingSpeak.getFieldAsInt(FIELD0));
    assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.getLastReadStatus());
  
    assertNotEqual(intVal, ThingSpeak.getFieldAsInt(FIELD9));
    assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.getLastReadStatus());
  }
  
  
  /* This test case checks for the following:
      - set values for all 8 fields
      - read multiple
      - get values for all 8 fields as long
      - invalid field number
  */
  test(getFieldAsLongCase)
  {
    float floatVal = 3.14159;
    int intVal = -47;
    long longVal = 100000L;
    const char * constCharVal = "foobar";
    String stringVal = "barfoo";
  
    // Always to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test basic multi-field write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, floatVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, intVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD3, longVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD4, constCharVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD5, stringVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD6, NULL));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey));
  
    // Test basic feed read
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
  
    // Test fetching data as long
    assertEqual((long)floatVal, ThingSpeak.getFieldAsLong(FIELD1));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual((long)intVal, ThingSpeak.getFieldAsInt(FIELD2));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(longVal, ThingSpeak.getFieldAsInt(FIELD3));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(0, ThingSpeak.getFieldAsInt(FIELD4));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(0, ThingSpeak.getFieldAsInt(FIELD5));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual((long)NULL, ThingSpeak.getFieldAsInt(FIELD6));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    // Test invalid field numbers
    assertNotEqual(longVal, ThingSpeak.getFieldAsInt(FIELD0));
    assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.getLastReadStatus());
  
    assertNotEqual(longVal, ThingSpeak.getFieldAsInt(FIELD9));
    assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.getLastReadStatus());
  }
  
  
  /* This test case checks for the following:
      - set values for all 8 fields
      - read multiple
      - get values for all 8 fields as String
      - invalid field number
  */
  test(getFieldAsStringCase)
  {
    float floatVal = 3.14159;
    int intVal = -47;
    long longVal = 100000L;
    float nanVal = NAN;
    float infVal = INFINITY;
    const char * constCharVal = "foobar";
    String stringVal = "barfoo";
  
    // Always to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test basic multi-field write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, floatVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, intVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD3, longVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD4, nanVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD5, constCharVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD6, stringVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD7, infVal));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD8, NULL));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey));
  
    // Test basic feed read
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
  
    // Test fetching data as String
    assertEqual("3.14159", ThingSpeak.getFieldAsString(FIELD1));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual("-47", ThingSpeak.getFieldAsString(FIELD2));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual("100000", ThingSpeak.getFieldAsString(FIELD3));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual("nan", ThingSpeak.getFieldAsString(FIELD4));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(constCharVal, ThingSpeak.getFieldAsString(FIELD5));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(stringVal, ThingSpeak.getFieldAsString(FIELD6));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual("inf", ThingSpeak.getFieldAsString(FIELD7));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    assertEqual(String(NULL), ThingSpeak.getFieldAsString(FIELD8));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.getLastReadStatus());
  
    // Test invalid field numbers
    assertNotEqual(stringVal, ThingSpeak.getFieldAsString(FIELD0));
    assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.getLastReadStatus());
  
    assertNotEqual(stringVal, ThingSpeak.getFieldAsString(FIELD9));
    assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.getLastReadStatus());
  }
  
  
  /* This test case checks for the following:
      - set status message
      - read multiple
      - get status message
  */
  test(getStatusCase)
  {
    String statusMessage = "abcdef";
    // Always wait 15 seconds to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test status write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setStatus(statusMessage));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string
  
    // Test basic feed read
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
  
    // Test fetching status message
    assertEqual(statusMessage, ThingSpeak.getStatus());
  }
  
  
  /* This test case checks for the following:
      - set latitude, longitude, elevation
      - read multiple
      - get latitude, longitude, elevation
  */
  test(getLocationCase)
  {
    float latitude = 42.28372; // +ve is North, -ve is South
    float longitude = -71.3472; // +ve is East, -ve is West
    float elevation = 54.9; // metres above sea level
  
    // Always wait 15 seconds to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test status write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setLatitude(latitude));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setLongitude(longitude));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setElevation(elevation));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string
  
    // Test basic feed read
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
  
    // Test fetching location coordinates
    assertNotEqual("", ThingSpeak.getLatitude());
    assertNotEqual("", ThingSpeak.getLongitude());
    assertNotEqual("", ThingSpeak.getElevation());
  }
  
  
  /* This test case checks for the following:
      - read multiple
      - get default current created-at timestamp
      - get user-defined created-at timestamp
  */
  test(getCreatedAtCase)
  {
    // Test basic feed read
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
  
    // Default current created-at timestamp
    assertNotEqual("", ThingSpeak.getCreatedAt());
  
    /* Test this separately
     
    // Always wait 15 seconds to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // User-defined created-at timestamp
    ThingSpeak.setCreatedAt("2020-07-17T20:24:50Z"); // Time should be later than the current time, since latest feed is read.
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeFields(testPrivateChannelNumber, testPrivateChannelWriteAPIKey)); // string
    assertEqual(TS_OK_SUCCESS, ThingSpeak.readMultipleFields(testPrivateChannelNumber, testPrivateChannelReadAPIKey));
    assertEqual("2020-07-17T20:24:50Z", ThingSpeak.getCreatedAt());
    
    */
  }

#endif // Mega and MKR1000 only tests


void setup()
{
  Serial.begin(9600);
  while (!Serial); // for the Arduino Leonardo/Micro only
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
