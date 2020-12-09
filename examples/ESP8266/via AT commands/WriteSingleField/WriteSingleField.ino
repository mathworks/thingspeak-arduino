/*
  WriteSingleField
  
  Description: Writes a value to a channel on ThingSpeak every 20 seconds.
  
  Hardware: Arduino compatible hardware controlling ESP8266 through AT commands. THIS IS CODE FOR THE ARDUINO, NOT THE ESP8266!
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires Arduino main board connected to ESP8266 (ESP-01) serial pins
  - The ESP8266 device must be running firmware capable of AT commands over TX/RX pins. 
      Details on reflashing and binaries can be found here: https://www.espressif.com/en/support/download/at  
  - Requires WiFiEsp library which is available through the Library Manager. The logging level for the WiFiEsp library is set to INFO.
      To disable logging completely, set _ESPLOGLEVEL_ to 0 in \Arduino\libraries\WiFiEsp\src\utility\debug.h
  - Use TX1/RX1 if present on Arduino header (Mega, Due, etc). If not, then connect TX to pin 7, RX to pin 6.
  - Some boards (Uno, Nano, Leonardo, etc) do not have enough memory to handle writing large strings to ThingSpeak.
      For these boards, keep individual field data lengths 32 characters or less.  
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  Wiring diagrams are available at:
  SoftSerial (Uno, Nano, Mini, etc): https://github.com/mathworks/thingspeak-arduino/blob/master/ESP-01_AT_Commands_SoftSerial_Hookup.pdf
  Hardware Serial1 (Mega, Leonardo, Due) - https://github.com/mathworks/thingspeak-arduino/blob/master/ESP-01_AT_Commands_Hardware_Serial_Hookup.pdf
  
    ESP8266 | Arduino without Serial1 | Arduino with Serial1  
    --------------------------------------------------------
       RX   |         pin 7           |        TX1
       TX   |         pin 6           |        RX1
       GND  |          GND            |        GND
       VCC  |          5V             |        5V
      CH_PD |          5V             |        5V
    
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

#include "WiFiEsp.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;    //  your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiEspClient  client;

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 0;

void setup() {
  Serial.begin(115200); //Initialize serial
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }  
  
  // initialize serial for ESP module  
  setEspBaudRate(ESP_BAUDRATE);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  Serial.print("Searching for ESP8266..."); 
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.println("found it!");
    
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
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

// This function attempts to set the ESP8266 baudrate. Boards with additional hardware serial ports
// can use 115200, otherwise software serial is limited to 19200.
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);  
  }
    
  Serial1.begin(baudrate);
}
