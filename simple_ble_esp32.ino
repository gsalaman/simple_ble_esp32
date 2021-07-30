/* Bare-bones control of an ESP32 over BLE.
 *  One characteristic...setting a dial from 0-9.
 */
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID          "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define DIAL_CHAR_UUID        "65b2dc13-930a-4439-b23e-ea853302d6db"

int dial_val;

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


void setup() 
{
  int i;
  uint8_t init_value[1];
  
  Serial.begin(115200);
  
  
  BLEDevice::init("ESP32 Dial Example");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pDialChar = pService->createCharacteristic(
                                         DIAL_CHAR_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                       
                                                                                                                 
  pDialChar->setCallbacks(new DialCB());
  init_value[0]=0;
  pDialChar->setValue(init_value, 1);

  pService->start();
  
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() 
{  
  Serial.print("Dial val: ");
  Serial.println(dial_val);
  delay(1000);
  
}
