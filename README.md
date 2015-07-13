#ThingSpeak Communication Library for Arduino

This library enables an Arduino or other compatible hardware to write or read data to or from ThingSpeak, an open data platform for the Internet of Things with MATLAB analytics and visualization.

Complete technical documentation is in the extras/documentation folder inside the library.

ThingSpeak offers free data storage and analysis of time-stamped numeric or alphanumeric data. Users can access ThingSpeak by visiting http://thingspeak.com and creating a ThingSpeak user account.

ThingSpeak stores data in channels. Channels support an unlimited number of timestamped observations (think of these as rows in a spreadsheet). Each channel has up to 8 fields (think of these as columns in a speadsheet).

Channels may be public, where anyone can see the data, or private, where only the owner and select users can read the data. Each channel has an associated Write API Key that is used to control who can write to a channel. In addition, private channels have one or more Read API Keys to control who can read from private channel. An API Key is not required to read from public channels.

You can visualize and do online analytics of your data on ThingSpeak using the built in version of MATLAB, or use the desktop version of MATLAB to get deeper historical insight. Visit http://www.mathworks.com/iot to learn more.

## Installation:
1. Download the ZIP file to your machine.
2. In the Arduino IDE, choose Sketch/Include Library/Add Zip Library
3. Navigate to the ZIP file, and click Open

See the Examples in the File/Examples menu.  The library appears in the Sketch/Include Library menu. Documentation is in the <USER>/libraries/ThingSpeak/extras/documentation directory

## Compatible Hardware:

* Arduino or compatible using a wired or Wi-Fi ethernet shield (we have tested with Uno and Mega)
* Arduino Yun running OpenWRT-Yun Release 1.5.3 (November 13th, 2014) or later. There are known issues with earlier versions. Visit this page to get the latest version.
* Particle Core or Photon (Formally Spark)

## Terms Used:
* **Public Channel:** A channel that can be viewed on ThingSpeak with data that can be read by anybody. Writing requires an API Key.
* **Private Channel:** A channel which only the owner can view and whose data can only be accessed by providing an API Key.
* **Fields:** Each channel can have up to 8 fields. One field is created by default.

## Examples:

The library includes several examples to help you get started. These are accessible in the Examples/ThingSpeak menu off the File menu in the Arduino IDE.

* **CheerLights:** Reads the latest CheerLights color on ThingSpeak, and sets an RGB LED.
* **ReadLastTemperature:** Reads the latest temperature from the public MathWorks weather station in Natick, MA on ThingSpeak.
* **ReadPrivateChannel:** Reads the latest voltage value from a private channel on ThingSpeak.
* **ReadWeatherStation:** Reads the latest weather data from the public MathWorks weather station in Natick, MA on ThingSpeak.
* **WriteMultipleVoltages:** Reads analog voltages from pins 0-7 and writes them to the 8 fields of a channel on ThingSpeak.
* **WriteVoltage:** Reads an analog voltage from pin 0, converts to a voltage, and writes it to a channel on ThingSpeak.
