#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- DHT22 Setup ---
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- LCD Setup ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Pins and Constants ---
const int SOIL_MOISTURE_PIN = 34;
const int SPRINKLER_CONTROL_PIN = 19;

int MOISTURE_THRESHOLD_LOW = 15;
int MOISTURE_THRESHOLD_HIGH = 85;
bool SPRINKLER_ACTIVATION_STATUS = false;

// --- WiFi & ThingSpeak ---
char* WIFI_NAME = " ";
char* WIFI_PASSWORD = "";
int myChannelNumber = //write your channel no. ;
char* myApiKey = " ";
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // WiFi setup
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected. IP: " + String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  // LCD & DHT setup
  lcd.init();
  lcd.backlight();
  dht.begin();

  pinMode(SPRINKLER_CONTROL_PIN, OUTPUT);
}

void loop() {
  // Read soil moisture
  int soilMoisturePercentage = map(analogRead(SOIL_MOISTURE_PIN), 0, 4095, 0, 100);

  // Read DHT sensor
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Check for invalid readings
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT22!");
    return;
  }

  // Sprinkler control
  if (soilMoisturePercentage < MOISTURE_THRESHOLD_LOW) {
    SPRINKLER_ACTIVATION_STATUS = true;
    digitalWrite(SPRINKLER_CONTROL_PIN, HIGH);
  } else if (soilMoisturePercentage > MOISTURE_THRESHOLD_HIGH) {
    SPRINKLER_ACTIVATION_STATUS = false;
    digitalWrite(SPRINKLER_CONTROL_PIN, LOW);
  }

  // Print on Serial
  Serial.println("-------------");
  Serial.printf("Soil Moisture: %d%%\n", soilMoisturePercentage);
  Serial.printf("Temperature: %.2f°C, Humidity: %.2f%%\n", temp, hum);
  Serial.printf("Sprinkler: %s\n", SPRINKLER_ACTIVATION_STATUS ? "ON" : "OFF");

  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moist: ");
  lcd.print(soilMoisturePercentage);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Sprink: ");
  lcd.print(SPRINKLER_ACTIVATION_STATUS ? "ON " : "OFF");

  // Send to ThingSpeak
  ThingSpeak.setField(1, soilMoisturePercentage);
  ThingSpeak.setField(2, SPRINKLER_ACTIVATION_STATUS);
  ThingSpeak.setField(3, temp);
  ThingSpeak.setField(4, hum);

  int x = ThingSpeak.writeFields(myChannelNumber, myApiKey);
  if (x == 200) Serial.println("ThingSpeak update success.");
  else Serial.println("Error sending to ThingSpeak: " + String(x));

  delay(4000); 
}
