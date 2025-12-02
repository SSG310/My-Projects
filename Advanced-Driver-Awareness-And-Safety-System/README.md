# 🚗 Advanced Driver Awareness and Safety System
A Smart Vehicle Controller integrating ADAS features such as drowsiness detection, traffic sign recognition, speed control, V2I traffic-light communication, and accident alerting.  
This prototype combines **Machine Learning, ESP32, Sensors, and IoT** to enhance vehicle safety.  

---

## 📌 Overview
This project implements a **low-cost ADAS system** capable of:
- Detecting driver drowsiness using EAR (Eye Aspect Ratio)  
- Recognizing Indian traffic signs using YOLOv8  
- Automatically adjusting vehicle speed  
- Responding to simulated traffic lights (V2I)  
- Detecting accidents and sending SMS alerts  

The system uses a **Python Smart Driver Hub** for ML tasks and **two ESP32 microcontrollers** for real-time actuation and communication.  

---

## ⭐ Features

### 🔹 1. Drowsiness Detection  
- EAR calculated using MediaPipe Face Mesh  
- EAR < 0.22 for more than 3 seconds = Drowsy  
- Triggers voice alert and sends STOP command  

### 🔹 2. Traffic Sign Recognition (YOLOv8)  
- YOLOv8-nano trained on Indian Road Traffic Sign Dataset (IRTSD)  
- Detects Stop, No Parking, No Stopping, Speed Limit signs, etc.  
- Achieved **mAP@0.5 = 0.711**  

### 🔹 3. Speed Control Module  
- DC motor controlled via ESP32 + L298N driver  
- PWM-based speed: 33%, 66%, 100%  
- Auto STOP during drowsiness detection  

### 🔹 4. Traffic Light Communication (V2I)  
- Secondary ESP32 simulates red/green light  
- Sends `/red_light` or `/green_light` to vehicle ESP32  
- Vehicle halts/resumes accordingly  

### 🔹 5. Accident Detection + SMS Alert  
- MPU6050 detects tilt > 45°  
- ESP32 → Flask → Twilio → SMS  
- Alert sent within **2–3 seconds**  

---

## 🧱 System Block Diagram
According to **Figure 1 (Page 8)**, the system integrates:
- Smart Driver Hub  
- ESP32 Vehicle Controller  
- ESP32 Traffic Light Module  
- Flask + Twilio  
- Motor Driver + MPU6050  

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

---

## 📊 Results Summary

### ✔ Drowsiness Detection  
- EAR threshold: **0.22**  
- Detected drowsiness after ~3 seconds  
- Voice alert + STOP action performed  

### ✔ Traffic Sign Recognition  
- mAP@0.5 = **0.711**  
- Accurate detection across multiple classes  
- Results verified with confusion matrix & PR curves  

### ✔ Speed Control  
- PWM motor speed: 33% / 66% / 100%  
- Instant STOP on drowsiness  

### ✔ Accident Alert  
- Tilt >45° detected reliably  
- SMS alert sent in **2–3 seconds**  

### ✔ Integrated System  
- Full system latency <100 ms  
- All modules worked together smoothly  

---

## 🚀 Future Improvements
- Lane detection & collision avoidance  
- GPS + GSM-based live tracking  
- Cloud dashboard  
- Android/iOS companion app  
- On-device AI optimization  

---

## 👥 Team
- **Snehanshu Gunjal**
- Hiba Ashekhan   
- Sourabh Pawar  
- Suchitkumar Khadakabhavi   

Guide: **Dr. Arun Tigadi**  
