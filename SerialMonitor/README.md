# ESP32-S3 USB Serial Logging Setup (reTerminal E1001 / XIAO_ESP32S3)

## Overview
The reTerminal E1001 uses ESP32-S3 with USB-C for serial communication via USB CDC. Default `Serial` (UART0 on GPIO1/3) isn't USB-connected, causing silent failures. Use `Serial1` remapped to GPIO43 (TX) and GPIO44 (RX) for reliable USB logging in Arduino IDE Serial Monitor.

Benefits:
- Avoids bootloader-only logs.
- Enables debugging without external hardware.

## Example Code
```cpp
// Alias for USB monitoring
HardwareSerial& usbSerial = Serial1;

void setup() {
  usbSerial.begin(115200, SERIAL_8N1, 44, 43);  // RX=44, TX=43 for USB CDC
  delay(500);  // Stabilize
  usbSerial.println("USB Serial Logging Started");
}

void loop() {
  usbSerial.println("Log message");
  delay(1000);
}
```

## Tips
- Set Arduino IDE board to "XIAO_ESP32S3".
- Enable "USB CDC On Boot".
- Use 115200 baud.
- Power cycle after upload if needed.