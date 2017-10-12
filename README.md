# ESP32BleAdvertise
Simple library of a BLE advertise using ESP32 in Arduino.

### Usage
```
void setup() {
    ble.begin("ESP32-BLE");  //sets the device name
}

void loop() {
    String str = String(random(0, 1000));
    ble.advertise(str);   // advertises a random number
    delay(1000);
}
```

### Downloading and Installing
Click on the green button "Clone or download" and download as a zip file.
In Arduino Studio, click in `Sketch > Include Library > Add .ZIP Library` and select the file you've just downloaded.

### Debugging and visualizing data
Check out this Android app, works very well for debugging this application:
https://play.google.com/store/apps/details?id=uk.co.alt236.btlescan&hl=en
You will see the advertise data at "Manufacturer Specific Data".


### Tips
Be aware of the limitations of the BLE broadcast in terms of the message size. Your advertise data should not be bigger than 20 bytes and the name of the device should not be bigger than 20 bytes.

## TODOs
+ Being able to change the UUID
