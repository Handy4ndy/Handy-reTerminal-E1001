/*
  Pages for reTerminal E1001
  Page navigation system with buttons, partial updates, and color inversion.

  Left: Previous page, Middle: Next page, Right: Invert colors.
  Uses GxEPD2 with buzzer feedback.

  Author: @Handy_4ndy
  Date: December 2025
*/

// Include libraries for the e-ink display
#include <GxEPD2_BW.h>                // Main library for black and white e-ink displays
#include <Fonts/FreeMonoBold18pt7b.h> // Font for text

// Include the bitmap image data
#include "seed800x480.h"

// Define the size of the bitmap image
#define BITMAP_WIDTH 800  // Width in pixels
#define BITMAP_HEIGHT 480 // Height in pixels

// Define pins connected to the display (SPI communication)
#define EPD_SCK_PIN 7   // Clock pin
#define EPD_MOSI_PIN 9  // Data pin
#define EPD_CS_PIN 10   // Chip select (tells display to listen)
#define EPD_DC_PIN 11   // Data/command (switches between sending data or commands)
#define EPD_RES_PIN 12  // Reset pin
#define EPD_BUSY_PIN 13 // Busy pin (tells when display is ready)

// Create display object for the 7.5" e-ink screen
GxEPD2_BW<GxEPD2_750_GDEY075T7, GxEPD2_750_GDEY075T7::HEIGHT> display(GxEPD2_750_GDEY075T7(EPD_CS_PIN, EPD_DC_PIN, EPD_RES_PIN, EPD_BUSY_PIN));

// Create SPI object for fast communication with the display
SPIClass hspi(HSPI);

// Use Serial1 for USB serial communication (for logging)
HardwareSerial &usbSerial = Serial1;

// Define button pins (GPIO numbers on the ESP32)
const int BUTTON_LEFT = 5;   // Left button connected to GPIO5
const int BUTTON_MIDDLE = 4; // Middle button connected to GPIO4
const int BUTTON_RIGHT = 3;  // Right button connected to GPIO3
const int BUZZER_PIN = 45;   // Buzzer connected to GPIO45

// Variables to remember the last state of each button
bool lastLeft = HIGH, lastMiddle = HIGH, lastRight = HIGH;

// Variables for page navigation
int currentPage = 0;        // Current page number (starts at 0)
const int totalPages = 4;   // Total number of pages
bool inverted = false;      // Whether colors are inverted (black/white swapped)
bool initialDisplay = true; // Flag for first display

// Function to center text on the screen
void printCentered(const char *text, int y)
{
    int16_t x1, y1;                                      // Temporary variables for text bounds
    uint16_t w, h;                                       // Width and height of the text
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h); // Get text size
    int x = (800 - w) / 2;                               // Calculate center x position (screen is 800 wide)
    display.setCursor(x, y);                             // Move cursor to centered position
    display.print(text);                                 // Print the text
}

// Function to display the current page
void showPage(int page, bool forceFull = false)
{
    bool doFull = initialDisplay || forceFull; // Do full update on first time or when forced
    initialDisplay = false;

    if (doFull)
    {
        // Full update: redraws the entire screen (slower but needed for big changes)
        display.setFullWindow();
        display.firstPage();
        do
        {
            // Set colors based on inversion
            uint16_t fgColor = inverted ? GxEPD_BLACK : GxEPD_WHITE; // Foreground (bitmap)
            uint16_t bgColor = inverted ? GxEPD_WHITE : GxEPD_BLACK; // Background

            display.fillScreen(bgColor); // Fill screen with background color
            // Draw the bitmap image
            display.drawBitmap(0, 0, seed800x480, BITMAP_WIDTH, BITMAP_HEIGHT, fgColor);

            // Add page number text at the bottom
            display.setFont(&FreeMonoBold18pt7b);
            display.setTextColor(bgColor);            // Text color matches background for contrast
            String text = "Page " + String(page + 1); // Page numbers start from 1
            printCentered(text.c_str(), 440);
        } while (display.nextPage());
    }
    else
    {
        // Partial update: only redraws a small area (faster for text changes)
        display.setPartialWindow(0, 400, 800, 80); // Update only bottom area
        display.firstPage();
        do
        {
            // Set colors based on inversion
            uint16_t fgColor = inverted ? GxEPD_BLACK : GxEPD_WHITE;
            uint16_t bgColor = inverted ? GxEPD_WHITE : GxEPD_BLACK;

            display.fillRect(0, 400, 800, 80, fgColor); // Clear text area

            // Add page number text
            display.setFont(&FreeMonoBold18pt7b);
            display.setTextColor(bgColor);
            String text = "Page " + String(page + 1);
            printCentered(text.c_str(), 440);
        } while (display.nextPage());
    }

    // Log what was displayed
    usbSerial.print("Displayed page ");
    usbSerial.print(page + 1);
    usbSerial.print(" (");
    usbSerial.print(inverted ? "inverted" : "normal");
    usbSerial.println(")");
}

void setup()
{
    // Start USB serial at 115200 baud rate
    usbSerial.begin(115200, SERIAL_8N1, 44, 43);
    delay(500); // Wait for serial to be ready
    usbSerial.println("Page Display Started");

    // Set buttons as inputs with pull-up resistors
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_MIDDLE, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT); // Buzzer as output

    // Start SPI communication for the display
    hspi.begin(EPD_SCK_PIN, -1, EPD_MOSI_PIN, -1);
    usbSerial.println("HSPI initialized");

    // Tell the display to use our SPI settings
    display.epd2.selectSPI(hspi, SPISettings(2000000, MSBFIRST, SPI_MODE0));

    // Initialize the display
    display.init(115200, true);
    usbSerial.println("Display initialized!");

    display.setRotation(0); // Set display orientation

    // Show the first page
    showPage(currentPage);
    usbSerial.println("Setup complete.");
}

void loop()
{
    // Check left button for previous page
    bool currentLeft = digitalRead(BUTTON_LEFT);
    if (currentLeft != lastLeft)
    {
        delay(50); // Debounce delay
        if (currentLeft == LOW)
        {                                // Button pressed
            tone(BUZZER_PIN, 1000, 300); // Beep
            usbSerial.println("Left Button Pressed - Previous Page");
            currentPage = (currentPage - 1 + totalPages) % totalPages; // Go to previous page
            showPage(currentPage);
        }
        else
        {
            usbSerial.println("Left Button Released");
        }
        lastLeft = currentLeft;
    }

    // Check middle button for next page
    bool currentMiddle = digitalRead(BUTTON_MIDDLE);
    if (currentMiddle != lastMiddle)
    {
        delay(50);
        if (currentMiddle == LOW)
        {
            tone(BUZZER_PIN, 1000, 300);
            usbSerial.println("Middle Button Pressed - Next Page");
            currentPage = (currentPage + 1) % totalPages; // Go to next page
            showPage(currentPage);
        }
        else
        {
            usbSerial.println("Middle Button Released");
        }
        lastMiddle = currentMiddle;
    }

    // Check right button for color inversion
    bool currentRight = digitalRead(BUTTON_RIGHT);
    if (currentRight != lastRight)
    {
        delay(50);
        if (currentRight == LOW)
        {
            tone(BUZZER_PIN, 1000, 300);
            usbSerial.println("Right Button Pressed - Invert Colors");
            inverted = !inverted;        // Toggle inversion
            showPage(currentPage, true); // Force full update for color change
        }
        else
        {
            usbSerial.println("Right Button Released");
        }
        lastRight = currentRight;
    }

    delay(10); // Small delay to prevent loop from running too fast
}