/**
 * Xahau Desktop - Minimal Version
 * Dedicated Xahau network stream monitor with WiFi portal
 * 
 * Features:
 * - WiFi connection via captive portal
 * - Real-time Xahau stream display
 * - No menus, no account info, no network selection
 * - Single static background throughout
 */

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "xahau800x480.h"

// =============================================================================
// HARDWARE CONFIGURATION
// =============================================================================

#define BITMAP_WIDTH 800
#define BITMAP_HEIGHT 480

// Display pins
#define EPD_SCK_PIN 7
#define EPD_MOSI_PIN 9
#define EPD_CS_PIN 10
#define EPD_DC_PIN 11
#define EPD_RES_PIN 12
#define EPD_BUSY_PIN 13

// Serial communication
#define SERIAL_BAUD_RATE 115200
#define SERIAL_RX_PIN 44
#define SERIAL_TX_PIN 43

// Button pins
#define BUTTON_LEFT 5    // Left button (GPIO5/KEY2) - Restart
#define BUTTON_MIDDLE 4  // Middle button (GPIO4/KEY1) - High Score
#define BUTTON_RIGHT 3   // Right button (GPIO3/KEY0) - Power Off

// =============================================================================
// GLOBAL OBJECTS
// =============================================================================

// Display and SPI
GxEPD2_BW<GxEPD2_750_GDEY075T7, GxEPD2_750_GDEY075T7::HEIGHT> display(GxEPD2_750_GDEY075T7(EPD_CS_PIN, EPD_DC_PIN, EPD_RES_PIN, EPD_BUSY_PIN));
SPIClass hspi(HSPI);
HardwareSerial &usbSerial = Serial1;

// WebSocket
WebSocketsClient webSocket;

// Preferences for persistent storage
Preferences preferences;

// =============================================================================
// LEDGER DATA
// =============================================================================

int currentLedgerIndex = 0;
int txnCount = 0;
String buildVersion = "";
String hostId = "";
int ioLatencyMs = 0;
float convergeTimeS = 0.0;
int peers = 0;
float loadFactor = 0.0;
String serverState = "";
int uptime = 0;
int lastUpdateLedger = 0;
bool firstDataReceived = false;

// High score tracking
int highScoreTxnCount = 0;
int highScoreLedgerIndex = 0;
String highScoreLedgerHash = "";
bool showingHighScore = false;
unsigned long highScoreDisplayStart = 0;
#define HIGH_SCORE_DISPLAY_DURATION 5000  // 5 seconds

// =============================================================================
// DISPLAY FUNCTIONS
// =============================================================================

void drawBackground() {
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_BLACK);
        display.drawBitmap(0, 0, xahau800x480, BITMAP_WIDTH, BITMAP_HEIGHT, GxEPD_WHITE);
    } while (display.nextPage());
}

void printCenteredLeft(const char *text, int y) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int x = 20 + (370 - w) / 2;
    display.setCursor(x, y);
    display.print(text);
}

void printCenteredRight(const char *text, int y) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int x = 440 + (310 - w) / 2;
    display.setCursor(x, y);
    display.print(text);
}

void updateLedgerDisplay() {
    display.setPartialWindow(0, 160, 400, 320);
    display.firstPage();
    do {
        display.fillRect(0, 160, 400, 320, GxEPD_WHITE);
        display.setFont(&FreeMonoBold12pt7b);
        display.setTextColor(GxEPD_BLACK);

        // Ledger info
        display.setCursor(20, 180);
        display.print("Server:");
        display.setCursor(180, 180);
        display.print("xahau.network");

        display.setCursor(20, 220);
        display.print("Ledger:");
        display.setCursor(180, 220);
        display.print(String(currentLedgerIndex));

        display.setCursor(20, 260);
        display.print("Txn Count:");
        display.setCursor(180, 260);
        display.print(String(txnCount));

        // Transaction visualization
        int startY = 300;
        int endY = 420;
        int boxX = 20;
        int boxWidth = 360;
        int pixelMargin = 10;
        int bottomMargin = 5;
        int pixelAreaWidth = boxWidth - 2 * pixelMargin;
        
        display.drawRoundRect(boxX, startY, boxWidth, endY - startY, 10, GxEPD_BLACK);
        
        int squareSize = (txnCount > 500) ? 3 : 6;
        int margin = (txnCount > 1250) ? 0 : 1;
        int numSquares = min(txnCount, 600);
        
        for (int i = 0; i < numSquares; i++) {
            int x = boxX + pixelMargin + (i * (squareSize + margin)) % pixelAreaWidth;
            int y = (endY - bottomMargin) - squareSize - ((i * (squareSize + margin)) / pixelAreaWidth) * (squareSize + margin);
            if (y >= startY + pixelMargin && x + squareSize <= boxX + boxWidth - pixelMargin) {
                display.fillRect(x, y, squareSize, squareSize, GxEPD_BLACK);
            }
        }

        printCenteredLeft("Learn more at xahau.network", 450);
    } while (display.nextPage());
    
    delay(100);
}

