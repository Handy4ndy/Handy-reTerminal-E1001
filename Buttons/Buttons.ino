/*
  Buttons for reTerminal E1001
  Monitors three buttons with debouncing and buzzer feedback.

  Left: GPIO5, Middle: GPIO4, Right: GPIO3, Buzzer: GPIO45.

  Author: @Handy_4ndy
  Date: December 2025
*/

// Define button pins (GPIO numbers on the ESP32)
const int BUTTON_LEFT = 5;   // Left button connected to GPIO5
const int BUTTON_MIDDLE = 4; // Middle button connected to GPIO4
const int BUTTON_RIGHT = 3;  // Right button connected to GPIO3
const int BUZZER_PIN = 45;   // Buzzer connected to GPIO45

// Variables to remember the last state of each button
bool lastLeft = HIGH, lastMiddle = HIGH, lastRight = HIGH;

// Use Serial1 for USB serial communication (for logging)
HardwareSerial &usbSerial = Serial1;

void setup()
{
    // Start USB serial at 115200 baud rate
    usbSerial.begin(115200, SERIAL_8N1, 44, 43);
    delay(500); // Wait a bit for serial to be ready

    usbSerial.println("Button Monitoring Started");

    // Set buttons as inputs with pull-up resistors (HIGH when not pressed)
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_MIDDLE, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    // Set buzzer pin as output
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
    // Check left button
    bool currentLeft = digitalRead(BUTTON_LEFT); // Read current state
    if (currentLeft != lastLeft)                 // If state changed
    {
        delay(50);              // Debounce delay to avoid false presses
        if (currentLeft == LOW) // Button is pressed (LOW when pressed)
        {
            usbSerial.println("Left Button Pressed - Navigate Page Left?");
            // You can add code here to go to previous page
            tone(BUZZER_PIN, 1000, 500); // Play a 1kHz beep for 500ms
        }
        else
        {
            usbSerial.println("Left Button Released");
        }
        lastLeft = currentLeft; // Update last state
    }

    // Check middle button (same logic as left)
    bool currentMiddle = digitalRead(BUTTON_MIDDLE);
    if (currentMiddle != lastMiddle)
    {
        delay(50);
        if (currentMiddle == LOW)
        {
            usbSerial.println("Middle Button Pressed - Navigate Page Right?");
            // You can add code here to go to next page
            tone(BUZZER_PIN, 1000, 500);
        }
        else
        {
            usbSerial.println("Middle Button Released");
        }
        lastMiddle = currentMiddle;
    }

    // Check right button (same logic as left)
    bool currentRight = digitalRead(BUTTON_RIGHT);
    if (currentRight != lastRight)
    {
        delay(50);
        if (currentRight == LOW)
        {
            usbSerial.println("Right Button Pressed - Refresh Dashboard?");
            // You can add code here to refresh the display
            tone(BUZZER_PIN, 1000, 500);
        }
        else
        {
            usbSerial.println("Right Button Released");
        }
        lastRight = currentRight;
    }

    // Small delay to prevent the loop from running too fast
    delay(10);
}