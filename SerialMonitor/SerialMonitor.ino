/*
  SerialMonitor for reTerminal E1001
  Demonstrates correct USB serial setup for logging on ESP32-S3.

  Uses Serial1 on GPIO44/43 for USB CDC.

  Author: @Handy_4ndy
  Date: December 2025

*/

// Create a reference to Serial1 for easier use (USB serial communication)
HardwareSerial &usbSerial = Serial1;

void setup()
{
  // Start USB serial at 115200 baud rate on specific pins
  usbSerial.begin(115200, SERIAL_8N1, 44, 43);
  delay(500);                                      // Wait a bit for serial to be ready
  usbSerial.println("USB Serial Logging Started"); // Print a startup message
}

void loop()
{
  usbSerial.println("Log message"); // Send a message to the serial monitor
  delay(1000);                      // Wait 1 second before sending the next message
}