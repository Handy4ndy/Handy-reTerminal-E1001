# Xahau Desktop

A minimal, dedicated Xahau network stream monitor for ESP32-S3 with e-Ink display. This streamlined version focuses on real-time ledger monitoring with zero configuration overhead.

![Version](https://img.shields.io/badge/version-1.1-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32--S3-green.svg)
![Network](https://img.shields.io/badge/network-Xahau-purple.svg)

## Features

✅ **Ultra-minimal** - Single purpose, zero menus, instant monitoring  
✅ **WiFi Portal** - Easy network setup via captive portal with persistent credentials  
✅ **Real-time Stream** - Live Xahau network data via WebSocket  
✅ **E-Ink Optimized** - Minimal flashing with partial updates  
✅ **Static Background** - Professional Xahau branding throughout  
✅ **Transaction Visualization** - Visual representation of ledger activity  
✅ **Server Stats** - Release version, peers, latency, state, and uptime  
✅ **High Score Tracking** - Persistent record of highest transaction count with ledger details  
✅ **Hardware Buttons** - Quick access to restart, high scores, and power management  

## What You Get

**No menus. No setup wizards. No account tracking.**  
Just pure, real-time Xahau network monitoring with persistent high score tracking.

### Left Panel
- Server hostname (xahau.network)
- Current ledger index
- Transaction count per ledger
- Visual transaction representation
- Educational link (xahau.network)

### Right Panel
- Release version (last 4 digits)
- Host ID
- IO latency
- Load factor
- Server state
- Peer count
- Converge time
- Uptime
- **OR** High Score display (when activated via button)

## About Xahau

Xahau is a Layer 1 blockchain extending XRPL technology with:
- **Smart Contract Hooks** - Programmable transaction logic
- **Governance System** - Decentralized decision making
- **Reward Mechanisms** - Built-in incentives
- **XRPL Compatibility** - Familiar tools and infrastructure

Learn more at [xahau.network](https://xahau.network)

## Hardware Requirements

- **MCU**: ESP32-S3 (reTerminal E1001 or compatible)
- **Display**: 7.5" GDEY075T7 e-Ink Display (800x480)
- **Interface**: HSPI
- **Power**: 5V USB-C

### Pin Configuration

```cpp
EPD_SCK_PIN   = 7   // SPI Clock
EPD_MOSI_PIN  = 9   // SPI Data
EPD_CS_PIN    = 10  // Chip Select
EPD_DC_PIN    = 11  // Data/Command
EPD_RES_PIN   = 12  // Reset
EPD_BUSY_PIN  = 13  // Busy Signal

BUTTON_LEFT   = 5   // Left button (GPIO5/KEY2) - Restart
BUTTON_MIDDLE = 4   // Middle button (GPIO4/KEY1) - High Score
BUTTON_RIGHT  = 3   // Right button (GPIO3/KEY0) - Power Off/On

SERIAL_RX_PIN = 44  // Debug RX
SERIAL_TX_PIN = 43  // Debug TX
```

## Software Requirements

### Arduino IDE Libraries
- **GxEPD2** - e-Ink display driver
- **WiFiManager** - Captive portal for WiFi setup
- **WebSocketsClient** - WebSocket connection to Xahau
- **ArduinoJson** - JSON parsing (v6 or v7)
- **Preferences** - Built-in ESP32 library for persistent storage

### Arduino Board Package
- **ESP32** by Espressif Systems (v2.0.17 or later)

## Installation

### 1. Clone or Download
```bash
git clone https://github.com/YourUsername/XahauDesktop.git
cd XahauDesktop
```

### 2. Install Libraries
In Arduino IDE:
- Go to **Sketch → Include Library → Manage Libraries**
- Install: `GxEPD2`, `WiFiManager`, `WebSocketsClient`, `ArduinoJson`

### 3. Configure Board
- Select **ESP32S3 Dev Module** from Tools → Board
- Set **USB CDC On Boot: Enabled**
- Set **Flash Size: 16MB**
- Set **Partition Scheme: Default**

### 4. Upload
- Connect your device via USB
- Click **Upload** (Ctrl+U)

## Hardware Buttons

### Left Button (GPIO5)
**Function**: Restart Device
- Press at any time to restart the ESP32
- Useful for WiFi reconfiguration or system reset
- 500ms debounce delay

### Middle Button (GPIO4)
**Function**: Show High Score
- Displays the highest transaction count ever recorded
- Shows ledger index and hash of the high score ledger
- Display remains for 5 seconds, then returns to server info
- High scores are saved permanently and survive reboots
- Serial output: "Middle button pressed - Showing high score"

### Right Button (GPIO3)
**Function**: Power Toggle
- **First press**: Hibernates the display (low power mode)
- **Second press**: Restarts the device
- Useful for saving power when not actively monitoring
- Serial output indicates hibernate and wake states

## First Run Setup

### 1. Power On
Device boots and displays Xahau background with logo.

### 2. WiFi Setup
- **Left side**: Shows Xahau network information
- **Right side**: Shows WiFi portal instructions

Connect to WiFi access point:
- **SSID**: `Xahau-Setup`
- **Portal**: `192.168.4.1`

Select your WiFi network and enter password.

### 3. Connection Status
After WiFi connects, right side displays:
- WiFi Connected!
- Your SSID name
- Device IP address
- "Connecting to ledger..."

### 4. Live Monitoring
Once first ledger data arrives, display automatically switches to live monitoring view.

## High Score Tracking

### Automatic Detection
The device automatically tracks the **highest transaction count** seen on any ledger:
- Monitors every ledger close event
- Compares transaction count against stored high score
- Automatically saves new records to persistent storage

### Stored Information
When a new high score is detected, the device saves:
- **Transaction Count**: Total number of transactions in that ledger
- **Ledger Index**: The ledger number where the record occurred
- **Ledger Hash**: Full hash of the record-breaking ledger

### Viewing High Score
- Press the **middle button** at any time
- Right panel switches to high score display for 5 seconds
- Shows transaction count, ledger index, and hash (first 10 + last 10 chars)
- Automatically returns to server info display
- Left panel continues live updates during high score display

### Persistence
- High scores are stored in ESP32 non-volatile memory (Preferences)
- Survives power cycles and restarts
- Loads automatically on boot
- Only updates when a new record is achieved

### Serial Output
New high scores are logged:
```
NEW HIGH SCORE! Txn Count: 1234
Ledger: 87654321
Hash: F1A2B3C4D5...
High score saved to preferences
```

## Usage

### Normal Operation
- Powers on → Shows background → WiFi portal → Connects → Live stream
- Updates automatically every 3-4 seconds (ledger close time)
- Server stats refresh every 5 ledgers
- High score tracking runs automatically in background
- Partial updates minimize e-Ink flashing

### Button Controls
- **Left Button**: Restart device (useful for WiFi reconfiguration)
- **Middle Button**: View high score (5-second display)
- **Right Button**: Hibernate display / Restart on second press

### WiFi Configuration
On first boot (or when no saved credentials exist):
1. Device creates `Xahau-Setup` access point
2. Connect to portal at `192.168.4.1`
3. Select your WiFi network and enter password
4. Credentials are saved and reused on subsequent boots

To change WiFi network:
- Press the **left button** to restart
- If saved WiFi is unavailable, portal recreates automatically

### Persistent Storage
The following data persists across reboots:
- WiFi credentials (via WiFiManager)
- High score transaction count
- High score ledger index
- High score ledger hash

### Reset WiFi
To force WiFi reconfiguration:
1. Uncomment these lines in the code:
   ```cpp
   wifiManager.resetSettings();
   usbSerial.println("WiFi settings cleared - starting portal");
   ```
2. Upload the modified sketch
3. Portal will appear on next boot
4. Re-comment the lines and upload again for normal operation

### Serial Debugging
Connect via USB Serial (115200 baud) to see:
- Connection status
- WebSocket events
- Ledger updates
- High score notifications
- Button press events
- Error messages

## Network Information

### Xahau Connection
- **Host**: xahau.network
- **Port**: 443 (WSS)
- **Protocol**: WebSocket Secure
- **Streams**: Ledger closed events
- **Commands**: `subscribe`, `server_info`

### Release Version Format
Xahau displays the last 4 digits of the build version after the "+" symbol.

Example: `2024.12.1+1234` → Display shows: `1234`

This provides a compact release identifier while maintaining readability on the e-Ink display.

### Data Refresh Rates
- **Ledger updates**: ~3-4 seconds (natural ledger close time)
- **Server stats**: Every 5 ledgers (~15-20 seconds)
- **Visual txn display**: Updates with each ledger close

## File Structure

```
XahauDesktop/
├── XahauDesktop.ino         # Main sketch with button controls & high scores
├── xahau800x480.h           # Xahau logo bitmap (800x480)
└── README.md                # This file
```

**Total**: ~510 lines of code in one file!

## Customization

### Change WebSocket Server
```cpp
// In setup()
webSocket.beginSSL("xahau.network", 443, "/");
// Replace with your preferred Xahau node
```

### Adjust Update Frequency
```cpp
// Server info refresh interval (in ledgers)
if (currentLedgerIndex >= lastUpdateLedger + 5) {
    // Change '5' to refresh more/less frequently
```

### Adjust High Score Display Duration
```cpp
#define HIGH_SCORE_DISPLAY_DURATION 5000  // 5 seconds
// Change to display high score for longer/shorter periods
```

### Modify Display Boundaries
```cpp
// Left panel:  0, 160, 400, 320
// Right panel: 430, 40, 320, 400
// Adjust in updateLedgerDisplay() and updateServerDisplay()
```

### Change Release Version Format
```cpp
// In webSocketEvent(), modify the substring extraction
int plusIndex = rawBuildVersion.indexOf('+');
if (plusIndex != -1 && rawBuildVersion.length() > plusIndex + 4) {
    buildVersion = rawBuildVersion.substring(plusIndex + 1, plusIndex + 5);
    // Change '5' to show more/fewer digits
}
```

### Disable Button Functions
```cpp
// Comment out button handling in loop()
// Or change GPIO pin assignments to unused pins
```

### Clear High Score
```cpp
// Add to setup() temporarily:
preferences.begin("xahau", false);
preferences.clear();
preferences.end();
// Upload, then remove the code
```

## Troubleshooting

### Display Not Updating
- Check SPI pin connections
- Verify BUSY pin responds (should go LOW when ready)
- Check serial output for errors

### WiFi Portal Not Appearing
- Wait 10 seconds after boot
- Check for `Xahau-Setup` network
- If WiFi credentials are saved, portal won't appear (working as intended)
- To force portal: comment out saved credentials or press left button to restart when WiFi unavailable

### No Ledger Data
- Verify internet connection
- Check serial output for WebSocket connection status
- Default timeout is 30 seconds
- Try power cycle if connection fails

### Buttons Not Responding
- Check GPIO pin connections (5, 4, 3)
- Buttons should be pulled HIGH (INPUT_PULLUP)
- Press should connect to GND
- Check serial output for button press confirmation
- 500ms debounce delay may require holding button briefly

### High Score Not Saving
- Check serial output for "High score saved to preferences"
- Verify Preferences library is included
- Try clearing preferences and starting fresh
- Ensure ESP32 has sufficient flash storage

### Display Stuck on High Score
- Wait for 5-second timeout
- Check serial output for "returning to server info"
- Press left button to restart if stuck

### Release Version Shows Full String
- Check that build version contains "+" character
- Fallback displays full version if parsing fails
- Modify substring logic in `webSocketEvent()` if needed

### Partial Update Issues
- E-Ink displays can ghost without full refresh
- Power cycle performs full refresh
- Partial updates minimize flashing but may show artifacts

## Technical Details

### Memory Usage
- **Program**: ~320KB
- **Dynamic Memory**: ~55KB
- **WebSocket Buffer**: 4KB JSON documents
- **Preferences**: ~100 bytes (high score data)

### Power Consumption
- **Active**: ~200mA (during update)
- **Idle**: ~150mA (monitoring)
- **E-Ink**: ~5mA (static display)

### Update Strategy
1. Full screen refresh on boot (background + logo)
2. Partial updates for left panel (ledger data)
3. Partial updates for right panel (server stats)
4. Minimizes flashing while maintaining readability

## Xahau-Specific Features

### Smart Contract Hooks
While this monitor doesn't display hook execution directly, it shows:
- Transaction counts including hook-triggered txns
- Server state reflecting hook processing load
- Network activity from automated hook workflows

### Governance
Monitor network health during governance votes:
- Peer count changes during amendments
- Server state during protocol upgrades
- Transaction volumes during voting periods

### Network Rewards
Track reward distribution activity:
- Transaction spikes during reward claims
- Ledger activity patterns
- Network participation metrics

## Future Enhancements

Potential additions (not included in current version):
- Hook execution statistics
- Governance vote tracking
- Reward distribution display
- Multiple Xahau node support
- Configurable sleep mode schedule
- OTA firmware updates
- Historical ledger graph
- QR code for high score sharing
- Buzzer alerts for new high scores
- Multi-network high score tracking

## Sister Project

Looking for XRP Ledger monitoring? Check out:
- **XRPLedgerDesktop** - Identical design for XRPL mainnet

## Key Differences from XRPL Version

| Feature | XRPL Version | Xahau Version |
|---------|--------------|---------------|
| Network | s2.ripple.com | xahau.network |
| WiFi Portal | XRPLedger-Setup | Xahau-Setup |
| Version Display | Full build string | Last 4 digits |
| Label | "Build:" | "Release:" |
| Info Text | Payments focus | Hooks & governance |
| Learn More | xrpl.org | xahau.network |

## License

MIT License - See LICENSE file for details

## Credits

- **Xahau Foundation** - Xahau network protocol
- **XRPL Labs** - Xahau development
- **xahau.network** - Public node infrastructure
- **GxEPD2** - E-Ink display library
- **Arduino** - Development framework

## Support

For issues, questions, or contributions:
- Open an issue on GitHub
- Check serial output for debugging
- Verify all library versions are up to date
- Visit [xahau.network](https://xahau.network) for network status

---

**Built with ❤️ for the Xahau community**

*Real-time monitoring. Zero complexity. Pure functionality.*
