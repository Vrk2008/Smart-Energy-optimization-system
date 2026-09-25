#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// -------- PIN DEFINITIONS --------

#define IR_PIN 2
#define DHT_PIN 3

#define LIGHT_LED 7
#define FAN_LED 8

#define DHTTYPE DHT11

// -------- OBJECTS --------

DHT dht(DHT_PIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);


// -------- SETUP --------

void setup() {

  Serial.begin(9600);

  pinMode(IR_PIN, INPUT);

  pinMode(LIGHT_LED, OUTPUT);
  pinMode(FAN_LED, OUTPUT);

  // Initially OFF
  digitalWrite(LIGHT_LED, LOW);
  digitalWrite(FAN_LED, LOW);

  // Start DHT11
  dht.begin();

  // Start LCD
  lcd.init();
  lcd.backlight();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SMART RESOURCE");

  lcd.setCursor(0, 1);
  lcd.print("SAVER SYSTEM");

  delay(2000);

  lcd.clear();
}


// -------- MAIN LOOP --------

void loop() {

  // Read IR sensor
  int person = digitalRead(IR_PIN);

  // Read temperature
  float temperature = dht.readTemperature();


  // -------- DHT11 ERROR --------

  if (isnan(temperature)) {

    digitalWrite(LIGHT_LED, LOW);
    digitalWrite(FAN_LED, LOW);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("DHT11 ERROR");

    lcd.setCursor(0, 1);
    lcd.print("CHECK SENSOR");

    Serial.println("DHT11 ERROR");

    delay(2000);

    return;
  }


  // -------- PERSON DETECTED --------

  // Most IR obstacle sensors:
  // LOW = object detected

  if (person == LOW) {

    // Light LED ON
    digitalWrite(LIGHT_LED, HIGH);


    // Temperature HIGH
    if (temperature > 28.0) {

      // Fan LED ON
      digitalWrite(FAN_LED, HIGH);

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("PERSON: YES");

      lcd.setCursor(0, 1);
      lcd.print("T:");
      lcd.print(temperature, 1);
      lcd.print("C FAN:ON");

      Serial.println("PERSON: YES");

      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");

      Serial.println("LIGHT LED: ON");
      Serial.println("FAN LED: ON");
    }


    // Temperature NORMAL
    else {

      // Fan LED OFF
      digitalWrite(FAN_LED, LOW);

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("PERSON: YES");

      lcd.setCursor(0, 1);
      lcd.print("T:");
      lcd.print(temperature, 1);
      lcd.print("C FAN:OFF");

      Serial.println("PERSON: YES");

      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");

      Serial.println("LIGHT LED: ON");
      Serial.println("FAN LED: OFF");
    }
  }


  // -------- PERSON NOT DETECTED --------

  else {

    digitalWrite(LIGHT_LED, LOW);
    digitalWrite(FAN_LED, LOW);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("PERSON: NO");

    lcd.setCursor(0, 1);
    lcd.print("RESOURCES OFF");

    Serial.println("PERSON: NO");
    Serial.println("LIGHT LED: OFF");
    Serial.println("FAN LED: OFF");
  }


  delay(1500);
}