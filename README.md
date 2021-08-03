# ESP32 Bluetooth Low Energy (BLE) Server Example
This repo demonstrates how to make a BLE server out of an ESP32.  We'll be transferring a simple piece of data (a "dial value", ranging from 0 to 9) between devices.

## BLE Overview
Standard Bluetooth is typically used when streaming data is needed...for example, a wireless headset.  This connection consumes a fair amount of power.  Bluetooth Low Energy (BLE) was added to the standard in order to facilitate short, energy-efficent data transfers.  

## Technology Tradeoffs
BLE provides a point-to-point connection via a client-server model.  If you are looking for a many-to-one or many-to-many network, this strategy may not be best for your project; something like MQTT may be better.

The main "pro" of using BLE is that this connection is "paired"...there's no network setup involved like with MQTT over WiFi.

## High Level BLE Concepts
A peripheral device (like the one we're creating here) is typically a BLE server.  
A server can support multiple Services.   
Each Service has one or more Characteristics.  These are the lowest level "data objects".

For this project, I'm going to make a simple Service that contains 3 Characteristics...one for speed, one for eye color, and one for background color.

Note that the Bluetooth SIG defines "Profiles" as well...these are industry-standard collections of services.  For example, there is a "heart rate monitor" profile that describes which services should be supported in an industry standard heart rate monitor.  Check out https://www.bluetooth.com/specifications/gatt for more info.

## Implementation
Each Service and Characteristic needs a UUID.  For the industry standard ones, you reference the official specifications, but this project here "user defined".  There's a website that lets you generate UUIDs (https://www.uuidgenerator.net/)

The arduino BLE libraries let you specify Callbacks for each service and characteristic.  I've currently got a "connect and disconnect" callback for the server, and then "Write" callbacks for the characteristcs that set the appropriate cylon variables.

In setup, I'm:
* Initializing my device (which sets it's name for discovery)
* Setting up the Service (along with it's callbacks)
* Setting up the Characteristics and attaching them to the service.



