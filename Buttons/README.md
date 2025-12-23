# ESP32-S3 Button Monitoring (reTerminal E1001)

## Overview
Monitors physical buttons (GPIO5,4,3) with debouncing, buzzer feedback (GPIO45), and USB serial logging.

Benefits: Hardware interaction, debounced input, audible/USB feedback.

## Example Code
```cpp
const int BUTTON_LEFT = 5, BUZZER_PIN = 45;
HardwareSerial& usbSerial = Serial1;

void setup() {
  usbSerial.begin(115200, SERIAL_8N1, 44, 43);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if (digitalRead(BUTTON_LEFT) == LOW) {
    usbSerial.println("Left Button Pressed");
    tone(BUZZER_PIN, 1000, 500);
  }
}
```

## Tips
- Board: XIAO_ESP32S3, USB CDC On Boot, 115200 baud.
- Buttons: Pull-up, press to GND.
- Buzzer: 1kHz tone on press.