void updateServerDisplay() {
    display.setPartialWindow(430, 40, 320, 400);
    display.firstPage();
    do {
        display.fillRect(420, 40, 330, 400, GxEPD_WHITE);
        display.setFont(&FreeMonoBold12pt7b);
        display.setTextColor(GxEPD_BLACK);

        display.setCursor(450, 80);
        display.print("Release:");
        display.setCursor(620, 80);
        display.print(buildVersion);

        display.setCursor(450, 120);
        display.print("Host:");
        display.setCursor(620, 120);
        display.print(hostId);

        display.setCursor(450, 160);
        display.print("Latency:");
        display.setCursor(620, 160);
        display.print(String(ioLatencyMs) + "ms");

        display.setCursor(450, 200);
        display.print("Load:");
        display.setCursor(620, 200);
        display.print(String(loadFactor));

        display.setCursor(450, 240);
        display.print("State:");
        display.setCursor(620, 240);
        display.print(serverState);

        display.setCursor(450, 280);
        display.print("Peers:");
        display.setCursor(620, 280);
        display.print(String(peers));

        display.setCursor(450, 320);
        display.print("Converge:");
        display.setCursor(620, 320);
        display.print(String(convergeTimeS) + "s");

        display.setCursor(450, 360);
        display.print("Uptime:");
        display.setCursor(620, 360);
        display.print(String(uptime) + "s");

        printCenteredRight("Xahau Network Stream", 420);
    } while (display.nextPage());
    
    delay(100);
}

void updateHighScoreDisplay() {
    display.setPartialWindow(430, 40, 320, 400);
    display.firstPage();
    do {
        display.fillRect(420, 40, 330, 400, GxEPD_WHITE);
        display.setFont(&FreeMonoBold18pt7b);
        display.setTextColor(GxEPD_BLACK);
        
        printCenteredRight("HIGH SCORE!", 100);
        
        display.setFont(&FreeMonoBold12pt7b);
        display.setCursor(450, 180);
        display.print("Txn Count:");
        display.setCursor(620, 180);
        display.print(String(highScoreTxnCount));

        display.setCursor(450, 240);
        display.print("Ledger:");
        display.setCursor(620, 240);
        display.print(String(highScoreLedgerIndex));

        display.setCursor(450, 300);
        display.print("Hash:");
        display.setCursor(450, 330);
        display.print(highScoreLedgerHash.substring(0, 10) + "...");
        display.setCursor(450, 360);
        display.print(highScoreLedgerHash.substring(highScoreLedgerHash.length() - 10));

        printCenteredRight("Record Breaking!", 420);
    } while (display.nextPage());
    
    delay(100);
}

