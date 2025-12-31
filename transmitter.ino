#include <SPI.h>
#include <RF24.h>
#include <DHT.h>

// NRF24L01 setup
RF24 radio(9, 10);  // CE, CSN pins

// Address for communication
const byte address[6] = "00001";

// DHT11 setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

struct SensorData {
  float temperature;
  float humidity;
};

void setup() {
  Serial.begin(9600);

  // Initialize DHT11 with a longer delay for stabilization
  dht.begin();
  delay(5000); // Increased delay for sensor stabilization

  // Initialize NRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  Serial.println("Transmitter setup complete");
}

void loop() {
  SensorData data;

  // Attempt to read from DHT sensor
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();

  if (!isnan(data.temperature) && !isnan(data.humidity)) {
    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(data.humidity);
    Serial.println(" %");

    // Delay before starting NRF transmission
    delay(1000);

    // Send data to receiver
    bool success = radio.write(&data, sizeof(data));
    if (success) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Failed to send data");
    }
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(3000);  // Longer delay between loops to prevent conflicts
}
