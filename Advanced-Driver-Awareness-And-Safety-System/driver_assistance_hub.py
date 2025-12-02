# Smart_Driver_Hub.py
# - Drowsiness detection (MediaPipe)
# - Traffic sign detection (YOLOv8 best.pt)
# - Sends commands to main ESP32
# - Flask endpoint /alert for accident alerts -> sends Twilio SMS + voice

import cv2, mediapipe as mp, time, math, threading, requests
from flask import Flask
from twilio.rest import Client
from ultralytics import YOLO
import pyttsx3

# ====== CONFIG - change these ======
ESP32_IP = "http://10.64.179.193"          # main ESP32 (vehicle)
IP_CAM_URL = "http://10.64.179.237:8080/video"  # mobile IP Webcam stream
MODEL_PATH = "best.pt"                     # YOLO model path

# Twilio config - replace with your account
account_sid = "" # Add your Twilio Account SID
auth_token = "" # Add your Twilio Auth Token
twilio_number = ""  # your Twilio phone number
target_number = ""  # your phone number
# ====================================

client = Client(account_sid, auth_token)
app = Flask(__name__)
engine = pyttsx3.init()

def speak(text):
    try:
        engine.say(text)
        engine.runAndWait()
    except Exception as e:
        print("TTS error:", e)

def send_to_esp(cmd):
    try:
        r = requests.get(f"{ESP32_IP}/{cmd}", timeout=0.6)
        print(f"[HTTP] /{cmd} -> {r.status_code}")
    except Exception as e:
        print(f"[ERR] /{cmd}:", e)

# shortcuts
on_drowsy = lambda: send_to_esp("drowsy_on")
off_drowsy = lambda: send_to_esp("drowsy_off")
sign_stop = lambda: send_to_esp("sign_stop")
speed30 = lambda: send_to_esp("sign_speed30")
speed50 = lambda: send_to_esp("sign_speed50")
speed80 = lambda: send_to_esp("sign_speed80")

# -------- Flask endpoint to receive accident alerts -----------
@app.route("/alert")
def alert():
    print("🚨 Received /alert (accident) from ESP32")
    threading.Thread(target=lambda: speak("Accident detected. Sending SMS."), daemon=True).start()
    # send Twilio SMS with fixed coordinates (or change later to dynamic)
    lat, lon = 15.820556, 74.498252
    loc_url = f"https://www.google.com/maps?q={lat},{lon}"
    msg = f"🚨 Accident Detected!\nLocation: {loc_url}"
    try:
        sms = client.messages.create(body=msg, from_=twilio_number, to=target_number)
        print("✅ SMS sent. SID:", sms.sid)
        return "SMS sent", 200
    except Exception as e:
        print("❌ Twilio send error:", e)
        return "Error", 500

# run Flask in background
def run_flask():
    app.run(host="0.0.0.0", port=5000)

# -------- Drowsiness setup (MediaPipe) ----------
mp_face = mp.solutions.face_mesh.FaceMesh()
L_EYE = [33,160,158,133,153,144]
R_EYE = [362,385,387,263,373,380]
EAR_THR = 0.22
SLEEP_T = 3.0

def ear(landmarks, pts, w, h):
    xy = [(int(landmarks[i].x*w), int(landmarks[i].y*h)) for i in pts]
    A = math.dist(xy[1], xy[5]); B = math.dist(xy[2], xy[4]); C = math.dist(xy[0], xy[3])
    return (A + B) / (2.0 * C)

# -------- Traffic sign setup (YOLO) ----------
model = YOLO(MODEL_PATH)
SPEAK_DELAY = 3

# -------- Video capture threads ----------
d_frame = None; t_frame = None; running = True

def cap_driver_cam():
    global d_frame
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("❌ Cannot open webcam")
        return
    while running:
        ret, frame = cap.read()
        if ret:
            d_frame = frame
    cap.release()

def cap_traffic_cam():
    global t_frame
    cap = cv2.VideoCapture(IP_CAM_URL)
    if not cap.isOpened():
        print("❌ Cannot open IP cam:", IP_CAM_URL)
        return
    while running:
        ret, frame = cap.read()
        if ret:
            t_frame = frame
    cap.release()

# Start Flask
threading.Thread(target=run_flask, daemon=True).start()
# Start capture threads
threading.Thread(target=cap_driver_cam, daemon=True).start()
threading.Thread(target=cap_traffic_cam, daemon=True).start()

# -------- Main loop ----------
sleep_start = None
alerting = False
last_label = ""
last_time = 0

try:
    while True:
        # DRIVER (drowsiness)
        if d_frame is not None:
            img = d_frame.copy()
            h, w = img.shape[:2]
            res = mp_face.process(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
            if res.multi_face_landmarks:
                lm = res.multi_face_landmarks[0].landmark
                a = (ear(lm, L_EYE, w, h) + ear(lm, R_EYE, w, h)) / 2.0
                if a < EAR_THR:
                    if sleep_start is None:
                        sleep_start = time.time()
                    elif time.time() - sleep_start >= SLEEP_T:
                        cv2.putText(img, "DROWSY", (30,60), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,0,255), 3)
                        if not alerting:
                            threading.Thread(target=lambda: speak("Wake up!"), daemon=True).start()
                            on_drowsy()
                            alerting = True
                else:
                    sleep_start = None
                    if alerting:
                        off_drowsy()
                        alerting = False
            cv2.imshow("Drowsiness", img)

        # TRAFFIC
        if t_frame is not None:
            tf = cv2.resize(t_frame, (640,480))
            results = model.predict(tf, conf=0.5, verbose=False)
            for r in results:
                for b in r.boxes:
                    cls = int(b.cls[0]); lbl = model.names[cls]
                    x1,y1,x2,y2 = map(int, b.xyxy[0].tolist())
                    cv2.rectangle(tf, (x1,y1), (x2,y2), (0,255,0), 2)
                    cv2.putText(tf, lbl, (x1, y1-10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,255,0), 2)
                    now = time.time()
                    if lbl != last_label or now - last_time > SPEAK_DELAY:
                        threading.Thread(target=lambda t=lbl: speak(t), daemon=True).start()
                        last_label, last_time = lbl, now
                        L = lbl.lower()
                        if "stop" in L: sign_stop()
                        elif "30" in L: speed30()
                        elif "50" in L: speed50()
                        elif "80" in L: speed80()
            cv2.imshow("Traffic Sign Detection", tf)

        if cv2.waitKey(1) == 27:
            running = False
            break

except KeyboardInterrupt:
    running = False

cv2.destroyAllWindows()
print("Exiting Smart Driver Hub")
