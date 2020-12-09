/*
  WriteSingleField
  
  Description: Writes a value to a channel on ThingSpeak every 20 seconds.
  
  Hardware: Arduino Yún, Yún Rev.2
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!

  Notes:
    - This example works with both Ethernet and WiFi
    - For WiFi with the Yún Rev.2, configure the SSID and password through the Access Point portal.
      See https://www.arduino.cc/en/Guide/ArduinoYunRev2 for details.
    
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

#include <BridgeClient.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

BridgeClient client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 0;

void setup() {
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  Bridge.begin();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // change the value
  number++;
  if(number > 99){
    number = 0;
  }
  
  delay(20000); // Wait 20 seconds to update the channel again
}
