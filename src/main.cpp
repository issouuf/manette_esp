// #include <WiFi.h>

// void setup(){
//   Serial.begin(115200);
  
//   // Variable to store the MAC address
//   uint8_t baseMac[6];
  
//   // Get MAC address of the WiFi station interface
//   esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
//   Serial.print("Station MAC: ");
//   for (int i = 0; i < 5; i++) {
//     Serial.printf("%02X:", baseMac[i]);
//   }
//   Serial.printf("%02X\n", baseMac[5]);
  
//   // Get the MAC address of the Wi-Fi AP interface
//   esp_read_mac(baseMac, ESP_MAC_WIFI_SOFTAP);
//   Serial.print("SoftAP MAC: ");
//   for (int i = 0; i < 5; i++) {
//     Serial.printf("%02X:", baseMac[i]);
//   }
//   Serial.printf("%02X\n", baseMac[5]);
  
//   // Get the MAC address of the Bluetooth interface
//   esp_read_mac(baseMac, ESP_MAC_BT);
//   Serial.print("Bluetooth MAC: ");
//   for (int i = 0; i < 5; i++) {
//     Serial.printf("%02X:", baseMac[i]);
//   }
//   Serial.printf("%02X\n", baseMac[5]);

//   // Get the MAC address of the Ethernet interface
//   esp_read_mac(baseMac, ESP_MAC_ETH);
//   Serial.print("Ethernet MAC: ");
//   for (int i = 0; i < 5; i++) {
//     Serial.printf("%02X:", baseMac[i]);
//   }
//   Serial.printf("%02X\n", baseMac[5]);
// }
 
// void loop(){
// }




















#include <PS4Controller.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_err.h"

unsigned long lastTimeStamp = 0;
#define EVENTS 0
#define BUTTONS 1
#define JOYSTICKS 0
#define SENSORS 0

void notify();

void onConnect() ;
  // Code à exécu
void onDisConnect(); 

void removePairedDevices();
void printDeviceAddress();
void setup() {
  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin("C8:F0:9E:2C:07:B2");
  removePairedDevices(); // This helps to solve connection issues
  Serial.print("This device MAC is: ");
  printDeviceAddress();
  Serial.println("");
}

void loop() {
  PS4.setLed(0,255,0);
}


void removePairedDevices() {
  uint8_t pairedDeviceBtAddr[20][6];
  int count = esp_bt_gap_get_bond_device_num();
  esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
  for (int i = 0; i < count; i++) {
    esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
  }
}

void printDeviceAddress() {
  const uint8_t* point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++) {
    char str[3];
    sprintf(str, "%02x", (int)point[i]);
    Serial.print(str);
    if (i < 5) {
      Serial.print(":");
    }
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void notify() {
#if EVENTS
  boolean sqd = PS4.event.button_down.square,
          squ = PS4.event.button_up.square,
          trd = PS4.event.button_down.triangle,
          tru = PS4.event.button_up.triangle;
  if (sqd)
    Serial.println("SQUARE down");
  else if (squ)
    Serial.println("SQUARE up");
  else if (trd)
    Serial.println("TRIANGLE down");
  else if (tru)
    Serial.println("TRIANGLE up");
#endif

#if BUTTONS
  boolean sq = PS4.Square(),
          tr = PS4.Triangle(),
          cr = PS4.Cross(),
          ci = PS4.Circle();
  if (sq)
    Serial.print(" SQUARE pressed");
  if (tr)
    Serial.print(" TRIANGLE pressed");
  if (sq | tr)
    Serial.println();
  if (cr)
    Serial.print(" CROSS pressed");
  if (ci)
    Serial.print(" CIRCLE pressed");

#endif

  //Only needed to print the message properly on serial monitor. Else we dont need it.
  if (millis() - lastTimeStamp > 50) {
#if JOYSTICKS
    Serial.printf("lx:%4d,ly:%4d,rx:%4d,ry:%4d\n",
                  PS4.LStickX(),
                  PS4.LStickY(),
                  PS4.RStickX(),
                  PS4.RStickY());
#endif
#if SENSORS
    Serial.printf("gx:%5d,gy:%5d,gz:%5d,ax:%5d,ay:%5d,az:%5d\n",
                  PS4.GyrX(),
                  PS4.GyrY(),
                  PS4.GyrZ(),
                  PS4.AccX(),
                  PS4.AccY(),
                  PS4.AccZ());
#endif
    lastTimeStamp = millis();
  }
}

void onDisConnect() {
  Serial.println("Disconnected!");
}