# ThingSpeak Communication Library for Arduino, ESP8266 and ESP32

This library enables an Arduino or other compatible hardware to write or read data to or from ThingSpeak, an open data platform for the Internet of Things with MATLAB analytics and visualization.

Hardware specific <a href="http://github.com/mathworks/thingspeak-arduino/tree/master/examples">examples</a> are found here. But to give you an idea of usage examples for <a href="#typical_write">writing</a> and <a href="#typical_read">reading</a> with an ESP8266 are shown below. Complete <a href="#documentation">documentation</a> in also shown below.

ThingSpeak offers free data storage and analysis of time-stamped numeric or alphanumeric data. Users can access ThingSpeak by visiting http://thingspeak.com and creating a ThingSpeak user account.

ThingSpeak stores data in channels. Channels support an unlimited number of timestamped observations (think of these as rows in a spreadsheet). Each channel has up to 8 fields (think of these as columns in a speadsheet). Check out this [video](https://www.mathworks.com/videos/introduction-to-thingspeak-107749.html) for an overview.

Channels may be public, where anyone can see the data, or private, where only the owner and select users can read the data. Each channel has an associated Write API Key that is used to control who can write to a channel. In addition, private channels have one or more Read API Keys to control who can read from private channel. An API Key is not required to read from public channels.  Each channel can have up to 8 fields. One field is created by default.

You can visualize and do online analytics of your data on ThingSpeak using the built in version of MATLAB, or use the desktop version of MATLAB to get deeper historical insight. Visit https://www.mathworks.com/hardware-support/thingspeak.html to learn more.

Libraries and examples for Particle devices can be found here: https://github.com/mathworks/thingspeak-particle

## Installation
In the Arduino IDE, choose Sketch/Include Library/Manage Libraries.  Click the ThingSpeak Library from the list, and click the Install button.

--- or ---

1. Download the ZIP file (below) to your machine.
2. In the Arduino IDE, choose Sketch/Include Library/Add Zip Library
3. Navigate to the ZIP file, and click Open

## Compatible Hardware:

* Arduino/Genuino or compatible using a WiFi Shield
* Arduino/Genuino or compatible using a WiFi Shield 101 (Use the WiFi101 library version 0.13.0 or older.)
* Arduino/Genuino or compatible using an Ethernet Shield
* Arduino/Genuino or compatible using a MKR ETH Shield
* Arduino MKR1000 
* Arduino MKR1010
* Arduino VIDOR 4000
* Arduino GSM 14000
* Arduino Uno WiFi Rev2
* Arduino Yún (Rev1 and Rev2)
* ESP8266 programming directly (tested with SparkFun ESP8266 Thing - Dev Board and NodeMCU 1.0 module)
* ESP8266 via AT commands 
* ESP32 (tested with SparkFun ESP32 Thing)

# Examples

The library includes several <a href="http://github.com/mathworks/thingspeak-arduino/tree/master/examples">examples organized by board type</a> to help you get started. These are accessible in Examples > ThingSpeak menu of the Arduino IDE.

* **ReadField:** Reading from a public channel and a private channel on ThingSpeak.
* **WriteSingleField:** Writing a value to a single field on ThingSpeak.
* **WriteMultipleFields:** Writing values to multiple fields and status in one transaction with ThingSpeak.
* **ReadMultipleFields:** Reading values from multiple fields, status, location, created-at timestamp from a public channel on ThingSpeak
* **SecureConnect:** Using the above features and connecting securely to ThingSpeak.

## <a id="typical_write">Typical Write Example</a>
In this case, write to a field with an ESP8266 with an incrementing number.   

```
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 0;

void setup() {
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  WiFi.mode(WIFI_STA); 
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
```

## <a id="typical_read">Typical Read Example</a>
In this case, read from a public channel and a private channel with an ESP8266.  The public channel is the temperature(F) at MathWorks headquarters.  The private channel is a counter that increments.

 ```
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Weather station channel details
unsigned long weatherStationChannelNumber = SECRET_CH_ID_WEATHER_STATION;
unsigned int temperatureFieldNumber = 4;

// Counting channel details
unsigned long counterChannelNumber = SECRET_CH_ID_COUNTER;
const char * myCounterReadAPIKey = SECRET_READ_APIKEY_COUNTER;
unsigned int counterFieldNumber = 1; 

void setup() {
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  int statusCode = 0;
  
  // Connect or reconnect to WiFi
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
 ```

# <a id="documentation">Documentation</a>

## begin
Initializes the ThingSpeak library and network settings, whether performing a secure connection or a normal connection to ThingSpeak.
```
bool begin (client) // defaults to ThingSpeak.com
```

| Parameter      | Type         | Description                                            |          
|----------------|:-------------|:-------------------------------------------------------|
| client         | Client &     | TCPClient created earlier in the sketch                |

### Returns
Always returns true. This does not validate the information passed in, or generate any calls to ThingSpeak.

### Remarks
use ```#define TS_ENABLE_SSL``` before ```#include <thingspeak.h>``` so as to perform a secure connection by passing a client that is capable of doing SSL. See the note regarding secure connection below.

## writeField
Write a value to a single field in a ThingSpeak channel.
```
int writeField(channelNumber, field, value, writeAPIKey)
```

| Parameter     | Type          | Description                                                                                     |          
|---------------|:--------------|:------------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                  |
| field         | unsigned int  | Field number (1-8) within the channel to write to.                                              |
| value         | int           | Integer value (from -32,768 to 32,767) to write.                                                |
|               | long          | Long value (from -2,147,483,648 to 2,147,483,647) to write.                                     |
|               | float         | Floating point value (from -999999000000 to 999999000000) to write.                             |
|               | String        | String to write (UTF8 string). ThingSpeak limits this field to 255 bytes.                       |
|               | const char *  | Character array (zero terminated) to write (UTF8). ThingSpeak limits this field to 255 bytes.   |
| writeAPIKey   | const char *  | Write API key associated with the channel. If you share code with others, do not share this key |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

### Remarks
Special characters will be automatically encoded by this method. See the note regarding special characters below.

## writeFields
Write a multi-field update. Call setField() for each of the fields you want to write first. 
```
int writeFields (channelNumber, writeAPIKey)	
```

| Parameter     | Type          | Description                                                                                     |          
|---------------|:--------------|:------------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                  |
| writeAPIKey   | const char *  | Write API key associated with the channel. If you share code with others, do not share this key |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

### Remarks
Special characters will be automatically encoded by this method. See the note regarding special characters below.

## writeRaw
Write a raw POST to a ThingSpeak channel. 
```
int writeRaw (channelNumber, postMessage, writeAPIKey)	
```

| Parameter     | Type          | Description                                                                                                                                       |          
|---------------|:--------------|:--------------------------------------------------------------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                                                                    |
| postMessage   | const char *  | Raw URL to write to ThingSpeak as a String. See the documentation at https://thingspeak.com/docs/channels#update_feed.                            |
|               | String        | Raw URL to write to ThingSpeak as a character array (zero terminated). See the documentation at https://thingspeak.com/docs/channels#update_feed. | 
| writeAPIKey   | const char *  | Write API key associated with the channel. If you share code with others, do not share this key                                                   |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

### Remarks
This method will not encode special characters in the post message.  Use '%XX' URL encoding to send special characters. See the note regarding special characters below.

## setField
Set the value of a single field that will be part of a multi-field update.
```
int setField (field, value)
```

| Parameter | Type         | Description                                                                                   |          
|-----------|:-------------|:----------------------------------------------------------------------------------------------|
| field     | unsigned int | Field number (1-8) within the channel to set                                                  |
| value     | int          | Integer value (from -32,768 to 32,767) to write.                                              |
|           | long         | Long value (from -2,147,483,648 to 2,147,483,647) to write.                                   |
|           | float        | Floating point value (from -999999000000 to 999999000000) to write.                           |
|           | String       | String to write (UTF8 string). ThingSpeak limits this field to 255 bytes.                     |
|           | const char * | Character array (zero terminated) to write (UTF8). ThingSpeak limits this field to 255 bytes. |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

## setStatus
Set the status of a multi-field update. Use status to provide additonal details when writing a channel update. Additionally, status can be used by the ThingTweet App to send a message to Twitter.
```
int setStatus (status)	
```

| Parameter | Type      | Description                                                                   |          
|--------|:-------------|:------------------------------------------------------------------------------|
| status | const char * | String to write (UTF8). ThingSpeak limits this to 255 bytes.                  |
|        | String       | const character array (zero terminated). ThingSpeak limits this to 255 bytes. |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

## setLatitude
Set the latitude of a multi-field update.
```
int setLatitude	(latitude)	
```

| Parameter | Type  | Description                                                                |          
|-----------|:------|:---------------------------------------------------------------------------|
| latitude  | float | Latitude of the measurement (degrees N, use negative values for degrees S) |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

## setLongitude
Set the longitude of a multi-field update.
```
int setLongitude (longitude)	
```

| Parameter | Type  | Description                                                                 |          
|-----------|:------|:----------------------------------------------------------------------------|
| longitude | float | Longitude of the measurement (degrees E, use negative values for degrees W) |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

## setElevation
Set the elevation of a multi-field update.
```
int setElevation (elevation)	
```

| Parameter | Type      | Description                                         |          
|-----------|:------|:--------------------------------------------------------|
| elevation | float | 	Elevation of the measurement (meters above sea level) |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

## setCreatedAt
Set the created-at date of a multi-field update. The timestamp string must be in the ISO 8601 format. Example "2017-01-12 13:22:54"
```
int setCreatedAt (createdAt)
```

| Parameter | Type         | Description                                                                                      |          
|-----------|:-------------|:-------------------------------------------------------------------------------------------------|
| createdAt | String       | Desired timestamp to be included with the channel update as a String.                            |
|           | const char * | Desired timestamp to be included with the channel update as a character array (zero terminated). |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

### Remarks
Timezones can be set using the timezone hour offset parameter. For example, a timestamp for Eastern Standard Time is: "2017-01-12 13:22:54-05". If no timezone hour offset parameter is used, UTC time is assumed.

## setTwitterTweet
Set the Twitter account and message to use for an update to be tweeted.
```
int setTwitterTweet	(twitter, tweet)	
```

| Parameter | Type         | Description                                                                      |          
|-----------|:-------------|:---------------------------------------------------------------------------------|
| twitter   | String       | Twitter account name as a String.                                                |
|           | const char * | Twitter account name as a character array (zero terminated).                     |
| tweet     | String       | Twitter message as a String (UTF-8) limited to 140 character.                    |
|           | const char * | Twitter message as a character array (zero terminated) limited to 140 character. |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

### Remarks
Prior to using this feature, a twitter account must be linked to your ThingSpeak account. To link your twitter account. login to ThingSpeak and go to Apps -> ThingTweet and click Link Twitter Account.

## readStringField
Read the latest string from a channel. Include the readAPIKey to read a private channel.
```
String readStringField (channelNumber, field, readAPIKey)	
```
```
String readStringField (channelNumber, field)	
```

| Parameter     | Type          | Description                                                                                    |          
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                 |
| field         | unsigned int  | Field number (1-8) within the channel to read from.                                            |
| readAPIKey    | const char *  | Read API key associated with the channel. If you share code with others, do not share this key |

### Returns
Value read (UTF8 string), or empty string if there is an error.

## readFloatField
Read the latest float from a channel. Include the readAPIKey to read a private channel.
```
float readFloatField (channelNumber, field, readAPIKey)	
```
```
float readFloatField (channelNumber, field)	
```

| Parameter     | Type          | Description                                                                                    |          
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                 |
| field         | unsigned int  | Field number (1-8) within the channel to read from.                                            |
| readAPIKey    | const char *  | Read API key associated with the channel. If you share code with others, do not share this key |

### Returns
Value read, or 0 if the field is text or there is an error. Use getLastReadStatus() to get more specific information. Note that NAN, INFINITY, and -INFINITY are valid results. 

## readLongField
Read the latest long from a channel. Include the readAPIKey to read a private channel.
```
long readLongField (channelNumber, field, readAPIKey)	
```
```
long readLongField (channelNumber, field)	
```

| Parameter     | Type          | Description                                                                                    |          
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                 |
| field         | unsigned int  | Field number (1-8) within the channel to read from.                                            |
| readAPIKey    | const char *  | Read API key associated with the channel. If you share code with others, do not share this key |

### Returns
Value read, or 0 if the field is text or there is an error. Use getLastReadStatus() to get more specific information. 

## readIntField
Read the latest int from a channel. Include the readAPIKey to read a private channel.
```
int readIntField (channelNumber, field, readAPIKey)		
```
```
int readIntField (channelNumber, field)		
```

| Parameter     | Type          | Description                                                                                    |          
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                 |
| field         | unsigned int  | Field number (1-8) within the channel to read from.                                            |
| readAPIKey    | const char *  | Read API key associated with the channel. If you share code with others, do not share this key |

### Returns
Value read, or 0 if the field is text or there is an error. Use getLastReadStatus() to get more specific information. If the value returned is out of range for an int, the result is undefined. 

## readStatus
Read the latest status from a channel. Include the readAPIKey to read a private channel.
```
String readStatus (channelNumber, readAPIKey)	
```
```
String readStatus (channelNumber)
```

| Parameter     | Type          | Description                                                                                    |
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                 |
| readAPIKey    | const char *  | Read API key associated with the channel. If you share code with others, do not share this key |

### Returns
Returns the status field as a String.

## String readCreatedAt()
Read the created-at timestamp associated with the latest update to a channel. Include the readAPIKey to read a private channel.
```
String readCreatedAt (channelNumber, readAPIKey)
```
```
String readCreatedAt (channelNumber)	
```

| Parameter     | Type          | Description                                                                                    |
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                 |
| readAPIKey    | const char *  | Read API key associated with the channel. If you share code with others, do not share this key |

### Returns
Returns the created-at timestamp as a String.

## readRaw
Read a raw response from a channel. Include the readAPIKey to read a private channel.
```
String readRaw (channelNumber, URLSuffix, readAPIKey)	
```
```
String readRaw	(channelNumber, URLSuffix)
```

| Parameter     | Type          | Description                                                                                                        |
|---------------|:--------------|:-------------------------------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                                     |
| URLSuffix     | String        | Raw URL to write to ThingSpeak as a String. See the documentation at https://thingspeak.com/docs/channels#get_feed |
| readAPIKey    | const char *  | Read API key associated with the channel. If you share code with others, do not share this key.                    |     

### Returns
Returns the raw response from a HTTP request as a String.

## readMultipleFields
Read all the latest fields, status, location, and created-at timestamp; and store these values locally. Use ```getField``` functions mentioned below to fetch the stored values. Include the readAPIKey to read a private channel.
```
int readMultipleFields (channelNumber, readAPIKey)
```
```
int readMultipleFields (channelNumber)
```

| Parameter     | Type          | Description                                                                                    |
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| channelNumber | unsigned long | Channel number                                                                                 |
| readAPIKey    | const char *  | Read API key associated with the channel. If you share code with others, do not share this key |

### Returns
HTTP status code of 200 if successful. See Return Codes below for other possible return values.

### Remarks
This feature not available in Arduino Uno due to memory constraints.

## getFieldAsString
Fetch the stored value from a field as String. Invoke this after invoking ```readMultipleFields```.
```
String getFieldAsString (field)
```
| Parameter     | Type          | Description                                                                                    |
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| field         | unsigned int  | Field number (1-8) within the channel to read from.                                            |

### Returns
Value read (UTF8 string), empty string if there is an error, or old value read (UTF8 string) if invoked before readMultipleFields().  Use getLastReadStatus() to get more specific information.

### Remarks
This feature not available in Arduino Uno due to memory constraints.

## getFieldAsFloat
Fetch the stored value from a field as Float. Invoke this after invoking ```readMultipleFields```.
```
float getFieldAsFloat (field)
```
| Parameter     | Type          | Description                                                                                    |
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| field         | unsigned int  | Field number (1-8) within the channel to read from.                                            |

### Returns
Value read, 0 if the field is text or there is an error, or old value read if invoked before readMultipleFields(). Use getLastReadStatus() to get more specific information. Note that NAN, INFINITY, and -INFINITY are valid results. 

### Remarks
This feature not available in Arduino Uno due to memory constraints.

## getFieldAsLong
Fetch the stored value from a field as Long. Invoke this after invoking ```readMultipleFields```.
```
long getFieldAsLong (field)
```
| Parameter     | Type          | Description                                                                                    |
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| field         | unsigned int  | Field number (1-8) within the channel to read from.                                            |

### Returns
Value read, 0 if the field is text or there is an error, or old value read if invoked before readMultipleFields(). Use getLastReadStatus() to get more specific information.

### Remarks
This feature not available in Arduino Uno due to memory constraints.

## getFieldAsInt
Fetch the stored value from a field as Int. Invoke this after invoking ```readMultipleFields```.
```
int getFieldAsInt (field)
```
| Parameter     | Type          | Description                                                                                    |
|---------------|:--------------|:-----------------------------------------------------------------------------------------------|
| field         | unsigned int  | Field number (1-8) within the channel to read from.                                            |

### Returns
Value read, 0 if the field is text or there is an error, or old value read if invoked before readMultipleFields(). Use getLastReadStatus() to get more specific information.

### Remarks
This feature not available in Arduino Uno due to memory constraints.

## getLastReadStatus
Get the status of the previous read.
```
int getLastReadStatus ()	
```

### Returns
See Return Codes below for other possible return values.

## Return Codes
| Value | Meaning                                                                                 |
|-------|:----------------------------------------------------------------------------------------|
| 200   | OK / Success                                                                            |
| 404   | Incorrect API key (or invalid ThingSpeak server address)                                |
| -101  | Value is out of range or string is too long (> 255 characters)                          |
| -201  | Invalid field number specified                                                          |
| -210  | setField() was not called before writeFields()                                          |
| -301  | Failed to connect to ThingSpeak                                                         |
| -302  | Unexpected failure during write to ThingSpeak                                           |
| -303  | Unable to parse response                                                                |
| -304  | Timeout waiting for server to respond                                                   |
| -401  | Point was not inserted (most probable cause is the rate limit of once every 15 seconds) |
|    0  | Other error                                                                             |

## Secure Connection
Securely connect to ThingSpeak API to use the above features and functionalities.

### HTTPS
HTTPS ensures confidentiality as well as authenticity.
Confidentiality: SSL Encryption using public-key cryptography.
Authenticity: creates "trust", the device knows whether it's connected to the actual api.thingkspeak.com and not a spoof of it.

### User Sketch Requirements
Always use ```#define TS_ENABLE_SSL``` before ```#include <thingspeak.h>``` so as to perform a secure connection. If not, then the default connection would be insecured HTTP.

#### Confidentiality without Authenticity
Case 1: TS_ENABLE_SSL macro defined + Client capable of doing SSL = Secure HTTPS Connection <br>
Case 2: TS_ENABLE_SSL macro defined + Client not capable of SSL = Defualt HTTP connection with a warning message sent to the user <br>
Case 3: TS_ENABLE_SSL macro undefined + Client capable of doing SSL = Error connecting to ThingSpeak status code returned to user <br>
Case 4: TS_ENABLE_SSL macro undefined + Client not capable of SSL =  HTTP connection

#### Confidentiality + Authenticity
Different client libraries have different methods of performing authenticity. <br>
Some ways: Root Certificate Check, Certificate Fingerprint Check. <br>
Perform the fingerprint and/or certificate check prior to invoking the ```begin()``` function. <br>
The certificate has an expiration date associated with it, and hence it's the user's responsibility to fetch the updated certificate for the Confidentiality + Authenticity HTTPS connection to be established. <br>
See the ReadMultipleFieldsSecure example on Fingerprint check HTTPS connection using ESP8266.
See the ReadMultipleFieldsSecure example on Root Certificate check HTTPS connection using ESP32.

## Special Characters
Some characters require '%XX' style URL encoding before sending to ThingSpeak.  The writeField() and writeFields() methods will perform the encoding automatically.  The writeRaw() method will not.

| Character  | Encoding |
|------------|:---------|
|     "      | %22      |
|     %      | %25      |
|     &      | %26      |
|     +      | %2B      |
|     ;      | %3B      |

Control characters, ASCII values 0 though 31, are not accepted by ThingSpeak and will be ignored.  Extended ASCII characters with values above 127 will also be ignored. 
