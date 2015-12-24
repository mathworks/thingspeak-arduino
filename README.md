#ThingSpeak Communication Library for Arduino, ESP8266, and Particle

This library enables an Arduino or other compatible hardware to write or read data to or from ThingSpeak, an open data platform for the Internet of Things with MATLAB analytics and visualization.

Complete technical documentation is in the extras/documentation folder inside the library.

ThingSpeak offers free data storage and analysis of time-stamped numeric or alphanumeric data. Users can access ThingSpeak by visiting http://thingspeak.com and creating a ThingSpeak user account.

ThingSpeak stores data in channels. Channels support an unlimited number of timestamped observations (think of these as rows in a spreadsheet). Each channel has up to 8 fields (think of these as columns in a speadsheet). Check out this [video](http://www.mathworks.com/videos/introduction-to-thingspeak-107749.html) for an overview.

Channels may be public, where anyone can see the data, or private, where only the owner and select users can read the data. Each channel has an associated Write API Key that is used to control who can write to a channel. In addition, private channels have one or more Read API Keys to control who can read from private channel. An API Key is not required to read from public channels.  Each channel can have up to 8 fields. One field is created by default.

You can visualize and do online analytics of your data on ThingSpeak using the built in version of MATLAB, or use the desktop version of MATLAB to get deeper historical insight. Visit https://www.mathworks.com/hardware-support/thingspeak.html to learn more.

## Installation
#### Arduino IDE
In the Arduino IDE, choose Sketch/Include Library/Manage Libraries.  Click the ThingSpeak Library from the list, and click the Install button.

--- or ---

1. Download the ZIP file (below) to your machine.
2. In the Arduino IDE, choose Sketch/Include Library/Add Zip Library
3. Navigate to the ZIP file, and click Open

#### Spark IDE
In the Spark Web IDE, click the libraries tab, find ThingSpeak, and choose "Include in App"

## Compatible Hardware:

* Arduino or compatible using a wired or Wi-Fi ethernet shield (we have tested with Uno and Mega), should work with Arduino WiFi Shield 101
* Arduino Yun running OpenWRT-Yun Release 1.5.3 (November 13th, 2014) or later. There are known issues with earlier versions. Visit this page to get the latest version.
* ESP8266. Tested with [SparkFun ESP8266 Thing - Dev Board](https://www.sparkfun.com/products/13711) and [NodeMCU 1.0 module](http://www.seeedstudio.com/depot/NodeMCU-v2-Lua-based-ESP8266-development-kit-p-2415.html)
* Particle Core, [Photon](https://www.particle.io/prototype#photon), and [Electron](https://www.particle.io/prototype#electron) (Formally Spark)

## Examples:

The library includes several examples to help you get started. These are accessible in the Examples/ThingSpeak menu off the File menu in the Arduino IDE.

* **CheerLights:** Reads the latest CheerLights color on ThingSpeak, and sets an RGB LED.
* **ReadLastTemperature:** Reads the latest temperature from the public MathWorks weather station in Natick, MA on ThingSpeak.
* **ReadPrivateChannel:** Reads the latest voltage value from a private channel on ThingSpeak.
* **ReadWeatherStation:** Reads the latest weather data from the public MathWorks weather station in Natick, MA on ThingSpeak.
* **WriteMultipleVoltages:** Reads analog voltages from pins 0-7 and writes them to the 8 fields of a channel on ThingSpeak.
* **WriteVoltage:** Reads an analog voltage from pin 0, converts to a voltage, and writes it to a channel on ThingSpeak.
