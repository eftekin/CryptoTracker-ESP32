# CryptoTracker ESP32

CryptoTracker ESP32 is a real-time cryptocurrency price tracker built on an ESP32 microcontroller. It fetches live coin data from a JSON file hosted online and displays current prices on a 16x2 I2C LCD screen.

## Features

- Fetches live cryptocurrency prices.
- Supports multiple coins from a remote JSON list.
- Displays real-time data on a 16x2 I2C LCD.
- Switches between different coins at set intervals.
- Uses `secrets.h` to keep API keys and Wi-Fi credentials safe.

## Hardware Requirements

- ESP32 development board
- 16x2 I2C LCD screen
- Jumper wires

## Libraries Used

- `LiquidCrystal_I2C`
- `Wire`
- `WiFi`
- `HTTPClient`
- `ArduinoJson`

## Setup

1. Clone this repository:
```bash
    git clone https://github.com/eftekin/CryptoTracker-ESP32.git
```
2. Install the required libraries via Arduino Library Manager.
3. Create a `secrets.h` file in the root directory:
```cpp
    #define WIFI_SSID "YourSSID"
    #define WIFI_PASSWORD "YourPassword"
    #define API_KEY "YourAPIKey"
```
4. Upload the code to your ESP32 using Arduino IDE.

## JSON File Structure

The coin list is fetched from an online JSON file. Example:
```json
[
    {"code": "BTC", "name": "Bitcoin"},
    {"code": "ETH", "name": "Ethereum"},
    {"code": "SOL", "name": "Solana"}
]
```

## License

This project is licensed under the MIT License.

