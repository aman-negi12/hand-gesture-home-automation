# Hand Gesture-Based Smart Home Automation

A contactless smart home automation system combining computer vision with Arduino-based embedded control for gesture-driven fan and light operation.

## System Overview

### Fan Control
- Python + OpenCV + MediaPipe captures the hand and tracks landmarks.
- Thumb-index finger distance is converted to a 0–255 motor-speed value.
- The value is smoothed and sent to Arduino over serial.
- Arduino applies PWM through an L298N motor driver and reads DHT11 temperature/humidity for LCD monitoring.

### Light Control
- Python + MediaPipe detects five finger states and sends them to Arduino over serial.
- Arduino controls five LEDs and uses an LDR for day/night logic.
- An ultrasonic sensor provides proximity detection during the night mode.

## Tech Stack

Python, C++, OpenCV, MediaPipe, Arduino Uno, L298N, DHT11, LDR, Ultrasonic Sensor, LiquidCrystal_I2C

## Repository Structure

```text
hand-gesture-home-automation/
├── fan-control/
│   ├── gesture_fan_control.py
│   └── fan_controller.ino
├── light-control/
│   ├── gesture_light_control.py
│   └── light_controller.ino
├── images/
├── docs/
├── requirements.txt
├── .gitignore
└── README.md
```

## Fan Workflow

```text
Camera → MediaPipe → Thumb/Index Distance → Python Speed Mapping → Serial → Arduino → PWM → L298N → 12V Fan
```

## Light Workflow

```text
Camera → MediaPipe → Finger States → Serial → Arduino → 5 LEDs
                                  ↘ LDR + Ultrasonic → Night/Proximity Logic
```

## Light Gesture Mapping

The Python controller sends five binary finger states. The Arduino counts the active fingers during daytime.

```text
1 finger → LED 1
2 fingers → LED 1 + LED 2
3 fingers → LED 1 + LED 2 + LED 3
4 fingers → first 4 LEDs
5 fingers → all 5 LEDs
```

## Setup

1. Install Python dependencies:

```bash
pip install -r requirements.txt
```

2. Update the COM port in the Python script to the Arduino port used on your PC.
3. Open the matching `.ino` file in Arduino IDE and upload it to the corresponding Arduino Uno.
4. Run the required controller:

```bash
python fan-control/gesture_fan_control.py
```

or

```bash
python light-control/gesture_light_control.py
```

## Notes

- Keep each Python controller paired with its corresponding Arduino sketch.
- Add your prototype/circuit images to `images/` and project documentation to `docs/` when publishing the repo.
