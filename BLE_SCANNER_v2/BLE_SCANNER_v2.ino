#include <TFT_eSPI.h>
#include <SPI.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define bleServerName "ESP_SERVER"
static BLEUUID bmsServiceUUID("236EAFA6-4A38-66C6-31EE-93006D15197D");

TFT_eSPI tft = TFT_eSPI(170, 320); // Invoke custom library

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

String deviceData;
String Scaned_BLE_Address;
static BLEAddress *Server_BLE_Address;
String My_BLE_Address = "38:1f:8d:e1:fa:db"; //Hardware Bluetooth MAC of my fitnessband, will vary for every band obtained through nRF connect application 
                         

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
     // deviceName = advertisedDevice.toString().c_str();
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress());
      Scaned_BLE_Address = Server_BLE_Address->toString().c_str();

      deviceData = advertisedDevice.toString().c_str();
    }
};


void setup()
{
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  tft.println("BLE SCANNER v2.2 / Jan 30");
  tft.println("");
  tft.println("getScan");
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  tft.println("setActiveScan(true)");
  pBLEScan->setInterval(100);
  tft.println("setInterval(100)");
  pBLEScan->setWindow(99);  // less or equal setInterval value
  tft.println("setWindow(99)");
  tft.println("Scanning...");
  delay(3000);

}    

void loop()
{
  deviceData = "";
  
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);

  tft.setTextColor(TFT_BLUE);
  tft.print(foundDevices.getCount()); tft.println(" devices");
  tft.setCursor(0, 18);

  tft.setTextColor(TFT_WHITE);
  deviceData.replace("Name: , ", "no name \n");
  deviceData.replace("Name: ", "");
  deviceData.replace(", ", "\n");
  deviceData.replace("Address: ", "");
  deviceData.replace("manufacturer data: ","data: ");
  //tft.println(deviceData);
 
  if (Scaned_BLE_Address == My_BLE_Address) {
    tft.setTextColor(TFT_GREEN);
    tft.println("Found BMS");
  } else {
    tft.println("BMS not found");
  }

  tft.println(deviceData);



/*
  uint32_t deviceCount = foundDevices.getCount();
  for (uint32_t i = 0; i < deviceCount; i++) {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    //if (strcmp(device.getName(i), "ANT-BLE20PHUB-0887") == 0) {
   //   tft.println("We found a device ANT-BLE");
   // }
 }

  int deviceCount = foundDevices.getCount();
  for (uint32_t i = 0; i < deviceCount; i++)
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);

    if (strcmp(device.getName(), "ANT-BLE20PHUB-0887") == 0)
    {
      tft.println("We found a device ANT-BLE");
    } else tft.println("cant' found a device ANT-BLE");
  }
*/


  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory

  delay(2000);
    
}