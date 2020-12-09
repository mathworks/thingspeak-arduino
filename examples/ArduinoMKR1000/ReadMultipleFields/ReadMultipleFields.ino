/*
  ReadMultipleFields
  
  Description: Demonstates reading from a public channel which requires no API key (reading from a private channel requires a read API key).
               The values read from the public channel is the current wind direction, wind speed, humidity, outside temperature, rain, pressure, 
               power level, and light intensity  at MathWorks headquaters in Natick, MA.The functionality also provides us to read the 
               status message, location coordinates, and created-at timestamp associated with the latest feed.
  
  Hardware: Arduino MKR1000
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires WiFi101 library version 0.15.3 or newer.
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

#include <WiFi101.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Weather station channel details
unsigned long weatherStationChannelNumber = SECRET_CH_ID_WEATHER_STATION;

int statusCode = 0;
int field[8] = {1,2,3,4,5,6,7,8};

void setup() {
  Serial.begin(115200);      // Initialize serial 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
    
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  // Connect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected");
  }
}

void loop() {
    
    // Read and store all the latest field values, location coordinates, status message, and created-at timestamp
    // use ThingSpeak.readMultipleFields(channelNumber, readAPIKey) for private channels
    statusCode = ThingSpeak.readMultipleFields(weatherStationChannelNumber);
    
    if(statusCode == 200)
    {
      // Fetch the stored data

      int windDir = ThingSpeak.getFieldAsInt(field[0]); // Field 1
      float windSpeed = ThingSpeak.getFieldAsFloat(field[1]); // Field 2
      int percentHumid = ThingSpeak.getFieldAsInt(field[2]); // Field 3
      float tempInF = ThingSpeak.getFieldAsFloat(field[3]); // Field 4
      float rainInchPerMin = ThingSpeak.getFieldAsFloat(field[4]); // Field 5
      float pressureInHg = ThingSpeak.getFieldAsFloat(field[5]); // Field 6
      float powerLevel = ThingSpeak.getFieldAsFloat(field[6]); // Field 7
      int lightIntensity = ThingSpeak.getFieldAsInt(field[7]); // Field 8
      String statusMessage = ThingSpeak.getStatus(); // Status message
      String latitude = ThingSpeak.getLatitude(); // Latitude
      String longitude = ThingSpeak.getLongitude(); // Longitude
      String elevation = ThingSpeak.getElevation(); // Elevation
      String createdAt = ThingSpeak.getCreatedAt(); // Created-at timestamp
      
      Serial.println("Wind Direction (North = 0 degrees): " + String(windDir));
      Serial.println("Wind Speed (mph): " + String(windSpeed));
      Serial.println("% Humidity: " + String(percentHumid));
      Serial.println("Temperature (F): " + String(tempInF));
      Serial.println("Rain (Inches/minute): " + String(rainInchPerMin));
      Serial.println("Pressure (\"Hg): " + String(pressureInHg));
      Serial.println("Power Level (V): " + String(powerLevel));
      Serial.println("Light Intensity: " + String(lightIntensity));
      Serial.println("Status Message, if any: " + statusMessage);
      Serial.println("Latitude, if any (+ve is North, -ve is South): " + latitude);
      Serial.println("Longitude, if any (+ve is East, -ve is West): " + longitude);
      Serial.println("Elevation, if any (meters above sea level): " + elevation);
      Serial.println("Created at, if any (YYYY-MM-DD hh:mm:ss): " + createdAt);
    }
    else{
      Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
    }

    Serial.println();
    delay(5000); // no need to fetch too often
       
}
