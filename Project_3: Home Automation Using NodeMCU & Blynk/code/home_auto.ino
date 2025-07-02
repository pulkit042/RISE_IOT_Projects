#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

Adafruit_FT6206 ctp = Adafruit_FT6206();

#define TFT_CS 5
#define TFT_DC 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// LED pins
#define LED1 17   // LIGHT
#define LED2 16  // FAN

// Button dimensions
#define BTN_WIDTH 100
#define BTN_HEIGHT 60

// Button positions
#define BTN1_ON_X 10
#define BTN1_ON_Y 40
#define BTN1_OFF_X 130
#define BTN1_OFF_Y 40

#define BTN2_ON_X 10
#define BTN2_ON_Y 150
#define BTN2_OFF_X 130
#define BTN2_OFF_Y 150

void drawInterface() {
  tft.fillScreen(ILI9341_BLACK);

  // Titles
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(50, 10);
  tft.print("FAN Controls");

  tft.setCursor(50, 120);
  tft.print("LIGHT Controls");

  // Dividers
  tft.drawLine(0, 115, 240, 115, ILI9341_WHITE); // horizontal line

  // Buttons
  tft.fillRect(BTN1_ON_X, BTN1_ON_Y, BTN_WIDTH, BTN_HEIGHT, ILI9341_GREEN);
  tft.setCursor(BTN1_ON_X + 25, BTN1_ON_Y + 20);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.print("ON");

  tft.fillRect(BTN1_OFF_X, BTN1_OFF_Y, BTN_WIDTH, BTN_HEIGHT, ILI9341_RED);
  tft.setCursor(BTN1_OFF_X + 20, BTN1_OFF_Y + 20);
  tft.print("OFF");

  tft.fillRect(BTN2_ON_X, BTN2_ON_Y, BTN_WIDTH, BTN_HEIGHT, ILI9341_GREEN);
  tft.setCursor(BTN2_ON_X + 25, BTN2_ON_Y + 20);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("ON");

  tft.fillRect(BTN2_OFF_X, BTN2_OFF_Y, BTN_WIDTH, BTN_HEIGHT, ILI9341_RED);
  tft.setCursor(BTN2_OFF_X + 20, BTN2_OFF_Y + 20);
  tft.print("OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  tft.begin();
  if (!ctp.begin()) {
    Serial.println("Touchscreen not found");
    while (1);
  }

  tft.setRotation(1);  // Landscape mode
  drawInterface();
}

void loop() {
  if (!ctp.touched()) return;

  TS_Point p = ctp.getPoint();

  // for landscape mode
  int tx = map(p.y, 0, 320, 0, 240); // X-axis 
  int ty = map(p.x, 0, 240, 0, 320); // Y-axis 

  Serial.print("Touch: ");
  Serial.print(tx); Serial.print(", "); Serial.println(ty);

  // LIGHT ON
  if (tx >= BTN1_ON_X && tx <= BTN1_ON_X + BTN_WIDTH && ty >= BTN1_ON_Y && ty <= BTN1_ON_Y + BTN_HEIGHT) {
    digitalWrite(LED1, HIGH);
    Serial.println("LED1 ON");
  }

  // LIGHT OFF
  if (tx >= BTN1_OFF_X && tx <= BTN1_OFF_X + BTN_WIDTH && ty >= BTN1_OFF_Y && ty <= BTN1_OFF_Y + BTN_HEIGHT) {
    digitalWrite(LED1, LOW);
    Serial.println("LED1 OFF");
  }

  // FAN ON
  if (tx >= BTN2_ON_X && tx <= BTN2_ON_X + BTN_WIDTH && ty >= BTN2_ON_Y && ty <= BTN2_ON_Y + BTN_HEIGHT) {
    digitalWrite(LED2, HIGH);
    Serial.println("LED2 ON");
  }

  // FAN OFF
  if (tx >= BTN2_OFF_X && tx <= BTN2_OFF_X + BTN_WIDTH && ty >= BTN2_OFF_Y && ty <= BTN2_OFF_Y + BTN_HEIGHT) {
    digitalWrite(LED2, LOW);
    Serial.println("LED2 OFF");
  }

  delay(300); // Debounce
}

