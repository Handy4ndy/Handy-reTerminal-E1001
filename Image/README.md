# ESP32-S3 e-Ink Image Display (reTerminal E1001)

## Overview
Displays a full-screen monochrome bitmap on the 7.5" e-ink display using GxEPD2. Includes USB serial logging for monitoring.

Benefits:
- Shows bitmap rendering on e-ink.
- Single update for no flashes.
- USB logging for feedback.

## Example Code
```cpp
// Includes and setup for GxEPD2, SPI, usbSerial
// In setup: init display, draw bitmap
display.setFullWindow();
display.firstPage();
do {
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(0, 0, seed800x480, 800, 480, GxEPD_BLACK);
} while (display.nextPage());
```

## Tips
- Set Arduino IDE board to "XIAO_ESP32S3".
- Enable "USB CDC On Boot".
- Use 115200 baud.
- Power cycle after upload.
- Bitmap: 800x480, ensure header file is included.