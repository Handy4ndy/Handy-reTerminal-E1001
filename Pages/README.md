# ESP32-S3 e-Ink Page Navigation (reTerminal E1001)

## Overview
Implements page navigation with physical buttons on the 7.5" e-ink display. Shows the same bitmap across 4 pages with centered page indicators, color inversion, and buzzer feedback. Uses partial updates for text changes to minimize flashes.

Benefits:
- Hardware button interaction with debouncing.
- Partial updates for efficient text refreshes.
- Buzzer tones and USB logging for feedback.

## Example Code
```cpp
// Button checks in loop
if (currentLeft == LOW) {
  tone(BUZZER_PIN, 1000, 300);
  usbSerial.println("Left Button Pressed - Previous Page");
  currentPage = (currentPage - 1 + totalPages) % totalPages;
  showPage(currentPage);
}
// Similar for middle (next) and right (invert)
```

## Tips
- Set Arduino IDE board to "XIAO_ESP32S3".
- Enable "USB CDC On Boot".
- Use 115200 baud.
- Power cycle after upload.
- Buttons: Left (prev), Middle (next), Right (invert).
- Buzzer on GPIO45 for 300ms beeps.