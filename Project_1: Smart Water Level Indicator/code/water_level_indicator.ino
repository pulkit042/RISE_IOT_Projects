#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin connections
const int trigPin = 12;
const int echoPin = 13;
const int buzzerPin = A0; 
const int tankHeight = 400;  // I have taken Tank Height = 400cm

const int ledPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};   // LED bar graph pins config.

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Water Level Meter ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // echo duration calc.
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  // Calculation of water level (cm)
  int waterLevel = tankHeight - distance;
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > tankHeight) waterLevel = tankHeight;

  // Display water level in cm
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(waterLevel);
  lcd.print(" cm   ");

  // Display progress bar
  int barLength = map(waterLevel, 0, tankHeight, 0, 16);
  lcd.setCursor(0, 1);
  for (int i = 0; i < barLength; i++) {
    lcd.write(byte(255)); // Full block character
  }
  for (int i = barLength; i < 16; i++) {
    lcd.print(" ");
  }

  // LED bar graph display
  int activeLeds = map(waterLevel, 0, tankHeight, 0, 10);
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], (i < activeLeds) ? HIGH : LOW);
  }

  if (waterLevel >= tankHeight - 4) {       // Buzzer triggers 4cm before full
  tone(buzzerPin, 1000); // 1kHz tone
} else {
  noTone(buzzerPin);
}

  delay(1000);
}