// =============================================================================
// WEBSOCKET FUNCTIONS
// =============================================================================

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            usbSerial.println("WebSocket Disconnected");
            break;
            
        case WStype_CONNECTED:
            usbSerial.println("WebSocket Connected to Xahau Network");
            webSocket.sendTXT("{\"command\": \"subscribe\", \"streams\": [\"ledger\"]}");
            break;
            
        case WStype_TEXT: {
            String message = String((char*)payload);
            
            DynamicJsonDocument doc(4096);
            DeserializationError error = deserializeJson(doc, message);
            if (error) {
                usbSerial.print("JSON parse failed: ");
                usbSerial.println(error.c_str());
                return;
            }

            // Handle ledger closed event
            if (doc.containsKey("type") && doc["type"].as<String>() == "ledgerClosed") {
                currentLedgerIndex = (int)doc["ledger_index"];
                txnCount = (int)doc["txn_count"];
                String ledgerHash = doc["ledger_hash"].as<String>();
                
                if (!firstDataReceived) {
                    firstDataReceived = true;
                    usbSerial.println("First ledger data received - initializing display");
                }
                
                // Check for new high score
                if (txnCount > highScoreTxnCount) {
                    highScoreTxnCount = txnCount;
                    highScoreLedgerIndex = currentLedgerIndex;
                    highScoreLedgerHash = ledgerHash;
                    usbSerial.println("NEW HIGH SCORE! Txn Count: " + String(highScoreTxnCount));
                    usbSerial.println("Ledger: " + String(highScoreLedgerIndex));
                    usbSerial.println("Hash: " + highScoreLedgerHash);
                    
                    // Save high score to preferences
                    preferences.begin("xahau", false);
                    preferences.putInt("hsTxnCount", highScoreTxnCount);
                    preferences.putInt("hsLedgerIdx", highScoreLedgerIndex);
                    preferences.putString("hsHash", highScoreLedgerHash);
                    preferences.end();
                    usbSerial.println("High score saved to preferences");
                }
                
                updateLedgerDisplay();

                if (currentLedgerIndex >= lastUpdateLedger + 5) {
                    webSocket.sendTXT("{\"command\": \"server_info\", \"ledger_index\": \"current\"}");
                    lastUpdateLedger = currentLedgerIndex;
                }
            }

            // Handle server_info response
            if (doc.containsKey("result") && doc["result"].containsKey("info")) {
                JsonObject info = doc["result"]["info"];
                String rawBuildVersion = info["build_version"].as<String>();
                
                // For Xahau, extract the last 4 digits after "+" in the release version
                int plusIndex = rawBuildVersion.indexOf('+');
                if (plusIndex != -1 && rawBuildVersion.length() > plusIndex + 4) {
                    buildVersion = rawBuildVersion.substring(plusIndex + 1, plusIndex + 5);
                } else {
                    buildVersion = rawBuildVersion; // Fallback if parsing fails
                }
                hostId = info["hostid"].as<String>();
                ioLatencyMs = info["io_latency_ms"].as<int>();
                convergeTimeS = info["last_close"]["converge_time_s"].as<float>();
                peers = info["peers"].as<int>();
                loadFactor = info["load_factor"].as<float>();
                serverState = info["server_state"].as<String>();
                uptime = info["uptime"].as<int>();
                updateServerDisplay();
            }
            break;
        }
    }
}

// =============================================================================
// WIFI SETUP
// =============================================================================

void setupWiFi() {
    usbSerial.println("Starting WiFi setup...");
    
    // Update only left side with XRP Ledger info (background already drawn in setup)
    display.setPartialWindow(0, 160, 400, 320);
    display.firstPage();
    do {
        display.fillRect(0, 160, 400, 320, GxEPD_WHITE);
        
        display.setFont(&FreeMonoBold18pt7b);
        display.setTextColor(GxEPD_BLACK);
        printCenteredLeft("Xahau Network", 200);
        
        display.setFont(&FreeMonoBold12pt7b);
        printCenteredLeft("Layer 1 blockchain with", 250);
        printCenteredLeft("smart contract hooks,", 280);
        printCenteredLeft("governance & rewards.", 310);
        printCenteredLeft("Extending XRPL technology", 360);
        printCenteredLeft("for the next generation.", 390);
        printCenteredLeft("Learn more at xahau.network", 440);
    } while (display.nextPage());
    
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(180);
    
    //// Reset WiFi settings - force captive portal every time (uncomment to enable)
    // wifiManager.resetSettings();
    // usbSerial.println("WiFi settings cleared - starting portal");
    
    // Update only right side with WiFi portal info
    display.setPartialWindow(430, 40, 320, 400);
    display.firstPage();
    do {
        display.fillRect(420, 40, 330, 400, GxEPD_WHITE);
        display.setFont(&FreeMonoBold12pt7b);
        display.setTextColor(GxEPD_BLACK);
        printCenteredRight("Connect to WiFi:", 180);
        printCenteredRight("Xahau-Setup", 240);
        printCenteredRight("192.168.4.1", 300);
    } while (display.nextPage());
    
    if (!wifiManager.autoConnect("Xahau-Setup")) {
        usbSerial.println("Failed to connect - restarting");
        display.setPartialWindow(430, 40, 320, 400);
        display.firstPage();
        do {
            display.fillRect(420, 40, 330, 400, GxEPD_WHITE);
            display.setFont(&FreeMonoBold12pt7b);
            display.setTextColor(GxEPD_BLACK);
            printCenteredRight("WiFi failed!", 180);
            printCenteredRight("Restarting...", 240);
        } while (display.nextPage());
        delay(3000);
        ESP.restart();
    }
    
    usbSerial.println("WiFi connected!");
    usbSerial.print("IP: ");
    usbSerial.println(WiFi.localIP());
    
    // Show success with SSID and IP on right side
    display.setPartialWindow(430, 40, 320, 400);
    display.firstPage();
    do {
        display.fillRect(420, 40, 330, 400, GxEPD_WHITE);
        display.setFont(&FreeMonoBold12pt7b);
        display.setTextColor(GxEPD_BLACK);
        printCenteredRight("WiFi Connected!", 180);
        printCenteredRight(WiFi.SSID().c_str(), 240);
        printCenteredRight(WiFi.localIP().toString().c_str(), 300);
        printCenteredRight("Connecting to ledger...", 360);
    } while (display.nextPage());
}

