int leds[] = {2, 3, 4, 5, 6};

// LDR
int ldr = 7;

// Ultrasonic
int trig = 9;
int echo = 10;

String input = "";
int fingers[5];

// Smart flags
bool autoOn = false;
bool manualOff = false;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
  }

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ldr, INPUT);
}

void loop() {

  // 📡 Ultrasonic
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  int distance = duration * 0.034 / 2;

  // 🌙 LDR
  int ldrValue = digitalRead(ldr);

  // 👋 Read gesture
  if (Serial.available()) {
    input = Serial.readStringUntil('\n');

    sscanf(input.c_str(), "%d,%d,%d,%d,%d",
           &fingers[0], &fingers[1], &fingers[2],
           &fingers[3], &fingers[4]);
  }

  // ☀️ DAY MODE (gesture control)
  if (ldrValue == LOW) {

    autoOn = false;
    manualOff = false;

    int count = fingers[0] + fingers[1] + fingers[2] + fingers[3] + fingers[4];

    for (int i = 0; i < 5; i++) {
      if (i < count) {
        digitalWrite(leds[i], HIGH);
      } else {
        digitalWrite(leds[i], LOW);
      }
    }
  }

  // 🌙 NIGHT MODE
  else {

    // Auto ON once
    if (!autoOn && !manualOff) {
      for (int i = 0; i < 5; i++) {
        digitalWrite(leds[i], HIGH);
      }
      autoOn = true;
    }

    // Ultrasonic detects person
    if (distance > 0 && distance < 30) {

      // Any finger → turn OFF
      if (fingers[0] || fingers[1] || fingers[2] || fingers[3] || fingers[4]) {
        for (int i = 0; i < 5; i++) {
          digitalWrite(leds[i], LOW);
        }
        manualOff = true;
      }
    }
  }

  delay(20);
}