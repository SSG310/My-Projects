/* ESP32_MAIN.ino
   Smart Vehicle Controller — All features included:
   - Motor control (L298) with PWM
   - Manual web UI (forward/back/left/right/stop)
   - Drowsiness endpoints: /drowsy_on, /drowsy_off
   - Traffic sign endpoints: /sign_speed30, /sign_speed50, /sign_speed80, /sign_stop
   - Traffic light endpoints: /red_light, /green_light
   - MPU6050 accident detection -> sends HTTP to ALERT_SERVER_URL
   - Serial debugging prints
*/

// ====== CONFIG - change these ======
const char* WIFI_SSID = "Sourabh";
const char* WIFI_PASS = "123456789";

// Flask alert server (Laptop) - change to your Flask server IP:PORT
const char* ALERT_SERVER_URL = "http://10.64.179.218:5000/alert";
// ===================================

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <math.h>

WebServer server(80);
MPU6050 mpu(Wire);

// Motor pins (L298)
#define IN1  13
#define IN2  12
#define IN3  14
#define IN4  27
#define ENA  32
#define ENB  33

// PWM channels and settings
const int CH_A = 0, CH_B = 1;
const int PWM_FREQ = 1000;
const int PWM_RES = 8;

bool drowsyDetected = false;
bool accidentDetected = false;
float ALERT_THRESHOLD = 45.0;  // degrees to trigger accident
float RESET_THRESHOLD = 40.0;  // degrees to clear

// ---------- Motor helpers ----------
void setSpeed(uint8_t a, uint8_t b) {
  ledcWrite(CH_A, a);
  ledcWrite(CH_B, b);
  Serial.printf("⚙ Speed A=%d B=%d\n", a, b);
}
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  setSpeed(0,0);
  Serial.println("🛑 Motors stopped");
}
void moveForward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  setSpeed(200,200);
  Serial.println("⬆ Forward");
}
void moveBackward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  setSpeed(200,200);
  Serial.println("⬇ Backward");
}
void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  setSpeed(200,200);
  Serial.println("⬅ Left");
}
void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  setSpeed(200,200);
  Serial.println("➡ Right");
}

// ---------- send accident alert to Flask server ----------
void sendAccidentAlert() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi not connected - cannot send alert");
    return;
  }
  HTTPClient http;
  http.begin(ALERT_SERVER_URL);
  int code = http.GET();
  Serial.printf("🚨 Alert sent -> HTTP %d\n", code);
  http.end();
}

// ---------- Web UI handler ----------
void handleRoot() {
  String html = R"rawliteral(
  <!doctype html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
  <title>Smart Vehicle Control</title>
  <style>body{font-family:sans-serif;text-align:center;background:#222;color:#eee}
  button{width:90px;height:60px;margin:8px;font-size:18px;border-radius:8px;border:none}
  .stop{background:#e74c3c;color:#fff}</style>
  </head><body><h2>🚗 Smart Vehicle</h2>
  <div>
    <a href="/forward"><button>⬆ Forward</button></a><br>
    <a href="/left"><button>⬅ Left</button></a>
    <a href="/stop"><button class="stop">⏹ Stop</button></a>
    <a href="/right"><button>➡ Right</button></a><br>
    <a href="/backward"><button>⬇ Back</button></a>
  </div>
  <p>Use ML / Traffic Light to control automatically too.</p>
  </body></html>
  )rawliteral";
  server.send(200, "text/html", html);
}

// ---------- setup routes ----------
void setupRoutes() {
  server.on("/", handleRoot);

  // Manual controls (blocked if drowsyDetected)
  server.on("/forward", [](){ if(!drowsyDetected) moveForward(); server.sendHeader("Location","/"); server.send(303);});
  server.on("/backward", [](){ if(!drowsyDetected) moveBackward(); server.sendHeader("Location","/"); server.send(303);});
  server.on("/left", [](){ if(!drowsyDetected) turnLeft(); server.sendHeader("Location","/"); server.send(303);});
  server.on("/right", [](){ if(!drowsyDetected) turnRight(); server.sendHeader("Location","/"); server.send(303);});
  server.on("/stop", [](){ stopMotors(); server.sendHeader("Location","/"); server.send(303);});

  // Drowsiness endpoints (from Python)
  server.on("/drowsy_on", [](){ drowsyDetected = true; stopMotors(); server.send(200,"text/plain","DROWSY_ON"); Serial.println("😴 Drowsy ON - motors stopped");});
  server.on("/drowsy_off", [](){ drowsyDetected = false; setSpeed(200,200); server.send(200,"text/plain","DROWSY_OFF"); Serial.println("✅ Drowsy OFF - motors active");});

  // Traffic sign endpoints (from Python)
  server.on("/sign_speed30", [](){ setSpeed(80,80); server.send(200,"text/plain","SPEED_30"); Serial.println("Speed limit 30");});
  server.on("/sign_speed50", [](){ setSpeed(150,150); server.send(200,"text/plain","SPEED_50"); Serial.println("Speed limit 50");});
  server.on("/sign_speed80", [](){ setSpeed(255,255); server.send(200,"text/plain","SPEED_80"); Serial.println("Speed limit 80");});
  server.on("/sign_stop", [](){ stopMotors(); server.send(200,"text/plain","SIGN_STOP"); Serial.println("STOP sign");});

  // Traffic light endpoints (from traffic ESP32)
  server.on("/red_light", [](){ stopMotors(); server.send(200,"text/plain","RED_LIGHT"); Serial.println("🔴 Red light - stopped");});
  server.on("/green_light", [](){ setSpeed(200,200); server.send(200,"text/plain","GREEN_LIGHT"); Serial.println("🟢 Green light - running");});
}

// ---------- WiFi connect ----------
void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(300); Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("✅ WiFi connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("⚠ WiFi connect failed - will retry.");
  }
}

// ---------- setup ----------
void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n=== ESP32 MAIN: Smart Vehicle ===");

  // motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // PWM setup
  ledcSetup(CH_A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA, CH_A);
  ledcAttachPin(ENB, CH_B);
  stopMotors();

  // MPU6050 init on I2C pins 21 (SDA) / 22 (SCL)
  Wire.begin(21, 22);
  mpu.begin();
  mpu.calcGyroOffsets(true);
  Serial.println("MPU6050 initialized");

  // connect wifi and start server
  connectWifi();
  setupRoutes();
  server.begin();
  Serial.println("🌐 Web server started");
}

// ---------- loop ----------
void loop() {
  server.handleClient();

  // MPU accident detection (tilt from accelerometer)
  mpu.update();
  float ax = mpu.getAccX(), ay = mpu.getAccY(), az = mpu.getAccZ();
  float tiltX = atan2(ay, sqrt(ax*ax + az*az)) * 180.0 / M_PI;
  float tiltY = atan2(ax, sqrt(ay*ay + az*az)) * 180.0 / M_PI;
  float maxTilt = max(fabs(tiltX), fabs(tiltY));

  Serial.printf("TiltX: %.2f TiltY: %.2f\n", tiltX, tiltY);

  if (!accidentDetected && maxTilt >= ALERT_THRESHOLD) {
    accidentDetected = true;
    Serial.println("🚨 Accident detected -> sending alert to Flask server");
    sendAccidentAlert();
  }

  if (accidentDetected && maxTilt <= RESET_THRESHOLD) {
    accidentDetected = false;
    Serial.println("✅ Accident cleared (tilt back to normal)");
  }

  delay(150);
}
