#include "Arduino.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//0x27
//0x3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);

int lightPin = 0;

int counter = 0;

void setup() {
	Serial.begin(115200);
	lcd.init();
	lcd.backlight();

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("temp_hum");
	lcd.setCursor(0, 1);
	lcd.print("...");

}

void loop() {
	int lightPower = analogRead(lightPin);
	delay(1000);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(counter++);
	lcd.setCursor(0, 1);
	lcd.print("light ");
	lcd.setCursor(8, 1);
	lcd.print(lightPower);
}
