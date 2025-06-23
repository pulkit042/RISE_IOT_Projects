#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin connections
const int trigPin = 12;
const int echoPin = 13;
const int buzzerPin = A0; 
const int binHeight = 400;  // I have taken Bin Height = 400cm


void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

 lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Garbage Level Meter ");
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
  int waterLevel = binHeight - distance;
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > binHeight) waterLevel = binHeight;

  // Display water level in cm
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(waterLevel);
  lcd.print(" cm   ");

  // Display progress bar
  int barLength = map(waterLevel, 0, binHeight, 0, 16);
  lcd.setCursor(0, 1);
  for (int i = 0; i < barLength; i++) {
    lcd.write(byte(255)); // Full block character
  }
  for (int i = barLength; i < 16; i++) {
    lcd.print(" ");
  }

  // Buzzer triggers 4cm before full
  if (waterLevel >= binHeight - 4) {
  tone(buzzerPin, 1000); // 1kHz tone
} else {
  noTone(buzzerPin);
}

  delay(1000);
}
