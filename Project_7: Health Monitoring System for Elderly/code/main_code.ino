#define BLYNK_TEMPLATE_ID "TMPL3TpiFkhEw"
#define BLYNK_TEMPLATE_NAME "Elderly Health Monitor"
#define BLYNK_AUTH_TOKEN "7ommo25WKEfZ_EM6GRiT9lE7COl-InKN"

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// DHT22
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Pulse Sensor (connected to analog pin)
#define PULSE_PIN 34

// MPU6050
Adafruit_MPU6050 mpu;

// WiFi & Blynk Auth
char auth[] = "7ommo25WKEfZ_EM6GRiT9lE7COl-InKN";
char ssid[] = "YourWiFiName";
char pass[] = "YourWiFiPassword";

// Variables for storing values
float temperature, humidity;
int pulseValue;
bool fallDetected = false;

void setup() {
  Serial.begin(115200);
  
  Blynk.begin(auth, ssid, pass);
  
  dht.begin();
  
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(PULSE_PIN, INPUT);
}

void loop() {
  Blynk.run();

  // Read temperature & humidity
  temperature = dht.readTemperature(true); // Read in Fahrenheit
  humidity = dht.readHumidity();

  // Read pulse sensor value
  pulseValue = analogRead(PULSE_PIN);

  // Detect fall
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;
  float totalAcceleration = sqrt(ax * ax + ay * ay + az * az);
  fallDetected = totalAcceleration > 25.0;  // Arbitrary fall threshold

  // Determine alarm messages
  String tempStatus = temperature > 100.0 ? "Fever Detected!" : "Normal Temp";
  String pulseStatus = "Normal Pulse";
  if (pulseValue > 100) {
    pulseStatus = "Tachycardia (High Pulse)";
  } else if (pulseValue < 60) {
    pulseStatus = "Bradycardia (Low Pulse)";
  }

  // Print for debug
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print(" F, Humidity: "); Serial.print(humidity);
  Serial.print(" %, Pulse: "); Serial.print(pulseValue);
  Serial.print(", Acc: "); Serial.println(totalAcceleration);

  // Send to Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, pulseValue);
  Blynk.virtualWrite(V3, fallDetected ? "FALL DETECTED!" : "Normal");
  Blynk.virtualWrite(V4, tempStatus);
  Blynk.virtualWrite(V5, pulseStatus);

  delay(1000);
}
