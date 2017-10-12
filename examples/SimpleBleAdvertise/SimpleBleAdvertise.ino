#include "ESP32BleAdvertise.h"
SimpleBLE ble;

void setup() {
    Serial.begin(115200);
    ble.begin("ESP32-BLE");  //sets the device name
}

void loop() {
    String str = String(random(0, 1000));
    ble.advertise(str);   // advertises a random number
    delay(1000);
}
