#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// DHT22 Configuration
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// MQ-2 Gas Sensor (analog)
#define MQ2_PIN 34  

// Output Pins
#define BUZZER_PIN 18
#define RED_LED_PIN 19
#define GREEN_LED_PIN 21

// Thresholds values(Temp - 50 celcius, Gas Constant - 450ppm)
#define TEMP_THRESHOLD 50.0 
#define GAS_THRESHOLD 3650

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // Default: Safe state
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
}

void loop() {
  delay(2000);  // Wait 2sec between reads

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasLevel = analogRead(MQ2_PIN);

  // Check if DHT22 readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Debug print
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.print("% | Gas: ");
  Serial.println(gasLevel);

  // FIRE/GAS ALERT
  if (temperature >= TEMP_THRESHOLD || gasLevel >= GAS_THRESHOLD) {
    tone(BUZZER_PIN, 1000);
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
  } else {
    // Safe state
    noTone(BUZZER_PIN);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
  }
}
