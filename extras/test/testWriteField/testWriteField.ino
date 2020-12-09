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

unsigned long testChannelNumber = 1070863;
const char * testChannelWriteAPIKey = "UI7FSU4O8ZJ5BM8O";

#define WRITE_DELAY_FOR_THINGSPEAK 15000 // Data write limit for a free user (15 sec).

// Field number macros
#define FIELD0 0
#define FIELD1 1
#define FIELD2 2
#define FIELD3 3
#define FIELD4 4
#define FIELD5 5
#define FIELD6 6
#define FIELD7 7
#define FIELD8 8
#define FIELD9 9

/* This test case checks the following:
    - basic value write
    - rate limit error
    - invalid field number
    - invalid channel number
    - invalid write API key
    - NULL value
*/
test(writeFieldCase) 
{
  float val = 1.0;
  
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test basic value write
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));

  // Test write when not enough time has elapsed
  // This test will work correctly after the fix by Afan is in production ThingSpeak
  //assertEqual(TS_ERR_NOT_INSERTED, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));

  // Test write to field out of range
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.writeField(testChannelNumber, FIELD0, val, testChannelWriteAPIKey));
  assertEqual(TS_ERR_INVALID_FIELD_NUM, ThingSpeak.writeField(testChannelNumber, FIELD9, val, testChannelWriteAPIKey));

  // Test write to invalid channel #
  // ThingSpeak accepts this, since it only uses the API key to write.  The Channel number is ignored.
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(0, FIELD1, val, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(4294967295L, FIELD1, val, testChannelWriteAPIKey));
 
  // Test write with invalid API key
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(TS_ERR_BADAPIKEY, ThingSpeak.writeField(testChannelNumber, FIELD1, val, "AFAKEAPIKEYFAKEX"));

  // Test NULL value
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, NULL, testChannelWriteAPIKey));

}

/* This test case checks the following:
    - right float value
    - max/min values of float
    - high precision values of float
    - NaN and Inf values
    - out of range values of float
*/
test(writeFieldFloatCase) 
{
  float val = 1.0;
  
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test float write
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));

  // Test max/min range values
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  val = -999999000000;
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  val = 999999000000;
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  
  // Test high precision values
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  val = -3.14159;
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  val = 3.14159;
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  
  // Test passing NaN and Inf
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  val = NAN;
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  val = -INFINITY;
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  val = INFINITY;
  assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  
  // Test out of range values
  delay(WRITE_DELAY_FOR_THINGSPEAK);
  val = -1000000000000.0;
  assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  val = 1000000000000.0;
  assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_SAMD_MKR1000) // Only the mega and mkr1000 has enough memory for all these tests

  /* This test case checks the following:
      - right int value
      - max/min values of int
  */
  test(writeFieldIntCase) 
  {
    int val = 1;
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test int write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  
    // Test max/min range values
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    val = -32768;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    val = 32767;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
    
  }
  
  /* This test case checks the following:
      - right long value
      - max/min values of long
  */
  test(writeFieldLongCase) 
  {
    long val = 1000000L;
    
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test long write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  
    // Test max/min range values
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    val = -2147483648L;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    val = 2147483647L;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, val, testChannelWriteAPIKey));
  }
  
  /* This test case checks the following:
      - right const char * value
      - empty const char * value
      - right String value (using String class)
      - empty String value (using String class)
      - field character limits via String (positive and negative cases)
  */
  test(writeFieldStringCase) 
  {
    const char * constCharVal = "The Rain in Spain";
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test const char * string write
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, constCharVal, testChannelWriteAPIKey));
  
    // Test empty const char * string write
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    constCharVal = "";
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, constCharVal, testChannelWriteAPIKey));
  
    String stringVal = "The Rain in Spain";
    
    // Test String class instance write
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, stringVal, testChannelWriteAPIKey));
  
    // Test empty String class instance write
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    stringVal = "";
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, FIELD1, stringVal, testChannelWriteAPIKey));
  
    unsigned int numChar = 256;
    String longString;
    longString.reserve(numChar);
  
    // Test field character limits via string
    for(unsigned int i = 0; i < 255; i++)
    {
      longString += '0';
    }
    
    // longString will have 256 '0' characters, since the number of characters ThingSpeak field can store is limited to 256 characters.
    
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    assertEqual(TS_OK_SUCCESS, ThingSpeak.writeField(testChannelNumber, 1, longString, testChannelWriteAPIKey));
  
    longString += '0';
  
    // longString will now have 257 '0' characters. Out of range error.
    
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.writeField(testChannelNumber, 1, longString, testChannelWriteAPIKey));
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
