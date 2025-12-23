/*
  Image for reTerminal E1001
  Displays a monochrome bitmap image on the 7.5" e-ink display.

  Uses GxEPD2 library with seed800x480 bitmap.

  Author: @Handy_4ndy
  Date: December 2025
*/

// Include library for the e-ink display
#include <GxEPD2_BW.h> // Main library for black and white e-ink displays

// Include the bitmap image data (this file contains the image as code)
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

void setup()
{
    // Start USB serial at 115200 baud rate
    usbSerial.begin(115200, SERIAL_8N1, 44, 43);
    delay(500); // Wait for serial to be ready
    usbSerial.println("Image Display Started");

    // Start SPI communication for the display
    hspi.begin(EPD_SCK_PIN, -1, EPD_MOSI_PIN, -1);
    usbSerial.println("HSPI initialized");

    // Tell the display to use our SPI settings
    display.epd2.selectSPI(hspi, SPISettings(2000000, MSBFIRST, SPI_MODE0));

    // Initialize the display
    display.init(115200, true);
    usbSerial.println("Display initialized!");

    display.setRotation(0); // Set display orientation (0 = normal)

    // Draw the image on the screen
    display.setFullWindow(); // Update the whole screen
    display.firstPage();     // Start drawing
    do
    {
        display.fillScreen(GxEPD_BLACK); // Fill background with black
        // Draw the bitmap at position (0,0), using white color for the image
        display.drawBitmap(0, 0, seed800x480, BITMAP_WIDTH, BITMAP_HEIGHT, GxEPD_WHITE);
    } while (display.nextPage()); // Finish drawing

    usbSerial.println("Image displayed.");
}

void loop()
{
    usbSerial.println("Image display loop running...");
    delay(5000); // Wait 5 seconds before printing again
}