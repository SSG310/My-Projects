# 🚗 Advanced Driver Awareness and Safety System
A Smart Vehicle Controller integrating ADAS features such as drowsiness detection, traffic sign recognition, speed control, V2I traffic-light communication, and accident alerting.  
This prototype combines **Machine Learning, ESP32, Sensors, and IoT** to enhance vehicle safety.  
(Reference: Project Report :contentReference[oaicite:0]{index=0})

---

## 📌 Overview
This project implements a **low-cost ADAS system** capable of:
- Detecting driver drowsiness using EAR (Eye Aspect Ratio)  
- Recognizing Indian traffic signs using YOLOv8  
- Automatically adjusting vehicle speed  
- Responding to simulated traffic lights (V2I)  
- Detecting accidents and sending SMS alerts  

The system uses a **Python Smart Driver Hub** for ML tasks and **two ESP32 microcontrollers** for real-time actuation and communication.  
(Reference: Abstract, Page 1–2 :contentReference[oaicite:1]{index=1})

---

## ⭐ Features

### 🔹 1. Drowsiness Detection  
- EAR calculated using MediaPipe Face Mesh  
- EAR < 0.22 for more than 3 seconds = Drowsy  
- Triggers voice alert and sends STOP command  
(Reference: Section 3.2, Pages 3–4 :contentReference[oaicite:2]{index=2})

### 🔹 2. Traffic Sign Recognition (YOLOv8)  
- YOLOv8-nano trained on Indian Road Traffic Sign Dataset (IRTSD)  
- Detects Stop, No Parking, No Stopping, Speed Limit signs, etc.  
- Achieved **mAP@0.5 = 0.711**  
(Reference: Section 3.3 & Table 1, Pages 5–15 :contentReference[oaicite:3]{index=3})

### 🔹 3. Speed Control Module  
- DC motor controlled via ESP32 + L298N driver  
- PWM-based speed: 33%, 66%, 100%  
- Auto STOP during drowsiness detection  
(Reference: Section 3.4, Page 6–7 :contentReference[oaicite:4]{index=4})

### 🔹 4. Traffic Light Communication (V2I)  
- Secondary ESP32 simulates red/green light  
- Sends `/red_light` or `/green_light` to vehicle ESP32  
- Vehicle halts/resumes accordingly  
(Reference: Section 3.5, Page 7 :contentReference[oaicite:5]{index=5})

### 🔹 5. Accident Detection + SMS Alert  
- MPU6050 detects tilt > 45°  
- ESP32 → Flask → Twilio → SMS  
- Alert sent within **2–3 seconds**  
(Reference: Section 3.6 & 4.5, Pages 7–8, 16 :contentReference[oaicite:6]{index=6})

---

## 🧱 System Block Diagram
According to **Figure 1 (Page 8)**, the system integrates:
- Smart Driver Hub  
- ESP32 Vehicle Controller  
- ESP32 Traffic Light Module  
- Flask + Twilio  
- Motor Driver + MPU6050  
:contentReference[oaicite:7]{index=7}

---

## 🛠 Technology Stack

### Hardware
- ESP32 (main controller)  
- ESP32 (traffic light unit)  
- MPU6050 IMU sensor  
- L298N Motor Driver  
- DC Motors  

### Software
- Python  
- OpenCV  
- MediaPipe  
- YOLOv8 (Ultralytics)  
- Arduino IDE  
- Flask Framework  
- Twilio API  
- pyttsx3  
(Reference: Section 3.8, Page 8–9 :contentReference[oaicite:8]{index=8})


---

## 📊 Results Summary

### ✔ Drowsiness Detection  
- EAR threshold: **0.22**  
- Detected drowsiness after ~3 seconds  
- Voice alert + STOP action performed  
(Reference: Page 10 :contentReference[oaicite:9]{index=9})

### ✔ Traffic Sign Recognition  
- mAP@0.5 = **0.711**  
- Accurate detection across multiple classes  
- Results verified with confusion matrix & PR curves  
(Reference: Pages 11–15 :contentReference[oaicite:10]{index=10})

### ✔ Speed Control  
- PWM motor speed: 33% / 66% / 100%  
- Instant STOP on drowsiness  
(Reference: Page 15 :contentReference[oaicite:11]{index=11})

### ✔ Accident Alert  
- Tilt >45° detected reliably  
- SMS alert sent in **2–3 seconds**  
(Reference: Page 16 :contentReference[oaicite:12]{index=12})

### ✔ Integrated System  
- Full system latency <100 ms  
- All modules worked together smoothly  
(Reference: Page 17 :contentReference[oaicite:13]{index=13})

---

## 🚀 Future Improvements
- Lane detection & collision avoidance  
- GPS + GSM-based live tracking  
- Cloud dashboard  
- Android/iOS companion app  
- On-device AI optimization  
(Reference: Section 6, Page 18 :contentReference[oaicite:14]{index=14})

---

## 👥 Team
- **Snehanshu Gunjal (02FE22BEC103)**  
- Sourabh Pawar  
- Suchitkumar Khadakabhavi  
- Hiba Ashekhan  

Guide: **Dr. Arun Tigadi**  
(Reference: Page 1–2 :contentReference[oaicite:15]{index=15})
