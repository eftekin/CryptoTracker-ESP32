# CryptoTracker ESP32

CryptoTracker ESP32 is a real-time cryptocurrency price tracker built on an ESP32 microcontroller. It fetches live cryptocurrency prices from the LiveCoinWatch API and displays them on a 16x2 I2C LCD screen.

## Features

- Fetches live cryptocurrency prices from [LiveCoinWatch API](https://www.livecoinwatch.com/tools/api)
- Dynamically loads supported coins from a remote JSON configuration file
- Displays coin names and current prices on a 16x2 I2C LCD screen
- Automatically rotates through different cryptocurrencies at configurable intervals
- Uses `secrets.h` to securely store API keys and Wi-Fi credentials
- Provides error handling for network and API issues

## Hardware Requirements

- ESP32 development board
- 16x2 I2C LCD screen (0x27 address)
- Jumper wires for connections
- USB cable for programming and power

## Wiring Diagram

Connect the I2C LCD to the ESP32 using the following pins:
- SDA: GPIO 32
- SCL: GPIO 33
- VCC: 5V (VIN)
- GND: GND

## Libraries Used

- `LiquidCrystal_I2C`: For controlling the LCD display
- `Wire`: For I2C communication
- `WiFi`: For connecting to the internet
- `HTTPClient`: For making API requests
- `ArduinoJson`: For parsing JSON data
- Standard C++ `map` and `vector` for data management

## Setup Instructions

1. Clone this repository:
   ```bash
   git clone https://github.com/eftekin/CryptoTracker-ESP32.git
   ```

2. Install the required libraries via Arduino Library Manager.

3. Create a `secrets.h` file in the project directory with the following contents:
   ```cpp
   #define WIFI_SSID "YourWiFiName"
   #define WIFI_PASSWORD "YourWiFiPassword"
   #define API_KEY "YourLiveCoinWatchAPIKey"
   ```

4. Obtain an API key from [LiveCoinWatch](https://www.livecoinwatch.com/tools/api).

5. Connect your ESP32 board to your computer and select the appropriate board and port in Arduino IDE.

6. Upload the code to your ESP32.

## Configuration

The device fetches a list of cryptocurrency codes from a remote JSON file. The default location is:
```
https://raw.githubusercontent.com/eftekin/CryptoTracker-ESP32/refs/heads/main/coins.json
```

The JSON file should contain an array of cryptocurrency codes:
```json
[
    "BTC",
    "ETH",
    "BNB",
    "DOGE",
    "SOL"
]
```

## Customization

- `FETCH_INTERVAL`: Time in milliseconds between API calls (default: 9000ms)
- `SWITCH_INTERVAL`: Time in milliseconds between cycling to the next coin (default: 5000ms)
- `SDA_PIN` and `SCL_PIN`: GPIO pins for I2C communication (defaults: 32 and 33)

## Troubleshooting

- If the LCD shows "WiFi Error!", check your WiFi credentials and connection.
- If the LCD shows "API Error!", verify your API key and internet connection.
- If the LCD shows "JSON error!" or "Parse Error!", there may be an issue with the JSON data format.
- If the LCD shows "No coins found!", check that your coins.json file is accessible and properly formatted.

## License

This project is licensed under the MIT License.

## Acknowledgements

- [LiveCoinWatch API](https://www.livecoinwatch.com/tools/api) for cryptocurrency price data
- Contributors and maintainers of the libraries used in this project
