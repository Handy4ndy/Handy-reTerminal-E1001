# Handy reTerminal E1001 Examples

![reTerminal E1001](https://files.seeedstudio.com/wiki/reterminal_e10xx/img/132.jpg)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A comprehensive collection of Arduino examples for the Seeed Studio reTerminal E1001, featuring a 7.5" e-ink display powered by ESP32-S3. This repository provides hands-on tutorials for button interactions, display rendering, image display, and advanced page navigation with USB serial logging.

## 🖥️ reTerminal E1001 Overview

The reTerminal E1001 is a versatile IoT device ideal for low-power applications:

- **Display**: 7.5" monochrome e-ink (800x480 resolution), ultra-low power consumption
- **Buttons**: 3 tactile buttons (GPIO5, GPIO4, GPIO3) for user input
- **Buzzer**: Piezo buzzer on GPIO45 for audio feedback
- **USB**: CDC serial interface for debugging and logging
- **Microcontroller**: ESP32-S3 with Arduino support
- **Programming**: Arduino IDE with XIAO_ESP32S3 board selection

## 📚 Examples

Dive into practical examples to master the reTerminal E1001:

### [SerialMonitor](SerialMonitor/)
- **Focus**: USB serial logging setup
- **Key Features**: Correct HardwareSerial initialization on GPIO44/43
- **Use Case**: Debugging and monitoring device output

### [Buttons](Buttons/)
- **Focus**: Button press detection with debouncing
- **Key Features**: Buzzer feedback, serial logging, interrupt-free polling
- **Use Case**: User interface prototyping

### [Display](Display/)
- **Focus**: Text rendering in multiple fonts
- **Key Features**: Centered text, GxEPD2 library integration
- **Use Case**: Status displays and information panels

### [Image](Image/)
- **Focus**: Full-screen bitmap display
- **Key Features**: Monochrome image rendering, seed800x480 bitmap
- **Use Case**: Logos, icons, or static graphics

### [Pages](Pages/)
- **Focus**: Interactive page navigation system
- **Key Features**: Partial updates, color inversion, buzzer tones
- **Use Case**: Multi-page applications with efficient e-ink updates

### [XahauDesktop](XahauDesktop/)
- **Focus**: Real-time Xahau network stream monitor
- **Key Features**: WiFi portal setup, WebSocket streaming, transaction visualization, high score tracking
- **Use Case**: Dedicated Xahau blockchain monitoring with e-ink display

### [XRPLedgerDesktop](XRPLedgerDesktop/)
- **Focus**: Real-time XRP Ledger stream monitor
- **Key Features**: WiFi portal setup, WebSocket streaming, transaction visualization, high score tracking
- **Use Case**: Dedicated XRPL blockchain monitoring with e-ink display

## 🚀 Getting Started

1. **Install Arduino IDE**: Version 1.8+ from [arduino.cc](https://www.arduino.cc/en/software)
2. **Add ESP32 Support**:
   - Open Arduino IDE
   - Go to File > Preferences > Additional Boards Manager URLs
   - Add: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools > Board > Boards Manager > Search "esp32" > Install
3. **Configure Board**:
   - Select "XIAO_ESP32S3" from Tools > Board
   - Enable "USB CDC On Boot" in Tools > USB CDC On Boot
4. **Install Libraries**:
   - Sketch > Include Library > Manage Libraries
   - Search and install "GxEPD2"
5. **Upload Code**:
   - Connect reTerminal E1001 via USB-C
   - Select port and upload sketch
6. **Monitor Output**:
   - Tools > Serial Monitor
   - Set baud rate to 115200

Each example folder contains a detailed README with code explanations and setup instructions.

## 📋 Requirements

- **Hardware**: Seeed Studio reTerminal E1001
- **Software**: Arduino IDE 1.8+
- **Libraries**: GxEPD2 (for e-ink display)
- **Dependencies**: ESP32 board support package

## 👨‍💻 About the Author

Handy4ndy is a passionate developer bridging blockchain technology with IoT and embedded systems. Specializing in XRPL (XRP Ledger) integrations and ESP32 projects, they contribute to open-source communities through innovative tools and examples.

### 🔗 Related Projects

Explore Handy4ndy's related ESP32 projects:

- **[ESP32-XRPL](https://github.com/Handy4ndy/ESP32-XRPL)**: ESP32-based XRPL account monitor using WebSockets for real-time blockchain tracking.
- **[Bitmap-Generator](https://github.com/Handy4ndy/Bitmap-Generator)**: Tool for converting PNG images to bitmap arrays for OLED and e-ink displays.


## 🤝 Contributing

Contributions are welcome! Feel free to:

- Report bugs or issues
- Suggest new examples
- Submit pull requests for improvements
- Share your reTerminal projects

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

*Built with ❤️ for the maker community. Happy coding!*"