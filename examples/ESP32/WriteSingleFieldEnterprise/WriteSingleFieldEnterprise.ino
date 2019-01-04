/*|----------------------------------------------------------|*/
/*|THINGSPEAK ENTERPRISE CHANNEL WRITER                      |*/
/*|EMAIL: martinius96@gmail.com                              |*/
/*|NOT TESTED OFFICIALY YET                                  |*/
/*|CORE: June 2018                                           |*/
/*|----------------------------------------------------------|*/
#include <WiFi.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include "ThingSpeak.h" //Thingspeak library
// PRIVATE CREDENTIALS 
#define SECRET_CH_ID 000000     // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "XYZ"   // replace XYZ with your channel write API Key
#define EAP_ANONYMOUS_IDENTITY "anonymous@example.com"
#define EAP_IDENTITY "nickname@example.com"
#define EAP_PASSWORD "password"
// END PRIVATE CREDENTIALS
const char* ssid = "eduroam"; // Eduroam SSID
WiFiClient  client;
int counter = 0;
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;
int number = 0;

void setup() {
  Serial.begin(115200);  //Initialize serial
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY)); 
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
  WiFi.begin(ssid); //connect to wifi network under Enterprise 802.1x
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if(counter>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
}
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: "); 
  Serial.println(WiFi.localIP()); //print LAN IP
}

void loop() {
int statusCode = 0;
  // Connect or reconnect to WiFi
 if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid);
  }else{
    if(counter!=0){
      counter = 0;
    }
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if(counter>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  statusCode = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
  if(statusCode == 200){
    Serial.println("Channel was updated! OK!");
  }else if(statusCode == 404){
    Serial.println("Incorrect API key (or invalid ThingSpeak server address)");
  }else if(statusCode == -101){
    Serial.println("Value is out of range or string is too long (> 255 characters)");
  }else if(statusCode == -201){
    Serial.println("  Invalid field number specified");
  }else if(statusCode == -210){
    Serial.println("setField() was not called before writeFields()");
  }else if(statusCode == -301){
    Serial.println("Failed to connect to ThingSpeak");
  }else if(statusCode == -302){
    Serial.println("Unexpected failure during write to ThingSpeak");
  }else if(statusCode == -303){
    Serial.println("Unable to parse response");
  }else if(statusCode == -304 ){
    Serial.println("Timeout waiting for server to respond");
  }else if(statusCode == -401){
    Serial.println("Point was not inserted (most probable cause is the rate limit of once every 15 seconds)");
  }else{
    Serial.println("Unexpected error!"); 
  }  
  number++;
  if(number > 99){
    number = 0;
  }
  delay(20000); // Wait 20 seconds to update the channel again
}
