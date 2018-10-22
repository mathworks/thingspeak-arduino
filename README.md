# ThingSpeak Communication Library for Arduino, ESP8266 and ESP32

This library enables an Arduino or other compatible hardware to write or read data to or from ThingSpeak, an open data platform for the Internet of Things with MATLAB analytics and visualization.

Complete technical documentation is in the extras/documentation folder inside the library.

ThingSpeak offers free data storage and analysis of time-stamped numeric or alphanumeric data. Users can access ThingSpeak by visiting http://thingspeak.com and creating a ThingSpeak user account.

ThingSpeak stores data in channels. Channels support an unlimited number of timestamped observations (think of these as rows in a spreadsheet). Each channel has up to 8 fields (think of these as columns in a speadsheet). Check out this [video](http://www.mathworks.com/videos/introduction-to-thingspeak-107749.html) for an overview.

Channels may be public, where anyone can see the data, or private, where only the owner and select users can read the data. Each channel has an associated Write API Key that is used to control who can write to a channel. In addition, private channels have one or more Read API Keys to control who can read from private channel. An API Key is not required to read from public channels.  Each channel can have up to 8 fields. One field is created by default.

You can visualize and do online analytics of your data on ThingSpeak using the built in version of MATLAB, or use the desktop version of MATLAB to get deeper historical insight. Visit https://www.mathworks.com/hardware-support/thingspeak.html to learn more.

Libraries and examples for Particle devices can be found here: https://github.com/mathworks/thingspeak-particle

## Installation
#### Arduino IDE
In the Arduino IDE, choose Sketch/Include Library/Manage Libraries.  Click the ThingSpeak Library from the list, and click the Install button.

--- or ---

1. Download the ZIP file (below) to your machine.
2. In the Arduino IDE, choose Sketch/Include Library/Add Zip Library
3. Navigate to the ZIP file, and click Open

#### Particle Web IDE
In the Particle Web IDE, click the libraries tab, find ThingSpeak, and choose "Include in App"

## Compatible Hardware:

* Arduino/Genuino or compatible using a WiFi Shield
* Arduino/Genuino or compatible using a WiFi Shield 101 
* Arduino/Genuino or compatible using an Ethernet Shield
* Arduino/Genuino or compatible using a MKR ETH Shield
* Arduino MKR1000 (use the WiFi101 library version 0.13.0 or older. WiFi101 library versions 0.14.0 and newer have a bug that stops this ThingSpeak library from working properly)
* Arduino MKR1010
* Arduino VIDOR 4000
* Arduino GSM 14000
* Arduino Yún (Rev1 and Rev2)
* ESP8266 (tested with [SparkFun ESP8266 Thing - Dev Board](https://www.sparkfun.com/products/13711) and [NodeMCU 1.0 module](http://www.seeedstudio.com/depot/NodeMCU-v2-Lua-based-ESP8266-development-kit-p-2415.html)) 
* ESP32 (tested with [SparkFun ESP32 Thing - Dev Board](https://www.sparkfun.com/products/13907))

## Examples:

The library includes several examples to help you get started.  These are accessible in Examples > ThingSpeak menu of the Arduino IDE.

* **ReadField:** Reading from a public channel and a private channel on ThingSpeak.
* **WriteSingleField:** Writing a value to a single field on ThingSpeak.
* **WriteMultipleFields:** Writing values to multiple fields and status in one transaction with ThingSpeak.
