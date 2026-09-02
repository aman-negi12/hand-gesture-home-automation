#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHTPIN A3
#define DHTTYPE DHT11
#define IN1 9
#define IN2 8
#define ENA 10

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

float temp;
float hum;
int motorSpeed = 0;
int lastSpeed = -1;
String inputStr = "";

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Gesture Fan Ctrl");
  delay(2000);
  lcd.clear();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      motorSpeed = constrain(inputStr.toInt(), 0, 255);
      inputStr = "";
    } else {
      inputStr += c;
    }
  }

  if (motorSpeed != lastSpeed) {
    analogWrite(ENA, motorSpeed);
    lastSpeed = motorSpeed;
  }

  temp = dht.readTemperature();
  hum = dht.readHumidity();

  if (!isnan(temp)) Serial.println(temp);

  if (!isnan(temp) && !isnan(hum)) {
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.print("C H:");
    lcd.print((int)hum);
    lcd.print("%   ");

    lcd.setCursor(0, 1);
    if (temp < 0) lcd.print("COLD! Fan OFF   ");
    else if (temp > 40) lcd.print("HOT!  Fan MAX   ");
    else {
      lcd.print("Spd:");
      lcd.print(motorSpeed);
      lcd.print("        ");
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
  delay(100);
}
