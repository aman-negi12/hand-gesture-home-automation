import serial
import time
import mediapipe as mp
import math
import cv2

# ---------------- SERIAL SETUP ----------------
arduino = serial.Serial('COM7', 9600)  # ⚠️ change COM port if needed
time.sleep(2)

# ---------------- MEDIAPIPE SETUP ----------------
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1)
mp_draw = mp.solutions.drawing_utils

# ---------------- CAMERA SETUP ----------------
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
time.sleep(2)

if not cap.isOpened():
    print("Camera not accessible")
    exit()

# ---------------- SMOOTHING ----------------
prev_speed = 0
alpha = 0.2   # smoothing factor

# ---------------- MAIN LOOP ----------------
while True:
    success, img = cap.read()

    if not success or img is None:
        continue

    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(img_rgb)

    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:
            mp_draw.draw_landmarks(img, handLms, mp_hands.HAND_CONNECTIONS)
            lm_list = []

            h, w, c = img.shape

            for id, lm in enumerate(handLms.landmark):
                lm_list.append((int(lm.x * w), int(lm.y * h)))

            if len(lm_list) >= 9:
                x1, y1 = lm_list[4]   # Thumb tip
                x2, y2 = lm_list[8]   # Index tip

                # Draw visuals
                cv2.circle(img, (x1, y1), 6, (0, 255, 0), -1)
                cv2.circle(img, (x2, y2), 6, (0, 255, 0), -1)
                cv2.line(img, (x1, y1), (x2, y2), (255, 0, 255), 2)

                # Distance
                distance = math.hypot(x2 - x1, y2 - y1)

                # ----------- CALIBRATED INVERTED MAPPING -----------
                min_dist = 20
                max_dist = 200

                distance = max(min_dist, min(max_dist, distance))

                speed = int((1 - (distance - min_dist) / (max_dist - min_dist)) * 255)
                speed = max(0, min(255, speed))

                # ----------- SMOOTHING -----------
                smooth_speed = int(alpha * speed + (1 - alpha) * prev_speed)
                prev_speed = smooth_speed

                # ----------- DEAD ZONE -----------
                if smooth_speed < 20:
                    smooth_speed = 0

                # Send to Arduino
                arduino.write((str(smooth_speed) + '\n').encode())
                time.sleep(0.05)

                # ----------- DISPLAY FIX (CORRECTED) -----------
                display_percent = int((255 - smooth_speed) / 255 * 100) -1

                cv2.putText(img, f"Speed: {display_percent}%", (10, 70),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    # Show output
    cv2.imshow("Gesture Fan Control", img)

    # Exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# ---------------- CLEANUP ----------------
cap.release()
cv2.destroyAllWindows()
arduino.close()