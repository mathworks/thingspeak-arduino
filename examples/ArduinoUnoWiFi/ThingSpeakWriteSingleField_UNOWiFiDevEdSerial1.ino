/*
  This example merges ThingSpeak > Arduino WiFi Shield > WriteSingleField example
  with UNO WiFi Developer Edition Serial1 > WiFiLinkWebClient example.

  created 19 April 2020
  by Luigi Russi

*/

/*
 WiFiLink example: WebClient
 https://github.com/jandrassy/arduino-library-wifilink
*/

/*
  WriteSingleField
  
  Description: Writes a value to a channel on ThingSpeak every 20 seconds.
  
  Hardware: Arduino WiFi Shield
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires the WiFi library
  - Ensure the WiFi Shield has the latest firmware. Instruction to update can be found here: https://www.arduino.cc/en/Hacking/WiFiShieldFirmwareUpgrading
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2018, The MathWorks, Inc.
*/

#include "ThingSpeak.h"
//#include <WiFi.h>
#include "secrets.h"

#include <UnoWiFiDevEdSerial1.h>
#include "WiFiLink.h"


//char ssid[] = SECRET_SSID;   // your network SSID (name) 
//char pass[] = SECRET_PASS;   // your network password
//int keyIndex = 0;            // your network key Index number (needed only for WEP)

WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  Serial1.begin(115200);
  Serial1.resetESP();
  delay(3000); //wait while WiFiLink firmware connects to WiFi with Web Panel settings

  WiFi.init(&Serial1);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
  }
  Serial.println("You're connected to the network");
  printWifiStatus();
  Serial.println();
  
  //while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  //}

//   check for the presence of the shield:
//  if (WiFi.status() == WL_NO_SHIELD) {
//    Serial.println("WiFi shield not present");
//     don't continue:
//    while (true);
//  }

//  String fv = WiFi.firmwareVersion();
//  if (fv != "1.1.0") {
//    Serial.println("Please upgrade the firmware");
//  }
    
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
//  if(WiFi.status() != WL_CONNECTED){
//    Serial.print("Attempting to connect to SSID: ");
//    Serial.println(SECRET_SSID);
//    while(WiFi.status() != WL_CONNECTED){
//      WiFi.begin(ssid, pass);
//      Serial.print(".");
//      delay(5000);     
//    } 
//    Serial.println("\nConnected.");
//  }
  
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
  // Try to play with delay to avoid: "Problem updating channel. HTTP error code 0" message in serial monitor.
  // See also https://uk.mathworks.com/matlabcentral/answers/483892-thingspeak-http-error-code-0-but-data-is-inserted

}

void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
