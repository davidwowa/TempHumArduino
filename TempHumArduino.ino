#include "Arduino.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// defines for DHT22 humidity and temperature sensor
#define DHTPIN 2
#define DHTTYPE DHT22

// constructor for lcd display with i2c driver
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// constructor for DHT22 sensor
DHT dht(DHTPIN, DHTTYPE);

// photoresistor
int light_pin = 0;

// time adjustment for loop and setup duration
int loop_delay = 1000;
int init_delay = 1000;

// measurement interval
int measurement_time = 1800; // half hour

// time counter which achieve measurement interval
int time_counter = 0;

// counter for arrays
int counter = 0;

// array size for temperature, humidity and light
int array_size = 48; // for 24 hours because measurement try every half hour

// arrays
int lights[48];
int temps[48];
int hums[48];

// medians
int median_light = -1;
int median_temperature = -1;
int median_humidity = -1;

// init array with default values (in this case is -100 (because light can't be -100, temperature its'n not probably and humidity must be over 0))
void init_array(int array[]) {
	for (int var = 0; var < array_size; ++var) {
		array[var] = -100;
	}
}

// median calculation
int calculate_median(int array[]) {
	int sum = 0;
	int counter = 0;
	for (int var = 0; var < array_size; ++var) {
		if (array[var] != -100) {
			sum = sum + array[var];
			++counter;
		}
	}
	// divide by zero :-))
	if (counter != 0) {
		return sum / counter;
	}
	return -1;
}

void add_patterrn(void) {
	/*
	 * display pattern
	 *
	 * LLL|TT|HH|CCCCEE
	 * LLL|TT|HH|EEEEEE
	 * L - light
	 * T - temperature
	 * H - humidity
	 * C - counter until is time for measurement achieve
	 * E - not used (empty)
	 */
	lcd.setCursor(3, 0);
	lcd.print("|");

	lcd.setCursor(6, 0);
	lcd.print("|");

	lcd.setCursor(9, 0);
	lcd.print("|");

	lcd.setCursor(3, 1);
	lcd.print("|");

	lcd.setCursor(6, 1);
	lcd.print("|");

	lcd.setCursor(9, 1);
	lcd.print("|");
}

void setup() {
//	Serial.begin(115200);

	lcd.init();
	lcd.begin(16, 2);
	lcd.backlight();

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("THL - starting");

	lcd.setCursor(0, 1);
	lcd.print("init DHT22");
	dht.begin();
	delay(init_delay);

	lcd.setCursor(0, 1);
	lcd.print("init arrays...");

	delay(init_delay);
	lcd.setCursor(0, 1);
	lcd.print("init tmp. arr");
	init_array(temps);

	delay(init_delay);
	lcd.setCursor(0, 1);
	lcd.print("init hum. arr");
	init_array(hums);

	delay(init_delay);
	lcd.setCursor(0, 1);
	lcd.print("init lgh. arr");
	init_array(lights);
}

void loop() {

	// display clear every mod4 seconds
	if (time_counter == measurement_time || time_counter == 0
			|| time_counter % 4 == 0) {
		lcd.clear();
	}

	// read light power
	int light_power = analogRead(light_pin);
	lcd.setCursor(0, 0);
	lcd.print(light_power);

	// if time for measuremet is achieve, calculate median
	if (time_counter == measurement_time || time_counter == 0) {
		lights[counter] = light_power;
		median_light = calculate_median(lights);
	}
	lcd.setCursor(0, 1);
	lcd.print(median_light);

	// dht.readTemperature();
	int temperature = random(-10, 99);
	lcd.setCursor(4, 0);
	lcd.print(temperature);

	if (time_counter == measurement_time || time_counter == 0) {
		temps[counter] = temperature;
		median_temperature = calculate_median(temps);
	}
	lcd.setCursor(4, 1);
	lcd.print(median_temperature);

	//	dht.readHumidity();
	int humidity = random(0, 99);
	lcd.setCursor(7, 0);
	lcd.print(humidity);

	if (time_counter == measurement_time || time_counter == 0) {
		hums[counter] = humidity;
		median_humidity = calculate_median(hums);
	}
	lcd.setCursor(7, 1);
	lcd.print(median_humidity);

	lcd.setCursor(10, 0);
	lcd.print(measurement_time - time_counter);

	// set array counter on zero and new begin
	if (counter < array_size) {
		++counter;
	} else {
		counter = 0;
	}

	// set timer count on zero and new begin
	if (time_counter == measurement_time) {
		time_counter = 0;
	} else {
		++time_counter;
	}

	add_patterrn();

	delay(loop_delay);
}
