#line 2 "testSetFunctions.ino"
/*
  testSetField unit test
  
  Unit Test for the writeFields and setField functions in the ThingSpeak Communication Library for Arduino along with setTwitter, setStatus and setCreatedAt functionalities.
  
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

/* This test case checks for the following:
    - set float value
    - set int value
    - set long value
    - set NAN
    - set const char * string value
    - set String class instance value
    - set INFINITY
    - set NULL
    - multiple write
    - rate limit error
    - invalid field set  
*/
test(setFieldCase) 
{
  float floatVal = 3.14159;
  int intVal = -47;
  long longVal = 100000L;
  float nanVal = NAN;
  float infVal = INFINITY;
  const char * constCharVal = "foobar";
  String stringVal = "barfoo";
  // Always wait to ensure that rate limit isn't hit
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // No fields set
  assertEqual(TS_ERR_SETFIELD_NOT_CALLED, ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));

  // Test basic multi-field write
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD1,floatVal));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD2,intVal));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD3,longVal));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD4,nanVal));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD5,constCharVal));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD6,stringVal));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD7,infVal));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD8,NULL));
  assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));

  // Test write when not enough time has elapsed
  assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(1,(float)3.14159));  // float
  assertEqual(TS_ERR_NOT_INSERTED,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));
  
  // Allow enough time to pass to make sure that it would work 
  delay(WRITE_DELAY_FOR_THINGSPEAK);

  // Test write to field out of range
  assertEqual(TS_ERR_INVALID_FIELD_NUM,ThingSpeak.setField(FIELD0,floatVal)); 
  assertEqual(TS_ERR_INVALID_FIELD_NUM,ThingSpeak.setField(FIELD9,floatVal)); 
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_SAMD_MKR1000)  // Only the mega and mkr1000 has enough memory for all these tests
  /* This test case checks the following:
      - max/min values of float
      - high precision values of float
      - NaN and Inf values
      - out of range values of float
  */
  test(setFieldFloatCase) 
  {
    float val = -999999000000;
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test max/min range values
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, val));
    val = 999999000000;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, val));
    
    // Test high precision values
    val = 3.14159;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD3, val));
    val = -3.14159;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD4, val));
  
    // Test passing NaN and Inf
    val = NAN;
    assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD5,val));
    val = INFINITY;
    assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD6,val));
    val = -INFINITY;
    assertEqual(TS_OK_SUCCESS,ThingSpeak.setField(FIELD7,val));
    
    // Test out of range values
    val = -1000000000000.0;
    assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.setField(FIELD8, val));
    val = 1000000000000.0;
    assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.setField(FIELD8, val));
    
    assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));
  }

  /* This test case checks for the following:
      - max/min range values of int
  */
  test(setFieldIntCase) 
  {
    int val = -32768;
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test max/min range values
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, val));
    val = 32767;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, val));
    
    assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));
  }

  /* This test case checks for the following:
      - max/min range values of long
  */
  test(setFieldLongCase) 
  {
    long val = -2147483648L;
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test max range values
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, val));
    val = 2147483647L;
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, val));
    
    assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey)); 
  }
  
  /* This test case checks for the following:
      - emptry string values (String class and const char *)
      - field character limits via String (positive and negative cases)
  */
  test(setFieldStringCase) 
  {
    const char * constChar_val = "";
    String string_val = "";
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test empty string
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD1, string_val));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD2, constChar_val));
  
    unsigned int numChar = 256;
    String longString;
    longString.reserve(numChar);
  
    // Test field character limits via string
    for(unsigned int i = 0; i < 255; i++)
    {
      longString += '0';
    }
  
    // longString will have 256 '0' characters, since the number of characters ThingSpeak field can store is limited to 256 characters.
    
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setField(FIELD3, longString));
  
    longString += '0';
  
    // longString will now have 257 '0' characters. Out of range error.
    
    assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.setField(FIELD4, longString));
    
    assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));
  }
  
  /* This test case checks for the following:
      - emptry string values (String class and const char *)
      - status character limits via String (positive and negative cases)
  */
  test(setStatusCase)
  {
    const char * constChar_status = "";
    String string_status = "";
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test empty string
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setStatus(constChar_status));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setStatus(string_status));
  
    unsigned int numChar = 256;
    String longStatus;
    longStatus.reserve(numChar);
  
    // Test status character limits via string
    for(unsigned int i = 0; i < 255; i++)
    {
      longStatus += '0';
    }
  
    // longStatus will have 256 '0' characters, since the number of characters ThingSpeak status can store is limited to 256 characters.
    
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setStatus(longStatus));
    
    longStatus += '0';
  
    // longString will now have 257 '0' characters. Out of range error.
    
    assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.setStatus(longStatus));
    
    assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));
  }
  
  /* This test case checks for the following:
      - emptry string values (String class and const char *)
      - twitter handle and tweet character limits via String (positive and negative cases)
  */
  test(setTwitterTweetCase)
  {
    const char * constChar_twitter = "";
    String string_twitter = "";
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test empty string
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setTwitterTweet(string_twitter, string_twitter)); // First argument is the twitter handle, second argument is the tweet
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setTwitterTweet(constChar_twitter, constChar_twitter)); // First argument is the twitter handle, second argument is the tweet
  
    unsigned int numChar = 256;
    String longTwitter;
    longTwitter.reserve(numChar);
  
    String normalTwitter = "normalString";
  
    // Test twitter character limits
    for(unsigned int i = 0; i < 255; i++)
    {
      longTwitter += '0';
    }
  
    // longTwitter will have 256 '0' characters, since the number of characters ThingSpeak status can store is limited to 256 characters.
    
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setTwitterTweet(longTwitter, normalTwitter)); // First argument is the twitter handle, second argument is the tweet
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setTwitterTweet(normalTwitter, longTwitter)); // First argument is the twitter handle, second argument is the tweet
  
    longTwitter += '0';
  
    // longTwitter will now have 257 '0' characters. Out of range error.
    
    assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.setTwitterTweet(longTwitter, normalTwitter)); // First argument is the twitter handle, second argument is the tweet
    assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.setTwitterTweet(normalTwitter, longTwitter)); // First argument is the twitter handle, second argument is the tweet
    
    assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));
  }
  
  /* This test case checks for the following:
      - emptry string values (String class and const char *)
      - CreatedAt character limits via String (negative case)
      - ISO 8601 format timestamp value
  */
  test(setCreatedAtCase)
  {
    const char * constChar_time = "";
    String string_time = "";
    
    // Always wait to ensure that rate limit isn't hit
    delay(WRITE_DELAY_FOR_THINGSPEAK);
  
    // Test empty string
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setCreatedAt(string_time));
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setCreatedAt(constChar_time));
  
    unsigned int numChar = 256;
    String longTime;
    longTime.reserve(numChar);
  
    // Test createdAt character limits
    longTime.reserve(numChar);
    for(unsigned int i = 0; i < numChar; i++)
    {
      longTime += '0';
    }
  
    // longTime will have 257 '0' characters. Out of range error.
    
    assertEqual(TS_ERR_OUT_OF_RANGE, ThingSpeak.setCreatedAt(longTime));
  
    // Test timestamp string
    longTime = "2020-19-06 04:47:10"; // ISO 8601 format
    assertEqual(TS_OK_SUCCESS, ThingSpeak.setCreatedAt(longTime));
    
    assertEqual(TS_OK_SUCCESS,ThingSpeak.writeFields(testChannelNumber, testChannelWriteAPIKey));
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
