# ESP32-S3 e-Ink Display Demo (reTerminal E1001)

## Overview
Demonstrates displaying centered text in multiple font sizes on the 7.5" e-ink display using GxEPD2 library. Includes USB serial logging for monitoring.

Benefits:
- Shows text rendering and centering on e-ink.
- Single update to avoid flashes.
- USB logging for feedback.

## Example Code
```cpp
// Includes and setup for GxEPD2, SPI, usbSerial
// In setup: init display, draw centered text in small/medium/large fonts
display.setFullWindow();
display.firstPage();
do {
  display.fillScreen(GxEPD_WHITE);
  display.setFont(&FreeMonoBold9pt7b);
  printCentered("Small: Hello World!", 140);
  // Similar for medium and large
} while (display.nextPage());
```

## Tips
- Set Arduino IDE board to "XIAO_ESP32S3".
- Enable "USB CDC On Boot".
- Use 115200 baud.
- Power cycle after upload.
- Screen: 800x480, e-ink avoids flashes with single updates.