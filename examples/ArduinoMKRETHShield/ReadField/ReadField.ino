/*
  ReadField
  
  Description: Demonstates reading from a public channel which requires no API key and reading from a private channel which requires a read API key.
               The value read from the public channel is the current outside temperature at MathWorks headquaters in Natick, MA.  The value from the
               private channel is an example counter that increments every 10 seconds.
  
  Hardware: Arduino MKR ETH Shield
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires the Ethernet library
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

#include <Ethernet.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

byte mac[] = SECRET_MAC;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

EthernetClient client;

// Weather station channel details
unsigned long weatherStationChannelNumber = SECRET_CH_ID_WEATHER_STATION;
unsigned int temperatureFieldNumber = 4;

// Counting channel details
unsigned long counterChannelNumber = SECRET_CH_ID_COUNTER;
const char * myCounterReadAPIKey = SECRET_READ_APIKEY_COUNTER;
unsigned int counterFieldNumber = 1; 

void setup() {
  Ethernet.init(5);   // MKR ETH shield
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
      
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  int statusCode = 0;
  
  // Read in field 4 of the public channel recording the temperature
  float temperatureInF = ThingSpeak.readFloatField(weatherStationChannelNumber, temperatureFieldNumber);  

  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200){
    Serial.println("Temperature at MathWorks HQ: " + String(temperatureInF) + " deg F");
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  }
  
  delay(15000); // No need to read the temperature too often.

  // Read in field 1 of the private channel which is a counter  
  long count = ThingSpeak.readLongField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);  

   // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200){
    Serial.println("Counter: " + String(count));
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  }
  
  delay(15000); // No need to read the counter too often.
  
}
