#include <SPI.h>
#include <RF24.h>
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// NRF24L01 setup
RF24 radio(9, 10);  // CE, CSN pins
const byte address[6] = "00001";

// DHT11 setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// RTC setup
RTC_DS1307 rtc;

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

struct SensorData {
  float temperature;
  float humidity;
};

SensorData remoteData;
unsigned long lastReceivedTime = 0;
const unsigned long timeoutPeriod = 5000; // 5 seconds

void setup() {
  Serial.begin(9600);
  dht.begin();
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  rtc.adjust(DateTime(2024, 11, 8, 3, 10, 0));  // Comment out after first upload

  // Initialize OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  Serial.println("Receiver setup complete");
}

void loop() {
  // Get local sensor readings
  float localTemp = dht.readTemperature();
  float localHumidity = dht.readHumidity();

  // Get current time
  DateTime now = rtc.now();

  // Check for incoming data
  if (radio.available()) {
    radio.read(&remoteData, sizeof(remoteData));
    lastReceivedTime = millis();
    Serial.println("Received data from transmitter");
  } else {
    Serial.println("No data received from transmitter");
  }

  // Print to Serial Monitor
  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  if (now.minute() < 10) Serial.print("0");
  Serial.println(now.minute());

  Serial.print("Local Temp: ");
  Serial.print(localTemp);
  Serial.print(" °C, Hum: ");
  Serial.print(localHumidity);
  Serial.println(" %");

  Serial.print("Remote Temp: ");
  Serial.print(remoteData.temperature);
  Serial.print(" °C, Hum: ");
  Serial.print(remoteData.humidity);
  Serial.println(" %");

  // Display on OLED
  display.clearDisplay();

  // Display Current Time
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Time: ");
  display.print(now.hour());
  display.print(":");
  if (now.minute() < 10) display.print("0");
  display.print(now.minute());

  // Display Local Sensor Data
  display.setCursor(0, 10);
  display.print("Loc Temp: ");
  display.print(localTemp - 2);
  display.print("C");

  display.setCursor(0, 20);
  display.print("Loc Hum: ");
  display.print(localHumidity);
  display.print("%");

  // Check if remote data has been received recently
  if (millis() - lastReceivedTime < timeoutPeriod) {
    // Display Remote Sensor Data
    display.setCursor(0, 30);
    display.print("Rem Temp: ");
    display.print(remoteData.temperature - 2);
    display.print("C");

    display.setCursor(0, 40);
    display.print("Rem Hum: ");
    display.print(remoteData.humidity);
    display.print("%");
  } else {
    // Display "Not Detected"
    display.setCursor(0, 30);
    display.print("Rem Temp: Not Detected");

    display.setCursor(0, 40);
    display.print("Rem Hum: Not Detected");
  }

  display.display(); // Refresh the OLED display
  delay(1000); // Update every second
}
