/*
  ThingSpeak(TM) Communication Library For Arduino, ESP8266 and ESP32

  Enables an Arduino or other compatible hardware to write or read data to or from ThingSpeak,
  an open data platform for the Internet of Things with MATLAB analytics and visualization. 

  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize and 
  analyze live data streams in the cloud.
  
  Copyright 2017, The MathWorks, Inc.
 
  See the accompaning licence file for licensing information.
*/

/**
  @mainpage
 * 
 * \ref ThingSpeakClass "For technical documentation, visit this page"
 * 
 * ThingSpeak offers free data storage and analysis of time-stamped numeric or alphanumeric data.
 * Users can access ThingSpeak by visiting http://thingspeak.com and creating a ThingSpeak user account.
 *
 * ThingSpeak stores data in channels.  Channels support an unlimited number of timestamped observations (think of these as rows in a spreadsheet).  
 * Each channel has up to 8 fields (think of these as columns in a speadsheet).  Check out this <a href="http://www.mathworks.com/videos/introduction-to-thingspeak-107749.html">video</a> for an overview.
 * 
 * Channels may be public, where anyone can see the data, or private, where only the owner and select users can read the data.
 * Each channel has an associated Write API Key that is used to control who can write to a channel.  
 * In addition, private channels have one or more Read API Keys to control who can read from private channel.  
 * An API Key is not required to read from public channels.  Each channel can have up to 8 fields. One field is created by default.
 *
 * You can visualize and do online analytics of your data on ThingSpeak using the built in version of MATLAB, or use the desktop version of MATLAB to get
 * deeper historical insight.  Visit https://www.mathworks.com/hardware-support/thingspeak.html to learn more.
 * 
 * <h3>Compatible Hardware</h3>
 * * <a href="http://www.arduino.cc">Arduino/Genuino</a> or compatible using a WiFi101 or Ethernet shield (we have tested with <a href="http://www.arduino.cc/en/Main/ArduinoBoardUno">Uno</a> and <a href="http://www.arduino.cc/en/Main/ArduinoBoardMega2560">Mega</a>)
 * * <a href="http://www.arduino.cc/en/Main/ArduinoBoardYun">Arduino Yun</a> running OpenWRT-Yun Release 1.5.3 (November 13th, 2014) or later.  There are known issues with earlier versions.  Visit [this page](http://www.arduino.cc/en/Main/Software) to get the latest version.
 * * <a href="http://www.arduino.cc/en/Main/ArduinoMKR1000">Arduino MKR1000</a>
 * * ESP8266 (tested with <a href="https://www.sparkfun.com/products/13711">SparkFun ESP8266 Thing - Dev Board</a> and <a href="http://www.seeedstudio.com/depot/NodeMCU-v2-Lua-based-ESP8266-development-kit-p-2415.html">NodeMCU 1.0 module</a>)
 * * <a href="http://www.sparkfun.com/products/13907">SparkFun ESP32 Thing</a>
  * 
 * <h3>Examples</h3>
 * The library includes several examples to help you get started.  These are accessible in the Examples/ThingSpeak menu off the File menu in the Arduino IDE.
 * * <b>CheerLights:</b> Reads the latest <a href="http://www.cheerlights.com">CheerLights</a> color on ThingSpeak, and sets an RGB LED.
 * * <b>ReadLastTemperature:</b> Reads the latest temperature from the public <a href="https://thingspeak.com/channels/12397">MathWorks weather station</a> in Natick, MA on ThingSpeak.
 * * <b>ReadPrivateChannel:</b> Reads the latest voltage value from a private channel on ThingSpeak.
 * * <b>ReadWeatherStation:</b> Reads the latest weather data from the public <a href="https://thingspeak.com/channels/12397">MathWorks weather station</a> in Natick, MA on ThingSpeak.
 * * <b>WriteMultipleVoltages:</b> Reads analog voltages from pins 0-7 and writes them to the 8 fields of a channel on ThingSpeak.
 * * <b>WriteVoltage:</b> Reads an analog voltage from pin 0, converts to a voltage, and writes it to a channel on ThingSpeak.
 */

#ifndef ThingSpeak_h
#define ThingSpeak_h

//#define PRINT_DEBUG_MESSAGES
//#define PRINT_HTTP


#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_ESP32)
  #include "Arduino.h"
  #include <Client.h>
#else
  #error Only Arduino MKR1000, Yun, Uno/Mega/Due with either WiFi101 or Ethernet shield. ESP8266 and ESP32 are also supported.
#endif


#define THINGSPEAK_URL "api.thingspeak.com"
#define THINGSPEAK_IPADDRESS IPAddress(184,106,153,149)
#define THINGSPEAK_PORT_NUMBER 80

#ifdef ARDUINO_ARCH_AVR
    #ifdef ARDUINO_AVR_YUN
        #define TS_USER_AGENT "tslib-arduino/1.3 (arduino yun)"
    #else
        #define TS_USER_AGENT "tslib-arduino/1.3 (arduino uno or mega)"
    #endif
#elif defined(ARDUINO_ARCH_ESP8266)
    #define TS_USER_AGENT "tslib-arduino/1.3 (ESP8266)"
#elif defined(ARDUINO_SAMD_MKR1000)
	#define TS_USER_AGENT "tslib-arduino/1.3 (arduino mkr1000)"
#elif defined(ARDUINO_SAM_DUE)
	#define TS_USER_AGENT "tslib-arduino/1.3 (arduino due)"
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
	#define TS_USER_AGENT "tslib-arduino/1.3 (arduino unknown sam or samd)"
#elif defined(ARDUINO_ARCH_ESP32)
	#define TS_USER_AGENT "tslib-arduino/1.3 (ESP32)"
#else
	#error "Platform not supported"
#endif

#define FIELDNUM_MIN 1
#define FIELDNUM_MAX 8
#define FIELDLENGTH_MAX 255  // Max length for a field in ThingSpeak is 255 bytes (UTF-8)

#define TIMEOUT_MS_SERVERRESPONSE 5000  // Wait up to five seconds for server to respond

#define OK_SUCCESS              200     // OK / Success
#define ERR_BADAPIKEY           400     // Incorrect API key (or invalid ThingSpeak server address)
#define ERR_BADURL              404     // Incorrect API key (or invalid ThingSpeak server address)
#define ERR_OUT_OF_RANGE        -101    // Value is out of range or string is too long (> 255 bytes)
#define ERR_INVALID_FIELD_NUM   -201    // Invalid field number specified
#define ERR_SETFIELD_NOT_CALLED -210    // setField() was not called before writeFields()
#define ERR_CONNECT_FAILED      -301    // Failed to connect to ThingSpeak
#define ERR_UNEXPECTED_FAIL     -302    // Unexpected failure during write to ThingSpeak
#define ERR_BAD_RESPONSE        -303    // Unable to parse response
#define ERR_TIMEOUT             -304    // Timeout waiting for server to respond
#define ERR_NOT_INSERTED        -401    // Point was not inserted (most probable cause is the rate limit of once every 15 seconds)

