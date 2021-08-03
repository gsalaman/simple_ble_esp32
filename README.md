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
Each Service has one or more Characteristics.  These are the lowest level "data objects".  Think of these as variables that you can read or write.  This is the 

For this project, I'm going to make a simple Service that contains only one characteristic to represent the "dial value"

Note that the Bluetooth SIG defines "Profiles" as well...these are industry-standard collections of services.  For example, there is a "heart rate monitor" profile that describes which services should be supported in an industry standard heart rate monitor.  Check out https://www.bluetooth.com/specifications/gatt for more info.

## Implementation
Each Service and Characteristic needs a UUID.  For the industry standard ones, you reference the official specifications, but this project here "user defined".  There's a website that lets you generate UUIDs (https://www.uuidgenerator.net/)

The arduino BLE libraries let you specify Callbacks for each service and characteristic.  I've currently got a "connect and disconnect" callback for the server, and then "Write" callback for the characteristc that sets the dial variable.

In setup, I'm:
* Initializing my device (which sets it's name for discovery)
* Setting up the Service (along with it's callbacks)
* Setting up the Characteristics and attaching them to the service

## A closer look at the implementation
We need to include the following files:
```
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
```

These allow us to do BLE via some C++ constructs.  Don't sweat it; you don't need to know C++ to proceed here...just follow the cookbook instructions below.  

We then have #defines for the Service UUID and Characteristic UUID:
```
#define SERVICE_UUID          "62a26d83-4fc6-4e00-b4ef-dc14244d1868"
#define DIAL_CHAR_UUID        "65b2dc13-930a-4439-b23e-ea853302d6db"
```

In additon, we've got a global variable to store the "dial value":
```
int dial_val;
```

We then need callbacks for when devices connect and disconnect from our server.  The library implements these as a C++ subclass...the syntax looks like this:
```
class MyServerCallbacks: public BLEServerCallbacks 
{
    void onConnect(BLEServer* pServer) 
    {
      Serial.println("Device Connected");
    };

    void onDisconnect(BLEServer* pServer) 
    {
      Serial.println("Device Disconnected");
    }
};
```
`MyServerCallbacks` is the user-set name...you  can call this whatever you want (or keep it the same).  The rest of the names should stay the same for the library to work.

Next, we need a callback for what to do when our characteristic is written.  This also is a C++ subclass, and looks like this:
```
class DialCB: public BLECharacteristicCallbacks 
{
    void onWrite(BLECharacteristic *pCharacteristic) 
    {
      
      std::string value = pCharacteristic->getValue();
      int temp_dial;

      temp_dial = atoi( value.c_str() );
      
      if ((temp_dial > 9) || (temp_dial < 0))
      {
        Serial.println("Bad Dial Input");
      }
      else
      {
        Serial.print("dial: ");
        Serial.println(temp_dial);

        dial_val = temp_dial;
      }
      
    }
};
```
Lots going on here...let's start with the definition.
`DialCB` is our user-defined name for the dial Callback (CB is my shorthand for "callback").  If we were doing other callbacks, we'd still have them as `BLECharacteristicCallbacks`, but have different user-defined names for them.  

Next we have a library defined function for what to do when this characteristic is written.  Keep the definition the same...don't change `void onWrite(BLECharacteristic *pCharacteristic)`

If you look at the implementation of this function, you'll see that this library uses the standard template library for strings, and we can use "getValue" to retrieve a string representation of our characteristic.  

Then we convert that string to a number via the `atoi` function.  `atoi` is "Ascii-to-integer"....it takes our input string and returns an integer.  We then check to make sure the integer  is in our expected range (0-9)  before setting our  global variable for the Dial.








