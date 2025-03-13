#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <map>
#include "secrets.h" // Include sensitive data

#define SDA_PIN 32
#define SCL_PIN 33

LiquidCrystal_I2C lcd(0x27, 16, 2);

const String apiUrl = "https://api.livecoinwatch.com/coins/map";
const String coinsJsonUrl = "https://raw.githubusercontent.com/eftekin/CryptoTracker-ESP32/refs/heads/main/coins.json";

struct CoinInfo {
    String name;
    float rate;
    bool isValid;
};

std::vector<String> coinList;
unsigned int currentCoinIndex = 0;
std::map<String, CoinInfo> coinData;

unsigned long lastFetchTime = 0;
unsigned long lastSwitchTime = 0;
const unsigned long FETCH_INTERVAL = 9000;  // 9 seconds
const unsigned long SWITCH_INTERVAL = 5000; // 5 seconds

bool needsDisplay = true;

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    lcd.init();
    lcd.backlight();

    lcd.clear();
    lcd.print("Connecting...");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected!");
    lcd.clear();
    lcd.print("Connected!");
    delay(1000);

    fetchCoinList();
}

void fetchCoinList() {
    lcd.clear();
    lcd.print("Fetching coins...");
    
    HTTPClient http;
    http.begin(coinsJsonUrl);
    int httpCode = http.GET();
    
    Serial.print("Coin list fetch response: ");
    Serial.println(httpCode);

    if (httpCode > 0) {
        String payload = http.getString();
        Serial.println("Payload: " + payload);
        
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, payload);
        
        if (error) {
            Serial.print("JSON Parse failed: ");
            Serial.println(error.c_str());
            lcd.clear();
            lcd.print("JSON error!");
            return;
        }

        JsonArray array = doc.as<JsonArray>();
        if (array.size() == 0) {
            Serial.println("Empty coin list!");
            lcd.clear();
            lcd.print("No coins found!");
            return;
        }

        for (JsonVariant coin : array) {
            String coinCode = coin.as<String>();
            coinList.push_back(coinCode);
            // Initialize coin data with default values
            coinData[coinCode] = {"Loading...", 0.0, false};
        }
        
        Serial.print("Loaded coins: ");
        Serial.println(coinList.size());
    } else {
        Serial.println("Failed to fetch coin list");
        lcd.clear();
        lcd.print("Fetch failed!");
        delay(2000);
    }

    http.end();
}

void fetchCoinData() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected!");
        lcd.clear();
        lcd.print("WiFi Error!");
        return;
    }

    HTTPClient http;
    http.begin(apiUrl);
    http.addHeader("x-api-key", API_KEY);
    http.addHeader("Content-Type", "application/json");

    // Construct JSON array of coin codes
    String coinsArray = "[";
    for (size_t i = 0; i < coinList.size(); i++) {
        coinsArray += "\"" + coinList[i] + "\"";
        if (i < coinList.size() - 1) coinsArray += ",";
    }
    coinsArray += "]";

    String jsonBody = "{\"currency\":\"USD\",\"codes\":" + coinsArray + 
                     ",\"sort\":\"rank\",\"order\":\"ascending\",\"offset\":0,\"limit\":0,\"meta\":true}";
    
    int httpCode = http.POST(jsonBody);
    Serial.print("API Response Code: ");
    Serial.println(httpCode);

    if (httpCode > 0) {
        String payload = http.getString();
        DynamicJsonDocument doc(16384); // Buffer size increased for safety
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            JsonArray array = doc.as<JsonArray>();
            int updatedCoins = 0;
            for (JsonVariant coin : array) {
                String code = coin["code"].as<String>();
                float price = coin["rate"].as<float>();
                String name = coin["name"].as<String>();

                if (coinData.find(code) != coinData.end() && coinData[code].rate != price) {
                    coinData[code] = {name, price, true};
                    updatedCoins++;
                    needsDisplay = true;
                }
            }
            Serial.printf("Updated %d coins\n", updatedCoins);
        } else {
            Serial.print("JSON Parse Error: ");
            Serial.println(error.c_str());
            lcd.clear();
            lcd.print("Parse Error!");
        }
    } else {
        Serial.println("API Error!");
        lcd.clear();
        lcd.print("API Error!");
    }

    http.end();
}

void updateDisplay() {
    if (coinList.empty()) return;

    const String& currentCoin = coinList[currentCoinIndex];
    CoinInfo& info = coinData[currentCoin];

    if (info.isValid) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(info.name);
        lcd.setCursor(0, 1);
        lcd.print("$");
        lcd.print(info.rate, 3);
    }

    needsDisplay = false;
}

void loop() {
    unsigned long currentTime = millis();

    // Add check for empty coin list
    if (coinList.empty()) {
        if (currentTime - lastFetchTime >= FETCH_INTERVAL) {
            Serial.println("Retrying coin list fetch...");
            fetchCoinList();
            lastFetchTime = currentTime;
        }
        delay(1000);
        return;
    }

    if (currentTime - lastFetchTime >= FETCH_INTERVAL) {
        fetchCoinData();
        lastFetchTime = currentTime;
    }

    if (currentTime - lastSwitchTime >= SWITCH_INTERVAL) {
        currentCoinIndex = (currentCoinIndex + 1) % coinList.size();
        lastSwitchTime = currentTime;
        needsDisplay = true;
    }

    if (needsDisplay) {
        updateDisplay();
    }

    delay(50);  // Reduced from 100ms to 50ms for smoother transitions
}