/**
 * @brief Enables an Arduino, ESP8266, ESP32 or other compatible hardware to write or read data to or from ThingSpeak, an open data platform for the Internet of Things with MATLAB analytics and visualization. 
 */
class ThingSpeakClass
{
  public:
	ThingSpeakClass()
	{
		resetWriteFields();
	    this->lastReadStatus = OK_SUCCESS;
	};

	/**
	 * @brief Initializes the ThingSpeak library and network settings using a custom installation of ThingSpeak.
	 * @param client EthernetClient, YunClient, TCPClient, or WiFiClient created earlier in the sketch
	 * @param customHostName Host name of a custom install of ThingSpeak
	 * @param port Port number to use with a custom install of ThingSpeak
	 * @return Always returns true
     * @comment This does not validate the information passed in, or generate any calls to ThingSpeak.
	 * @code
		#include <SPI.h>
		#include <Ethernet.h>
		byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
		EthernetClient client;

		#include "ThingSpeak.h"

		void setup() {
		  Ethernet.begin(mac);
		  ThingSpeak.begin(client,"api.thingspeak.com", 80);
		}
	 * @endcode
	 */
    bool begin(Client & client, const char * customHostName, unsigned int port)
	{
#ifdef PRINT_DEBUG_MESSAGES
		Serial.print("ts::tsBegin    (client: Client URL: "); Serial.print(customHostName); Serial.println(")");
#endif
	this->setClient(&client);
	this->setServer(customHostName, port);
	resetWriteFields();
	this->lastReadStatus = OK_SUCCESS;
	return true;
	};

	/**
	 * @brief Initializes the ThingSpeak library and network settings using a custom installation of ThingSpeak.
	 * @param client EthernetClient, YunClient, TCPClient, or WiFiClient created earlier in the sketch
	 * @param customIP IP address of a custom install of ThingSpeak
	 * @param port Port number to use with a custom install of ThingSpeak
	 * @return Always returns true
     * @comment This does not validate the information passed in, or generate any calls to ThingSpeak.
	 * @code
		#include <SPI.h>
		#include <Ethernet.h>
		byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
		EthernetClient client;

		#include "ThingSpeak.h"

		void setup() {
		  Ethernet.begin(mac);
		  ThingSpeak.begin(client,IPAddress(184,106,153,149), 80);
		}
	 * @endcode
	 */
	bool begin(Client & client, IPAddress customIP, unsigned int port)
	{
#ifdef PRINT_DEBUG_MESSAGES
		Serial.print("ts::tsBegin    (client: Client IP: "); Serial.print(customIP); Serial.println(")");
#endif
		this->setClient(&client);
		this->setServer(customIP, port);
		resetWriteFields();
		this->lastReadStatus = OK_SUCCESS;
		return true;
	};

	/**
	 * @brief Initializes the ThingSpeak library and network settings using the ThingSpeak.com service.
	 * @param client EthernetClient, YunClient, TCPClient, or WiFiClient created earlier in the sketch
	 * @return Always returns true
     * @comment This does not validate the information passed in, or generate any calls to ThingSpeak.
	 * @code
		#include <SPI.h>
		#include <Ethernet.h>
		byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
		EthernetClient client;

		#include "ThingSpeak.h"

		void setup() {
		  Ethernet.begin(mac);
		  ThingSpeak.begin(client);
		}
	 * @endcode
	 */
	bool begin(Client & client)
	{
#ifdef PRINT_DEBUG_MESSAGES
		Serial.print("ts::tsBegin");
#endif
		this->setClient(&client);
		this->setServer();
		resetWriteFields();
		this->lastReadStatus = OK_SUCCESS;
		return true;
	};
	
	/**
	 * @brief Write an integer value to a single field in a ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to write to.
	 * @param value Integer value (from -32,768 to 32,767) to write.
	 * @param writeAPIKey Write API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Visit https://thingspeak.com/docs/channels for more information about channels, API keys, and fields.  ThingSpeak limits the number of writes to a channel to once every 15 seconds.
	 * @code
		void loop() {
			int sensorValue = analogRead(A0);
			ThingSpeak.writeField(myChannelNumber, 1, sensorValue, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int writeField(unsigned long channelNumber, unsigned int field, int value, const char * writeAPIKey)
	{
		char valueString[10];  // int range is -32768 to 32768, so 7 bytes including terminator, plus a little extra
		itoa(value, valueString, 10);
		return writeField(channelNumber, field, valueString, writeAPIKey);
	};

	/**
	 * @brief Write a long value to a single field in a ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to write to.
	 * @param value Long value (from -2,147,483,648 to 2,147,483,647) to write.
	 * @param writeAPIKey Write API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Visit https://thingspeak.com/docs/channels for more information about channels, API keys, and fields.  ThingSpeak limits the number of writes to a channel to once every 15 seconds.
	 * @code
		void loop() {
			int sensorValue = analogRead(A0);
			ThingSpeak.writeField(myChannelNumber, 1, sensorValue, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int writeField(unsigned long channelNumber, unsigned int field, long value, const char * writeAPIKey)
	{
		char valueString[15];  // long range is -2147483648 to 2147483647, so 12 bytes including terminator
   		ltoa(value, valueString, 10);
		return writeField(channelNumber, field, valueString, writeAPIKey);
	};

	/**
	 * @brief Write a floating point value to a single field in a ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to write to.
	 * @param value Floating point value (from -999999000000 to 999999000000) to write.  If you need more accuracy, or a wider range, you should format the number using <tt>dtostrf</tt> and writeField().
	 * @param writeAPIKey Write API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Visit https://thingspeak.com/docs/channels for more information about channels, API keys, and fields.  ThingSpeak limits the number of writes to a channel to once every 15 seconds.
	 * @code
		void loop() {
			int sensorValue = analogRead(A0);
			float voltage = sensorValue * (5.0 / 1023.0);
			ThingSpeak.writeField(myChannelNumber, 1, voltage, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int writeField(unsigned long channelNumber, unsigned int field, float value, const char * writeAPIKey)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::writeField (channelNumber: "); Serial.print(channelNumber); Serial.print(" writeAPIKey: "); Serial.print(writeAPIKey); Serial.print(" field: "); Serial.print(field); Serial.print(" value: "); Serial.print(value,5); Serial.println(")");
		#endif
		char valueString[20]; // range is -999999000000.00000 to 999999000000.00000, so 19 + 1 for the terminator
		int status = convertFloatToChar(value, valueString);
		if(status != OK_SUCCESS) return status;

		return writeField(channelNumber, field, valueString, writeAPIKey);
	};

	/**
	 * @brief Write a string to a single field in a ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to write to.
	 * @param value String to write (UTF8 string).  ThingSpeak limits this field to 255 bytes.
	 * @param writeAPIKey Write API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Visit https://thingspeak.com/docs/channels for more information about channels, API keys, and fields.  ThingSpeak limits the number of writes to a channel to once every 15 seconds.
	 * @code
		void loop() {
			int sensorValue = analogRead(A0);
			if (sensorValue > 512) {
				ThingSpeak.writeField(myChannelNumber, 1, "High", myWriteAPIKey);
			}
			else {
				ThingSpeak.writeField(myChannelNumber, 1, "Low", myWriteAPIKey);
			}
			delay(20000);
		}
	 * @endcode
	 */
	int writeField(unsigned long channelNumber, unsigned int field, const char * value, const char * writeAPIKey)
	{
		return writeField(channelNumber, field, String(value), writeAPIKey);
	};

