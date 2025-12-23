/*
  Display for reTerminal E1001
  Displays "Hello World!" in multiple font sizes on the 7.5" e-ink display.

  Uses GxEPD2 library with centered text.

  Author: @Handy_4ndy
  Date: December 2025
*/

// Include libraries for the e-ink display
#include <GxEPD2_BW.h>                // Main library for black and white e-ink displays
#include <Fonts/FreeMonoBold9pt7b.h>  // Small font for text
#include <Fonts/FreeMonoBold18pt7b.h> // Medium font for text
#include <Fonts/FreeMonoBold24pt7b.h> // Large font for text

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

void setup()
{
    // Start USB serial at 115200 baud rate
    usbSerial.begin(115200, SERIAL_8N1, 44, 43);
    delay(500); // Wait for serial to be ready
    usbSerial.println("Display Hello World Started");

    // Start SPI communication for the display
    hspi.begin(EPD_SCK_PIN, -1, EPD_MOSI_PIN, -1);
    usbSerial.println("HSPI initialized");

    // Tell the display to use our SPI settings
    display.epd2.selectSPI(hspi, SPISettings(2000000, MSBFIRST, SPI_MODE0));

    // Initialize the display
    display.init(115200, true);
    usbSerial.println("Display initialized!");

    display.setRotation(0);              // Set display orientation (0 = normal)
    display.setFont(&FreeMonoBold9pt7b); // Start with small font
    display.setTextColor(GxEPD_BLACK);   // Set text color to black

    // Draw everything at once to avoid flickering
    display.setFullWindow(); // Update the whole screen
    display.firstPage();     // Start drawing
    do
    {
        display.fillScreen(GxEPD_WHITE); // Fill background with white
        // Small size text
        display.setFont(&FreeMonoBold9pt7b);
        printCentered("Small: Hello World!", 140);
        // Medium size text
        display.setFont(&FreeMonoBold18pt7b);
        printCentered("Medium: Hello World!", 240);
        // Large size text
        display.setFont(&FreeMonoBold24pt7b);
        printCentered("Large: Hello World!", 340);
    } while (display.nextPage()); // Finish drawing

    usbSerial.println("Hello World displayed.");
}

void loop()
{
    usbSerial.println("Display loop running...");
    delay(5000); // Wait 5 seconds before printing again
}