// =============================================================================
// SETUP & LOOP
// =============================================================================

void setup() {
    // Initialize serial
    usbSerial.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL_RX_PIN, SERIAL_TX_PIN);
    delay(500);
    usbSerial.println("\n=== Xahau Desktop ===");

    // Initialize SPI
    hspi.begin(EPD_SCK_PIN, -1, EPD_MOSI_PIN, -1);
    
    // Initialize display
    display.epd2.selectSPI(hspi, SPISettings(2000000, MSBFIRST, SPI_MODE0));
    display.init(SERIAL_BAUD_RATE, true);
    display.setRotation(0);
    
    usbSerial.println("Display initialized");

    // Initialize buttons
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_MIDDLE, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    usbSerial.println("Buttons initialized");

    // Load high score from preferences
    preferences.begin("xahau", true);  // Read-only mode
    highScoreTxnCount = preferences.getInt("hsTxnCount", 0);
    highScoreLedgerIndex = preferences.getInt("hsLedgerIdx", 0);
    highScoreLedgerHash = preferences.getString("hsHash", "");
    preferences.end();
    
    if (highScoreTxnCount > 0) {
        usbSerial.println("Loaded high score from preferences:");
        usbSerial.println("  Txn Count: " + String(highScoreTxnCount));
        usbSerial.println("  Ledger: " + String(highScoreLedgerIndex));
        usbSerial.println("  Hash: " + highScoreLedgerHash);
    } else {
        usbSerial.println("No previous high score found");
    }

    // Draw background once
    drawBackground();
    
    // Setup WiFi
    setupWiFi();
    
    // Connect to Xahau Network WebSocket
    usbSerial.println("Connecting to Xahau Network...");
    webSocket.beginSSL("xahau.network", 443, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(4000);
    
    // Wait for first data before continuing
    usbSerial.println("Waiting for first ledger data...");
    unsigned long startWait = millis();
    while (!firstDataReceived && millis() - startWait < 30000) {
        webSocket.loop();
        delay(100);
    }
    
    if (firstDataReceived) {
        usbSerial.println("Setup complete - monitoring ledger stream");
    } else {
        usbSerial.println("Timeout waiting for ledger data");
    }
}

void loop() {
    webSocket.loop();
    
    // Check if high score display timeout has elapsed
    if (showingHighScore && (millis() - highScoreDisplayStart >= HIGH_SCORE_DISPLAY_DURATION)) {
        showingHighScore = false;
        updateServerDisplay();  // Return to server info display
        usbSerial.println("High score display timeout - returning to server info");
    }
    
    // Handle button presses
    if (digitalRead(BUTTON_LEFT) == LOW) {
        usbSerial.println("Left button pressed - Restarting device...");
        delay(500);  // Debounce
        ESP.restart();
    }
    
    if (digitalRead(BUTTON_MIDDLE) == LOW) {
        usbSerial.println("Middle button pressed - Showing high score");
        if (highScoreTxnCount > 0) {
            showingHighScore = true;
            highScoreDisplayStart = millis();
            updateHighScoreDisplay();
        } else {
            usbSerial.println("No high score recorded yet");
        }
        delay(500);  // Debounce
    }
    
    if (digitalRead(BUTTON_RIGHT) == LOW) {
        usbSerial.println("Right button pressed - Powering down display...");
        display.hibernate();
        usbSerial.println("Display hibernated. Press button again to restart.");
        delay(500);  // Debounce
        
        // Wait for button release
        while (digitalRead(BUTTON_RIGHT) == LOW) {
            delay(50);
        }
        
        // Stay in hibernated state until button is pressed again
        while (digitalRead(BUTTON_RIGHT) == HIGH) {
            delay(100);
        }
        
        // Button pressed again - restart
        usbSerial.println("Right button pressed again - Restarting device...");
        delay(500);
        ESP.restart();
    }
    
    delay(100);
}