	/**
	 * @brief Write a String to a single field in a ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to write to.
	 * @param value Character array (zero terminated) to write (UTF8).  ThingSpeak limits this field to 255 bytes.
	 * @param writeAPIKey Write API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Visit https://thingspeak.com/docs/channels for more information about channels, API keys, and fields.  ThingSpeak limits the number of writes to a channel to once every 15 seconds.
	 * @code
		void loop() {
			int sensorValue = analogRead(A0);
			String meaning;
			if (sensorValue < 400) {
				meaning = String("Too Cold!");
			} else if (sensorValue > 600) {
				meaning = String("Too Hot!");
			} else {
				meaning = String("Just Right");
			}
			ThingSpeak.writeField(myChannelNumber, 1, meaning, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int writeField(unsigned long channelNumber, unsigned int field, String value, const char * writeAPIKey)
	{
		// Invalid field number specified
		if(field < FIELDNUM_MIN || field > FIELDNUM_MAX) return ERR_INVALID_FIELD_NUM;
		// Max # bytes for ThingSpeak field is 255
		if(value.length() > FIELDLENGTH_MAX) return ERR_OUT_OF_RANGE;
		
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::writeField (channelNumber: "); Serial.print(channelNumber); Serial.print(" writeAPIKey: "); Serial.print(writeAPIKey); Serial.print(" field: "); Serial.print(field); Serial.print(" value: \""); Serial.print(value); Serial.println("\")");
		#endif
		String postMessage = String("field") + String(field) + "=" + value;
		return writeRaw(channelNumber, postMessage, writeAPIKey);
 	};

    
	/**
	 * @brief Set the value of a single field that will be part of a multi-field update.
	 * To write multiple fields at once, call setField() for each of the fields you want to write, and then call writeFields()
	 * @param field Field number (1-8) within the channel to set
	 * @param value Integer value (from -32,768 to 32,767) to set.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setLatitude(), setLongitude(), setElevation(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int setField(unsigned int field, int value)
	{

		char valueString[10];  // int range is -32768 to 32768, so 7 bytes including terminator
		itoa(value, valueString, 10);
		
		return setField(field, valueString);

	};

	/**
	 * @brief Set the value of a single field that will be part of a multi-field update.
	 * To write multiple fields at once, call setField() for each of the fields you want to write, and then call writeFields()
	 * @param field Field number (1-8) within the channel to set
	 * @param value Long value (from -2,147,483,648 to 2,147,483,647) to write.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setLatitude(), setLongitude(), setElevation(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int setField(unsigned int field, long value)
	{
		char valueString[15];  // long range is -2147483648 to 2147483647, so 12 bytes including terminator
	    ltoa(value, valueString, 10);
		return setField(field, valueString);
	};


	/**
	 * @brief Set the value of a single field that will be part of a multi-field update.
	 * To write multiple fields at once, call setField() for each of the fields you want to write, and then call writeFields()
	 * @param field Field number (1-8) within the channel to set
	 * @param value Floating point value (from -999999000000 to 999999000000) to write.  If you need more accuracy, or a wider range, you should format the number yourself (using <tt>dtostrf</tt>) and setField() using the resulting string.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setLatitude(), setLongitude(), setElevation(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
    int setField(unsigned int field, float value)
	{
		char valueString[20]; // range is -999999000000.00000 to 999999000000.00000, so 19 + 1 for the terminator
		int status = convertFloatToChar(value, valueString);
		if(status != OK_SUCCESS) return status;

		return setField(field, valueString);
	};

	/**
	 * @brief Set the value of a single field that will be part of a multi-field update.
	 * To write multiple fields at once, call setField() for each of the fields you want to write, and then call writeFields()
	 * @param field Field number (1-8) within the channel to set
	 * @param value String to write (UTF8).  ThingSpeak limits this to 255 bytes.  
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setLatitude(), setLongitude(), setElevation(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
    int setField(unsigned int field, const char * value)
	{
		return setField(field, String(value));
	};

	/**
	 * @brief Set the value of a single field that will be part of a multi-field update.
	 * To write multiple fields at once, call setField() for each of the fields you want to write, and then call writeFields()
	 * @param field Field number (1-8) within the channel to set
	 * @param value String to write (UTF8).  ThingSpeak limits this to 255 bytes.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setLatitude(), setLongitude(), setElevation(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
    int setField(unsigned int field, String value)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setField   (field: "); Serial.print(field); Serial.print(" value: \""); Serial.print(value); Serial.println("\")");
		#endif
		if(field < FIELDNUM_MIN || field > FIELDNUM_MAX) return ERR_INVALID_FIELD_NUM;
		// Max # bytes for ThingSpeak field is 255 (UTF-8)
		if(value.length() > FIELDLENGTH_MAX) return ERR_OUT_OF_RANGE;
		this->nextWriteField[field - 1] = value;
		return OK_SUCCESS;
	};


	/**
	 * @brief Set the latitude of a multi-field update.
	 * To record latitude, longitude and elevation of a write, call setField() for each of the fields you want to write, setLatitude() / setLongitude() / setElevation(), and then call writeFields()
	 * @param latitude Latitude of the measurement (degrees N, use negative values for degrees S)
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setField(), setLongitude(), setElevation(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			setLatitude(42.2833);
			setLongitude(-71.3500);
			setElevation(100);
			delay(20000);
		}
	 * @endcode
	 */
	int setLatitude(float latitude)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setLatitude(latitude: "); Serial.print(latitude,3); Serial.println("\")");
		#endif
		this->nextWriteLatitude = latitude;
		return OK_SUCCESS;
	};


	/**
	 * @brief Set the longitude of a multi-field update.
	 * To record latitude, longitude and elevation of a write, call setField() for each of the fields you want to write, setLatitude() / setLongitude() / setElevation(), and then call writeFields()
	 * @param longitude Longitude of the measurement (degrees E, use negative values for degrees W)
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setField(), setLatitude(), setElevation(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			setLatitude(42.2833);
			setLongitude(-71.3500);
			setElevation(100);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int setLongitude(float longitude)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setLongitude(longitude: "); Serial.print(longitude,3); Serial.println("\")");
		#endif
		this->nextWriteLongitude = longitude;
		return OK_SUCCESS;
	};


	/**
	 * @brief Set the elevation of a multi-field update.
	 * To record latitude, longitude and elevation of a write, call setField() for each of the fields you want to write, setLatitude() / setLongitude() / setElevation(), and then call writeFields()
	 * @param elevation Elevation of the measurement (meters above sea level)
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setField(), setLatitude(), setLongitude(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			setLatitude(42.2833);
			setLongitude(-71.3500);
			setElevation(100);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int setElevation(float elevation)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setElevation(elevation: "); Serial.print(elevation,3); Serial.println("\")");
		#endif
		this->nextWriteElevation = elevation;
		return OK_SUCCESS;
	};

	/**
	 * @brief Set the status of a multi-field update.
	 * To record a status message on a write, call setStatus() then call writeFields(). Use status to provide additonal 
	 * details when writing a channel update.  Additonally, status can be used by the ThingTweet App to send a message to
	 * Twitter.
	 * @param status String to write (UTF8).  ThingSpeak limits this to 255 bytes.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, timeRead);
			ThingSpeak.setStatus(sensor3Meaning);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	
    int setStatus(const char * status)
	{
		return setStatus(String(status));
	};

	/**
	 * @brief Set the status of a multi-field update.
	 * To record a status message on a write, call setStatus() then call writeFields(). Use status to provide additonal 
	 * details when writing a channel update.  Additonally, status can be used by the ThingTweet App to send a message to
	 * Twitter.
	 * @param status String to write (UTF8).  ThingSpeak limits this to 255 bytes.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, timeRead);
			ThingSpeak.setStatus(sensor3Meaning);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
    int setStatus(String status)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setStatus(status: "); Serial.print(status); Serial.println("\")");
		#endif
		// Max # bytes for ThingSpeak field is 255 (UTF-8)
		if(status.length() > FIELDLENGTH_MAX) return ERR_OUT_OF_RANGE;
		this->nextWriteStatus = status;
		return OK_SUCCESS;
	};
	
	/**
	 * @brief Set the Twitter account and message to use for an update to be tweeted.
	 * To send a message to twitter call setTwitterTweet() then call writeFields()
	 * @param twitter Twitter account name as a String.
	 * @param tweet Twitter message as a String (UTF-8) limited to 140 character.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Prior to using this feature, a twitter account must be linked to your ThingSpeak account. Do this by logging into ThingSpeak and going to Apps, then ThingTweet and clicking Link Twitter Account.
	 * @see writeFields(),getLastReadStatus()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, timeRead);
			ThingSpeak.setTwitterTweet("YourTwitterAccountName",sensor3Meaning);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);	
		}
	 * @endcode
	 */	
	int setTwitterTweet(const char * twitter, const char * tweet)
	{
		return setTwitterTweet(String(twitter), String(tweet));
	};

	/**
	 * @brief Set the Twitter account and message to use for an update to be tweeted.
	 * To send a message to twitter call setTwitterTweet() then call writeFields()
	 * @param twitter Twitter account name as a String.
	 * @param tweet Twitter message as a String (UTF-8) limited to 140 character.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Prior to using this feature, a twitter account must be linked to your ThingSpeak account. Do this by logging into ThingSpeak and going to Apps, then ThingTweet and clicking Link Twitter Account.
	 * @see writeFields(),getLastReadStatus()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, timeRead);
			ThingSpeak.setTwitterTweet("YourTwitterAccountName",sensor3Meaning);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);	
		}
	 * @endcode
	 */	
	int setTwitterTweet(String twitter, const char * tweet)
	{
		return setTwitterTweet(twitter, String(tweet));
	};

	/**
	 * @brief Set the Twitter account and message to use for an update to be tweeted.
	 * To send a message to twitter call setTwitterTweet() then call writeFields()
	 * @param twitter Twitter account name as a String.
	 * @param tweet Twitter message as a String (UTF-8) limited to 140 character.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Prior to using this feature, a twitter account must be linked to your ThingSpeak account. Do this by logging into ThingSpeak and going to Apps, then ThingTweet and clicking Link Twitter Account.
	 * @see writeFields(),getLastReadStatus()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, timeRead);
			ThingSpeak.setTwitterTweet("YourTwitterAccountName",sensor3Meaning);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);	
		}
	 * @endcode
	 */	
	int setTwitterTweet(const char * twitter, String tweet)
	{
		return setTwitterTweet(String(twitter), tweet);
	};

	/**
	 * @brief Set the Twitter account and message to use for an update to be tweeted.
	 * To send a message to twitter call setTwitterTweet() then call writeFields()
	 * @param twitter Twitter account name as a String.
	 * @param tweet Twitter message as a String (UTF-8) limited to 140 character.
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Prior to using this feature, a twitter account must be linked to your ThingSpeak account. Do this by logging into ThingSpeak and going to Apps, then ThingTweet and clicking Link Twitter Account.
	 * @see writeFields(),getLastReadStatus()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, timeRead);
			ThingSpeak.setTwitterTweet("YourTwitterAccountName",sensor3Meaning);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);	
		}
	 * @endcode
	 */	
	int setTwitterTweet(String twitter, String tweet){
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setTwitterTweet(twitter: "); Serial.print(twitter); Serial.print(", tweet: "); Serial.print(tweet); Serial.println("\")");
		#endif
		// Max # bytes for ThingSpeak field is 255 (UTF-8)
		if((twitter.length() > FIELDLENGTH_MAX) || (tweet.length() > FIELDLENGTH_MAX)) return ERR_OUT_OF_RANGE;
		
		this->nextWriteTwitter = twitter;
		this->nextWriteTweet = tweet;
		
		return OK_SUCCESS;	
	};
	
	/**
	 * @brief Set the created-at date of a multi-field update.
	 * To record created-at of a write, call setField() for each of the fields you want to write, setCreatedAt(), and then call writeFields()
	 * @param createdAt Desired timestamp to be included with the channel update as a String.  The timestamp string must be in the ISO 8601 format. Example "2017-01-12 13:22:54"
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Timezones can be set using the timezone hour offset parameter. For example, a timestamp for Eastern Standard Time is: "2017-01-12 13:22:54-05".  If no timezone hour offset parameter is used, UTC time is assumed.
	 * @see setField(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.setCreatedAt("2017-01-06T13:56:28");
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int setCreatedAt(const char * createdAt)
	{
		return setCreatedAt(String(createdAt));
	}
	
/**
	 * @brief Set the created-at date of a multi-field update.
	 * To record created-at of a write, call setField() for each of the fields you want to write, setCreatedAt(), and then call writeFields()
	 * @param createdAt Desired timestamp to be included with the channel update as a String.  The timestamp string must be in the ISO 8601 format. Example "2017-01-12 13:22:54"
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark Timezones can be set using the timezone hour offset parameter. For example, a timestamp for Eastern Standard Time is: "2017-01-12 13:22:54-05".  If no timezone hour offset parameter is used, UTC time is assumed.
	 * @see setField(), writeFields()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.setCreatedAt("2017-01-06T13:56:28");
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */	
	int setCreatedAt(String createdAt)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setCreatedAt(createdAt: "); Serial.print(createdAt); Serial.println("\")");
		#endif
		
		// the ISO 8601 format is too complicated to check for valid timestamps here
		// we'll need to reply on the api to tell us if there is a problem
		// Max # bytes for ThingSpeak field is 255 (UTF-8)
		if(createdAt.length() > FIELDLENGTH_MAX) return ERR_OUT_OF_RANGE;
		this->nextWriteCreatedAt = createdAt;
		
		return OK_SUCCESS;
	}
	

	/**
	 * @brief Write a multi-field update.
	 * Call setField() for each of the fields you want to write, setLatitude() / setLongitude() / setElevation(), and then call writeFields()
	 * @param channelNumber Channel number
	 * @param writeAPIKey Write API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @see setField(), setLatitude(), setLongitude(), setElevation()
	 * @code
		void loop() {
			int sensor1Value = analogRead(A0);
			float sensor2Voltage = analogRead(A1) * (5.0 / 1023.0);
			String sensor3Meaning;
			int sensor3Value = analogRead(A2);
			if (sensor3Value < 400) {
				sensor3Meaning = String("Too Cold!");
			} else if (sensor3Value > 600) {
				sensor3Meaning = String("Too Hot!");
			} else {
				sensor3Meaning = String("Just Right");
			}
			long timeRead = millis();

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			setLatitude(42.2833);
			setLongitude(-71.3500);
			setElevation(100);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int writeFields(unsigned long channelNumber, const char * writeAPIKey)
	{
		String postMessage = String("");
		bool fFirstItem = true;
		for(size_t iField = 0; iField < 8; iField++)
		{
			if(this->nextWriteField[iField].length() > 0)
			{
				if(!fFirstItem)
				{
					postMessage = postMessage + String("&");
				}
				postMessage = postMessage + String("field") + String(iField + 1) + String("=") + this->nextWriteField[iField];
				fFirstItem = false;
				this->nextWriteField[iField] = "";
			}
		}

		if(!isnan(nextWriteLatitude))
		{
			if(!fFirstItem)
			{
				postMessage = postMessage + String("&");
			}
			postMessage = postMessage + String("lat=") + String(this->nextWriteLatitude);
			fFirstItem = false;
			this->nextWriteLatitude = NAN;
		}

		if(!isnan(this->nextWriteLongitude))
		{
			if(!fFirstItem)
			{
				postMessage = postMessage + String("&");
			}
			postMessage = postMessage + String("long=") + String(this->nextWriteLongitude);
			fFirstItem = false;
			this->nextWriteLongitude = NAN;
		}


		if(!isnan(this->nextWriteElevation))
		{
			if(!fFirstItem)
			{
				postMessage = postMessage + String("&");
			}
			postMessage = postMessage + String("elevation=") + String(this->nextWriteElevation);
			fFirstItem = false;
			this->nextWriteElevation = NAN;
		}
		
		if(this->nextWriteStatus.length() > 0)
		{
			if(!fFirstItem)
			{
				postMessage = postMessage + String("&");
			}
			postMessage = postMessage + String("status=") + String(this->nextWriteStatus);
			fFirstItem = false;
			this->nextWriteStatus = "";
		}
		
		if(this->nextWriteTwitter.length() > 0)
		{
			if(!fFirstItem)
			{
				postMessage = postMessage + String("&");
			}
			postMessage = postMessage + String("twitter=") + String(this->nextWriteTwitter);
			fFirstItem = false;
			this->nextWriteTwitter = "";
		}
		
		if(this->nextWriteTweet.length() > 0)
		{
			if(!fFirstItem)
			{
				postMessage = postMessage + String("&");
			}
			postMessage = postMessage + String("tweet=") + String(this->nextWriteTweet);
			fFirstItem = false;
			this->nextWriteTweet = "";
		}
		
		if(this->nextWriteCreatedAt.length() > 0)
		{
			if(!fFirstItem)
			{
				postMessage = postMessage + String("&");
			}
			postMessage = postMessage + String("created_at=") + String(this->nextWriteCreatedAt);
			fFirstItem = false;
			this->nextWriteCreatedAt = "";
		}
		
		
		if(fFirstItem)
		{
			// setField was not called before writeFields
			return ERR_SETFIELD_NOT_CALLED;
		}

		return writeRaw(channelNumber, postMessage, writeAPIKey);
	};


	/**
	 * @brief Write a raw POST to a ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param postMessage Raw URL to write to ThingSpeak as a string.  See the documentation at https://thingspeak.com/docs/channels#update_feed.
	 * @param writeAPIKey Write API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark This is low level functionality that will not be required by most users.
	 * @code
		void loop() {
			const char postMessage[] = "field1=23&created_at=2014-12-31%2023:59:59";

			ThingSpeak.setField(1, sensor1Value);
			ThingSpeak.setField(2, sensor2Voltage);
			ThingSpeak.setField(3, sensor3Meaning);
			ThingSpeak.setField(4, timeRead);
			ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int writeRaw(unsigned long channelNumber, const char * postMessage, const char * writeAPIKey)
	{
		return writeRaw(channelNumber, String(postMessage), writeAPIKey);
	};


	/**
	 * @brief Write a raw POST to a ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param postMessage Raw URL to write to ThingSpeak as a String.  See the documentation at https://thingspeak.com/docs/channels#update_feed.
	 * @param writeAPIKey Write API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return HTTP status code of 200 if successful.  See getLastReadStatus() for other possible return values.
	 * @remark This is low level functionality that will not be required by most users.
	 * @code
		void loop() {
			String postMessage = String("field1=23&created_at=2014-12-31%2023:59:59");
			ThingSpeak.writeRaw(myChannelNumber, postMessage, myWriteAPIKey);
			delay(20000);
		}
	 * @endcode
	 */
	int writeRaw(unsigned long channelNumber, String postMessage, const char * writeAPIKey)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::writeRaw   (channelNumber: "); Serial.print(channelNumber); Serial.print(" writeAPIKey: "); Serial.print(writeAPIKey); Serial.print(" postMessage: \""); Serial.print(postMessage); Serial.println("\")");
		#endif

		if(!connectThingSpeak())
		{
			// Failed to connect to ThingSpeak
			return ERR_CONNECT_FAILED;
		}

		postMessage = postMessage + String("&headers=false");

		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("               POST \"");Serial.print(postMessage);Serial.println("\"");
		#endif

		postMessage = postMessage + String("\n");

		// Post data to thingspeak
		if(!this->client->print("POST /update HTTP/1.1\r\n")) return abortWriteRaw();
		if(!writeHTTPHeader(writeAPIKey)) return abortWriteRaw();
		if(!this->client->print("Content-Type: application/x-www-form-urlencoded\r\n")) return abortWriteRaw();
		if(!this->client->print("Content-Length: ")) return abortWriteRaw();
		if(!this->client->print(postMessage.length())) return abortWriteRaw();
		if(!this->client->print("\r\n\r\n")) return abortWriteRaw();
		if(!this->client->print(postMessage)) return abortWriteRaw();
  
		String entryIDText = String();
		int status = getHTTPResponse(entryIDText);
		if(status != OK_SUCCESS)
		{
			client->stop();
			return status;
		}
		long entryID = entryIDText.toInt();

		#ifdef PRINT_DEBUG_MESSAGES
		Serial.print("               Entry ID \"");Serial.print(entryIDText);Serial.print("\" (");Serial.print(entryID);Serial.println(")");
		#endif

		client->stop();
		
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.println("disconnected.");
		#endif
		if(entryID == 0)
		{
			// ThingSpeak did not accept the write
			status = ERR_NOT_INSERTED;
		}
		return status;
	};
	
	/**
	 * @brief Read the latest string from a private ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to read from.
	 * @param readAPIKey Read API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return Value read (UTF8 string), or empty string if there is an error.  Use getLastReadStatus() to get more specific information.
	 * @code
		void loop() {
		  String message = ThingSpeak.readStringField(myChannelNumber, 1, myReadAPIKey);
		  Serial.print("Latest message is: "); 
		  Serial.println(message);
		  delay(30000);
		}
	 * @endcode
	 */
    String readStringField(unsigned long channelNumber, unsigned int field, const char * readAPIKey)
	{
		if(field < FIELDNUM_MIN || field > FIELDNUM_MAX)
		{
			this->lastReadStatus = ERR_INVALID_FIELD_NUM;
			return("");
		}
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::readStringField(channelNumber: "); Serial.print(channelNumber); 
			if(NULL != readAPIKey)
			{
				Serial.print(" readAPIKey: "); Serial.print(readAPIKey);
			}
			Serial.print(" field: "); Serial.print(field); Serial.println(")");
		#endif
		return readRaw(channelNumber, String(String("/fields/") + String(field) + String("/last")), readAPIKey);
	}


	/**
	 * @brief Read the latest string from a public ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to read from.
	 * @return Value read (UTF8), or empty string if there is an error.  Use getLastReadStatus() to get more specific information.
	 * @code
		void loop() {
		  String message = ThingSpeak.readStringField(myChannelNumber, 1);
		  Serial.print("Latest message is: "); 
		  Serial.println(message);
		  delay(30000);
		}
	 * @endcode
	 */
	String readStringField(unsigned long channelNumber, unsigned int field)
	{
		return readStringField(channelNumber, field, NULL);
	};


	/**
	 * @brief Read the latest float from a private ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to read from.
	 * @param readAPIKey Read API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return Value read, or 0 if the field is text or there is an error.  Use getLastReadStatus() to get more specific information.  Note that NAN, INFINITY, and -INFINITY are valid results.
	 * @code
		void loop() {
		  float voltage = ThingSpeak.readFloatField(myChannelNumber, 1, myReadAPIKey);
		  Serial.print("Latest voltage is: "); 
		  Serial.print(voltage);
		  Serial.println("V"); 
		  delay(30000);
		}
	 * @endcode
	 */
    float readFloatField(unsigned long channelNumber, unsigned int field, const char * readAPIKey)
	{
		return convertStringToFloat(readStringField(channelNumber, field, readAPIKey));
	};


	/**
	 * @brief Read the latest float from a public ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to read from.
	 * @return Value read, or 0 if the field is text or there is an error.  Use getLastReadStatus() to get more specific information.  Note that NAN, INFINITY, and -INFINITY are valid results.
	 * @code
		void loop() {
		  float voltage = ThingSpeak.readFloatField(myChannelNumber, 1);
		  Serial.print("Latest voltage is: "); 
		  Serial.print(voltage);
		  Serial.println("V"); 
		  delay(30000);
		}
	 * @endcode
	 */
    float readFloatField(unsigned long channelNumber, unsigned int field)
	{
		return readFloatField(channelNumber, field, NULL);
	};


	/**
	 * @brief Read the latest long from a private ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to read from.
	 * @param readAPIKey Read API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return Value read, or 0 if the field is text or there is an error.  Use getLastReadStatus() to get more specific information.
	 * @code
		void loop() {
		  long value = ThingSpeak.readLongField(myChannelNumber, 1, myReadAPIKey);
		  Serial.print("Latest value is: "); 
		  Serial.print(value);
		  delay(30000);
		}
	 * @endcode
	 */
    long readLongField(unsigned long channelNumber, unsigned int field, const char * readAPIKey)
	{
        // Note that although the function is called "toInt" it really returns a long.
		return readStringField(channelNumber, field, readAPIKey).toInt();
	}


	/**
	 * @brief Read the latest long from a public ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to read from.
	 * @return Value read, or 0 if the field is text or there is an error.  Use getLastReadStatus() to get more specific information.
	 * @code
		void loop() {
		  long value = ThingSpeak.readLongField(myChannelNumber, 1);
		  Serial.print("Latest value is: "); 
		  Serial.print(value);
		  delay(30000);
		}
	 * @endcode
	 */
	long readLongField(unsigned long channelNumber, unsigned int field)
	{
		return readLongField(channelNumber, field, NULL);
	};


	/**
	 * @brief Read the latest int from a private ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to read from.
	 * @param readAPIKey Read API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return Value read, or 0 if the field is text or there is an error.  Use getLastReadStatus() to get more specific information.
	 * @remark If the value returned is out of range for an int, the result is undefined.
	 * @code
		void loop() {
		  int value = ThingSpeak.readIntField(myChannelNumber, 1, myReadAPIKey);
		  Serial.print("Latest value is: "); 
		  Serial.print(value);
		  delay(30000);
		}
	 * @endcode
	 */
    int readIntField(unsigned long channelNumber, unsigned int field, const char * readAPIKey)
	{
		return readLongField(channelNumber, field, readAPIKey);
	}


	/**
	 * @brief Read the latest int from a public ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param field Field number (1-8) within the channel to read from.
	 * @return Value read, or 0 if the field is text or there is an error.  Use getLastReadStatus() to get more specific information.
	 * @remark If the value returned is out of range for an int, the result is undefined.
	 * @code
		void loop() {
		  int value = ThingSpeak.readIntField(myChannelNumber, 1);
		  Serial.print("Latest value is: "); 
		  Serial.print(value);
		  delay(30000);
		}
	 * @endcode
	 */
    int readIntField(unsigned long channelNumber, unsigned int field)
	{
		return readLongField(channelNumber, field, NULL);
	};

	/**
	 * @brief Read the latest status from a private ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param readAPIKey Read API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return Value read (UTF8 string). An empty string is returned if there was no status written to the channel or in case of an error.  Use getLastReadStatus() to get more specific information.
	 * @code
		void loop() {
		  String value = ThingSpeak.readStatus(myChannelNumber, myReadAPIKey);
		  Serial.print("Latest status is: "); 
		  Serial.print(value);
		  delay(30000);
		}
	 * @endcode
	 */	
	String readStatus(unsigned long channelNumber, const char * readAPIKey)
	{
		String content = readRaw(channelNumber, "/feeds/last.txt?status=true", readAPIKey);
		
		if(getLastReadStatus() != OK_SUCCESS){
			return String("");
		}
		
		return getJSONValueByKey(content, "status");
	};
	
		/**
	 * @brief Read the latest status from a public ThingSpeak channel
	 * @param channelNumber Channel number	
	 * @return Value read (UTF8 string). An empty string is returned if there was no status written to the channel or in case of an error.  Use getLastReadStatus() to get more specific information.
	 * @code
		void loop() {
		  String value = ThingSpeak.readStatus(myChannelNumber, myReadAPIKey);
		  Serial.print("Latest status is: "); 
		  Serial.print(value);
		  delay(30000);
		}
	 * @endcode
	 */
	String readStatus(unsigned long channelNumber)
	{
		return readStatus(channelNumber, NULL);
	};
	
	/**
	 * @brief Read the created-at timestamp associated with the latest update to a private ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param readAPIKey Read API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return Value read (UTF8 string). An empty string is returned if there was no created-at timestamp written to the channel or in case of an error.  Use getLastReadStatus() to get more specific information.
	 * @code
		void loop() {
		  String value = ThingSpeak.readCreatedAt(myChannelNumber);
		  Serial.print("Latest update timestamp is: "); 
		  Serial.print(value);
		  delay(30000);
		}
	 * @endcode
	 */	
	String readCreatedAt(unsigned long channelNumber, const char * readAPIKey)
	{
		String content = readRaw(channelNumber, "/feeds/last.txt", readAPIKey);
		
		if(getLastReadStatus() != OK_SUCCESS){
			return String("");
		}
		
		return getJSONValueByKey(content, "created_at");
	};

	/**
	 * @brief Read the created-at timestamp associated with the latest update to a private ThingSpeak channel
	 * @param channelNumber Channel number
	 * @return Value read (UTF8 string). An empty string is returned if there was no created-at timestamp written to the channel or in case of an error.  Use getLastReadStatus() to get more specific information.
	 * @code
		void loop() {
		  String value = ThingSpeak.readCreatedAt(myChannelNumber);
		  Serial.print("Latest update timestamp is: "); 
		  Serial.print(value);
		  delay(30000);
		}
	 * @endcode
	 */	
	String readCreatedAt(unsigned long channelNumber)
	{
		return readCreatedAt(channelNumber, NULL);
	};
	
	/**
	 * @brief Read a raw response from a public ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param URLSuffix Raw URL to write to ThingSpeak as a String.  See the documentation at https://thingspeak.com/docs/channels#get_feed
	 * @return Response if successful, or empty string.	 Use getLastReadStatus() to get more specific information.
	 * @remark This is low level functionality that will not be required by most users.
	 * @code
		void loop() {
		  String response = ThingSpeak.readRaw(myChannelNumber, String("feeds/days=1"));
		  Serial.print("Response: "); 
		  Serial.print(response);
		  delay(30000);
		}
	 * @endcode
	 */
	String readRaw(unsigned long channelNumber, String URLSuffix)
	{
		return readRaw(channelNumber, URLSuffix, NULL);
	}
	
	/**
	 * @brief Read a raw response from a private ThingSpeak channel
	 * @param channelNumber Channel number
	 * @param URLSuffix Raw URL to write to ThingSpeak as a String.  See the documentation at https://thingspeak.com/docs/channels#get_feed
	 * @param readAPIKey Read API key associated with the channel.  *If you share code with others, do _not_ share this key*
	 * @return Response if successful, or empty string.	 Use getLastReadStatus() to get more specific information.
	 * @remark This is low level functionality that will not be required by most users.
	 * @code
		void loop() {
		  String response = ThingSpeak.readRaw(myChannelNumber, String("feeds/days=1"), myReadAPIKey);
		  Serial.print("Response: "); 
		  Serial.print(response);
		  delay(30000);
		}
	 * @endcode
	 */
	String readRaw(unsigned long channelNumber, String URLSuffix, const char * readAPIKey)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::readRaw   (channelNumber: "); Serial.print(channelNumber);
			if(NULL != readAPIKey)
			{
				Serial.print(" readAPIKey: "); Serial.print(readAPIKey);
			}
			Serial.print(" URLSuffix: \""); Serial.print(URLSuffix); Serial.println("\")");
		#endif

		if(!connectThingSpeak())
		{
			this->lastReadStatus = ERR_CONNECT_FAILED;
			return String("");
		}

		String URL = String("/channels/") + String(channelNumber) + URLSuffix;

		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("               GET \"");Serial.print(URL);Serial.println("\"");
		#endif

		// Post data to thingspeak
		if(!this->client->print("GET ")) return abortReadRaw();
		if(!this->client->print(URL)) return abortReadRaw();
		if(!this->client->print(" HTTP/1.1\r\n")) return abortReadRaw();
		if(!writeHTTPHeader(readAPIKey)) return abortReadRaw();
		if(!this->client->print("\r\n")) return abortReadRaw();
 
		String content = String();
		int status = getHTTPResponse(content);
			
		this->lastReadStatus = status;


		#ifdef PRINT_DEBUG_MESSAGES
			if(status == OK_SUCCESS)
			{
				Serial.print("Read: \""); Serial.print(content); Serial.println("\"");
			}
		#endif

		client->stop();
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.println("disconnected.");
		#endif

		if(status != OK_SUCCESS)
		{
			// return status;
			return String("");
		}

        // This is a workaround to a bug in the Spark implementation of String
    	return String("") + content;
	};
	
	/**
	 * @brief Get the status of the previous read.
	 * @return Generally, these are HTTP status codes.  Negative values indicate an error generated by the library.
	 * Possible response codes:
	 *  * 200: OK / Success
	 *  * 404: Incorrect API key (or invalid ThingSpeak server address)
	 *  * -101: Value is out of range or string is too long (> 255 characters)
	 *  * -201: Invalid field number specified
	 *  * -210: setField() was not called before writeFields()
	 *  * -301: Failed to connect to ThingSpeak
	 *  * -302: Unexpected failure during write to ThingSpeak
	 *  * -303: Unable to parse response
     *  * -304: Timeout waiting for server to respond
	 *  * -401: Point was not inserted (most probable cause is the rate limit of once every 15 seconds)
	 * @remark The read functions will return zero or empty if there is an error.  Use this function to retrieve the details.
	 * @code
		void loop() {
		  String message = ThingSpeak.readStringField(myChannelNumber, 1);
		  int resultCode = ThingSpeak.getLastReadStatus();
		  if(resultCode == 200)
		  {
			  Serial.print("Latest message is: "); 
			  Serial.println(message);
		  }
		  else
		  {
			  Serial.print("Error reading message.  Status was: "); 
			  Serial.println(resultCode);
		  }
		  delay(30000);
		}
	 * @endcode
	 */
	int getLastReadStatus()
	{
		return this->lastReadStatus;
	};
private:
	
	String getJSONValueByKey(String textToSearch, String key)
	{	
		if(textToSearch.length() == 0){
			return String("");
		} 
		
		String searchPhrase = String("\"") + key + String("\":\"");
		
		int fromPosition = textToSearch.indexOf(searchPhrase,0);
		
		if(fromPosition == -1){
			// return because there is no status or it's null
			return String("");
		}
		
		fromPosition = fromPosition + searchPhrase.length();
				
		int toPosition = textToSearch.indexOf("\"", fromPosition);
		
		
		if(toPosition == -1){
			// return because there is no end quote
			return String("");
		}
		
		textToSearch.remove(toPosition);
		
		return textToSearch.substring(fromPosition);	
	}
	
    int abortWriteRaw()
    {
        this->client->stop();
        return ERR_UNEXPECTED_FAIL;
    }

    String abortReadRaw()
    {
		this->client->stop();
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.println("ReadRaw abort - disconnected.");
		#endif
		this->lastReadStatus = ERR_UNEXPECTED_FAIL;
		return String("");
    }

	void setServer(const char * customHostName, unsigned int port)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setServer  (URL: \""); Serial.print(customHostName); Serial.println("\")");
		#endif
		this->customIP = INADDR_NONE;
		this->customHostName = customHostName;
        this->port = port;
	};

	void setServer(IPAddress customIP, unsigned int port)
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setServer  (IP: \""); Serial.print(customIP); Serial.println("\")");
		#endif
		this->customIP = customIP;
		this->customHostName = NULL;
        this->port = port;
	};

	void setServer()
	{
		#ifdef PRINT_DEBUG_MESSAGES
			Serial.print("ts::setServer  (default)");
		#endif
		this->customIP = INADDR_NONE;
		this->customHostName = NULL;
        this->port = THINGSPEAK_PORT_NUMBER;
	};

	void setClient(Client * client) {this->client = client;};

	Client * client = NULL;
	const char * customHostName = NULL;
	IPAddress customIP = INADDR_NONE;
    unsigned int port = THINGSPEAK_PORT_NUMBER;
	String nextWriteField[8];
	float nextWriteLatitude;
	float nextWriteLongitude;
	float nextWriteElevation;
	int lastReadStatus;
	String nextWriteStatus;
	String nextWriteTwitter;
	String nextWriteTweet;
	String nextWriteCreatedAt;

	bool connectThingSpeak()
	{
		bool connectSuccess = false;
		if(this->customIP == INADDR_NONE && NULL == this->customHostName)
        {
 			#ifdef PRINT_DEBUG_MESSAGES
  			    Serial.print("               Connect to default ThingSpeak URL...");
			#endif
			connectSuccess = client->connect(THINGSPEAK_URL,THINGSPEAK_PORT_NUMBER);
            if(!connectSuccess)
            {
 			    #ifdef PRINT_DEBUG_MESSAGES
  			        Serial.print("Failed. Try default IP...");
			    #endif
			    connectSuccess = client->connect(THINGSPEAK_IPADDRESS,THINGSPEAK_PORT_NUMBER);
            }
       }
       else
       {
		    if(!(this->customIP == INADDR_NONE))
		    {
			    // Connect to the server on port 80 (HTTP) at the customIP address
			    #ifdef PRINT_DEBUG_MESSAGES
  			      Serial.print("               Connect to ");Serial.print(this->customIP);Serial.print("...");
			    #endif
			    connectSuccess = client->connect(this->customIP,this->port);
		    }
		    if(NULL != this->customHostName)
		    {
			    // Connect to the server on port 80 (HTTP) at the URL address
			    #ifdef PRINT_DEBUG_MESSAGES
				    Serial.print("               Connect to ");Serial.print(this->customHostName);Serial.print(" ...");
			    #endif
			    connectSuccess = client->connect(customHostName,this->port);
		    }
        }

		#ifdef PRINT_DEBUG_MESSAGES
		if (connectSuccess)
		{
			Serial.println("Success.");
		}
		else
		{
			Serial.println("Failed.");
		}
		#endif
		return connectSuccess;
	};

	bool writeHTTPHeader(const char * APIKey)
	{
        if(NULL != this->customHostName)
        {
		    if (!this->client->print("Host: ")) return false;
		    if (!this->client->print(this->customHostName)) return false;
		    if (!this->client->print("\r\n")) return false;
        }
        else
        {
		    if (!this->client->print("Host: api.thingspeak.com\r\n")) return false;
        }
		if (!this->client->print("Connection: close\r\n")) return false;
		if (!this->client->print("User-Agent: ")) return false;
		if (!this->client->print(TS_USER_AGENT)) return false;
		if (!this->client->print("\r\n")) return false;
		if(NULL != APIKey)
		{
			if (!this->client->print("X-THINGSPEAKAPIKEY: ")) return false;
			if (!this->client->print(APIKey)) return false;
			if (!this->client->print("\r\n")) return false;
		}
		return true;
	};

	int getHTTPResponse(String & response)
	{
        long startWaitForResponseAt = millis();
        while(client->available() == 0 && millis() - startWaitForResponseAt < TIMEOUT_MS_SERVERRESPONSE)
        {
            delay(100);
        }
        if(client->available() == 0)
        {
			return ERR_TIMEOUT; // Didn't get server response in time
        }

		if(!client->find(const_cast<char *>("HTTP/1.1")))
		{
			#ifdef PRINT_HTTP
				Serial.println("ERROR: Didn't find HTTP/1.1");
    		#endif
			return ERR_BAD_RESPONSE; // Couldn't parse response (didn't find HTTP/1.1)
		}
		int status = client->parseInt();
		#ifdef PRINT_HTTP
			Serial.print("Got Status of ");Serial.println(status);
		#endif
		if(status != OK_SUCCESS)
		{
			return status;
		}

		if(!client->find(const_cast<char *>("\r\n")))
		{
			#ifdef PRINT_HTTP
    			Serial.println("ERROR: Didn't find end of status line");
			#endif
			return ERR_BAD_RESPONSE;
		}
		#ifdef PRINT_HTTP
    		Serial.println("Found end of status line");
		#endif

		if(!client->find(const_cast<char *>("\n\r\n")))
		{
			#ifdef PRINT_HTTP
				Serial.println("ERROR: Didn't find end of header");
			#endif
			return ERR_BAD_RESPONSE;
		}
		#ifdef PRINT_HTTP
			Serial.println("Found end of header");
		#endif

		String tempString = client->readString();
		response = tempString;
		#ifdef PRINT_HTTP
    		Serial.print("Response: \"");Serial.print(response);Serial.println("\"");
		#endif
		return status;
	};

	int convertFloatToChar(float value, char *valueString)
	{
		// Supported range is -999999000000 to 999999000000
		if(0 == isinf(value) && (value > 999999000000 || value < -999999000000))
		{
			// Out of range
			return ERR_OUT_OF_RANGE;
		}
		// assume that 5 places right of decimal should be sufficient for most applications

        #if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
		  sprintf(valueString, "%.5f", value);
		#else
		  dtostrf(value,1,5, valueString);
        #endif
		return OK_SUCCESS;
	};

	float convertStringToFloat(String value)
	{
		// There's a bug in the AVR function strtod that it doesn't decode -INF correctly (it maps it to INF)
		float result = value.toFloat();
		
		if(1 == isinf(result) && *value.c_str() == '-')
		{
			result = (float)-INFINITY;
		}
		return result;
	};

	void resetWriteFields()
	{
		for(size_t iField = 0; iField < 8; iField++)
		{
			this->nextWriteField[iField] = "";
		}
		this->nextWriteLatitude = NAN;
		this->nextWriteLongitude = NAN;
		this->nextWriteElevation = NAN;
		this->nextWriteStatus = "";
		this->nextWriteTwitter = "";
		this->nextWriteTweet = "";
		this->nextWriteCreatedAt = "";
	};
};

extern ThingSpeakClass ThingSpeak;

#endif //ThingSpeak_h
