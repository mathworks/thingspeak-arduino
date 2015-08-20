/*
  ReadWeatherStation
  
  Reads the latest weather data every 60 seconds from the public MathWorks
  weather station in Natick, MA https://thingspeak.com/channels/12397 on ThingSpeak.
  
  ThingSpeak ( https://www.thingspeak.com ) is a free IoT service for prototyping
  systems that collect, analyze, and react to their environments.
  
  Copyright 2015, The MathWorks, Inc.
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the extras/documentation folder where the library was installed.
  See the accompaning licence file for licensing information.
*/

#include "ThingSpeak.h"

/// ***********************************************************************************************************
// This example selects the correct library to use based on the board selected under the Tools menu in the IDE.
// Yun, Wired Ethernet shield, wi-fi shield, and Spark are all supported.
// With Uno and Mega, the default is that you're using a wired ethernet shield (http://www.arduino.cc/en/Main/ArduinoEthernetShield)
// If you're using a wi-fi shield (http://www.arduino.cc/en/Main/ArduinoWiFiShield), uncomment the line below
// ***********************************************************************************************************
//#define USE_WIFI_SHIELD
#ifdef ARDUINO_ARCH_AVR

  #ifdef ARDUINO_AVR_YUN
    #include "YunClient.h"
    YunClient client;
  #else

    #ifdef USE_WIFI_SHIELD
      #include <SPI.h>
      #include <WiFi.h>
      char ssid[] = "<YOURNETWORK>";          //  your network SSID (name) 
      char pass[] = "<YOURPASSWORD>";   // your network password
      int status = WL_IDLE_STATUS;
      WiFiClient  client;
    #else
      // Use wired ethernet shield
      #include <SPI.h>
      #include <Ethernet.h>
      byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
      EthernetClient client;
    #endif
  #endif
#endif

// On the Particle Core and Photon, the results are published to the Particle dashboard using events.
// Go to http://dashboard.particle.io, click on the logs tab, and you'll see the events coming in. 
#ifdef SPARK
  TCPClient client;
#endif

/*
  This is the ThingSpeak channel number for the MathwWorks weather station
  https://thingspeak.com/channels/12397.  It senses a number of things and puts them in the eight
  field of the channel:
  Field 1 - Wind Direction (degrees where 0 is North)
  Field 2 - Wind Speed (MPH)
  Field 3 - Humidity (%RH)
  Field 4 - Temperature (Degrees F)
  Field 5 - Rainfall (inches since last measurement)
  Field 6 - Atmospheric Pressure (inHg)
*/
unsigned long weatherStationChannelNumber = 12397;

void setup() {
  #ifdef ARDUINO_ARCH_AVR
    Serial.begin(9600);
    #ifdef ARDUINO_AVR_YUN
      Bridge.begin();
    #else
      #ifdef USE_WIFI_SHIELD
        WiFi.begin(ssid, pass);
      #else
        Ethernet.begin(mac);
      #endif
    #endif
  #endif
  
  ThingSpeak.begin(client);
}

void loop() {
  float windDirection = ThingSpeak.readFloatField(weatherStationChannelNumber,1);
  float windSpeed = ThingSpeak.readFloatField(weatherStationChannelNumber,2);
  float humidity = ThingSpeak.readFloatField(weatherStationChannelNumber,3);
  float temperature = ThingSpeak.readFloatField(weatherStationChannelNumber,4);
  float rainfall = ThingSpeak.readFloatField(weatherStationChannelNumber,5);
  float pressure = ThingSpeak.readFloatField(weatherStationChannelNumber,6);

  #ifdef ARDUINO_ARCH_AVR
    Serial.println("======================================"); 
    Serial.println("Current weather conditions in Natick: "); 
    Serial.print(temperature);
    Serial.print(" degrees F, "); 
    Serial.print(humidity);
    Serial.println("% humidity"); 
    Serial.print("Wind at ");
    Serial.print(windSpeed);
    Serial.print(" MPH at "); 
    Serial.print(windDirection);
    Serial.println(" degrees"); 
    Serial.print("Pressure is ");
    Serial.print(pressure);
    Serial.print(" inHg");
    if(rainfall > 0)
    {
    	Serial.print(", and it's raining"); 
    }
    Serial.println(); 
  #endif
  #ifdef SPARK
    Spark.publish("thingspeak-weather", "Current weather conditions in Natick: ",60,PRIVATE);
    Spark.publish("thingspeak-weather", String(temperature) + " degrees F, " + String(humidity) + "% humidity",60,PRIVATE); 
    Spark.publish("thingspeak-weather", "Wind at " + String(windSpeed) + " MPH at " + String (windDirection) + " degrees",60,PRIVATE); 
    if(rainfall > 0)
    {
      Spark.publish("thingspeak-weather", "Pressure is " + String(pressure) + " inHg, and it's raining",60,PRIVATE);
    }
    else
    {
      Spark.publish("thingspeak-weather", "Pressure is " + String(pressure) + " inHg",60,PRIVATE);
    }
  #endif

  delay(60000); // Note that the weather station only updates once a minute

}
