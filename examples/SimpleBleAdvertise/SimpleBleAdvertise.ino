#include "ESP32ArduinoBeacon.h"
SimpleBLE ble;

void setup() {
    Serial.begin(115200);
    ble.begin("3ab87438");  //sets the device name
}

void loop() {
    String str = String(random(0, 1000));
    ble.advertise(str);   // advertises a random number
    delay(1000);
}
