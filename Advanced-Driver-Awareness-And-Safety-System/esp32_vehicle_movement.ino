#include <Servo.h>

// Define relay pins for motor control
#define relay1 4  // Relay for Motor 1 forward
#define relay2 5  // Relay for Motor 1 backward
#define relay3 6  // Relay for Motor 2 forward
#define relay4 7  // Relay for Motor 2 backward

// Define the pin for the servo motor
#define servoPin 9

// Variables for Bluetooth commands
char command = ' ';

// Initialize the Servo object
Servo myServo;

void setup() {
  // Initialize relay pins as OUTPUT
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  // Initialize servo motor
  myServo.attach(servoPin);
  myServo.write(0); // Start at 0 degrees

  // Ensure all relays are OFF initially
  Stop();

  // Initialize serial communication
  Serial.begin(9600);    // For debugging via USB
  Serial1.begin(9600);   // Bluetooth communication on Serial1 (Pins 18, 19 on Mega)
  delay(500);
}

void loop() {
  // Read the incoming data from the Smartphone Android App
  if (Serial1.available() > 0) {
    command = Serial1.read();  // Read the Bluetooth command
    Serial.print("Received command: ");
    Serial.println(command);

    // Process command based on button presses
    switch (command) {
      case 'F':  // Move forward
        forward();
        break;
      case 'B':  // Move backward
        backward();
        break;
      case 'L':  // Turn left
        turnLeft();
        break;
      case 'R':  // Turn right
        turnRight();
        break;
      case 'M':  // SENSOR button for servo control
        toggleServo();
        break;
      default:
        Stop();  // Stop if no valid command is received
        break;
    }
  }

  delay(100);  // Small delay to avoid excessive processing
}

// Function to move forward
void forward() {
  Serial.println("Forward");
  digitalWrite(relay1, HIGH);  // Motor 1 forward
  digitalWrite(relay2, LOW);   // Motor 1 backward OFF
  digitalWrite(relay3, LOW);   // Motor 2 forward OFF
  digitalWrite(relay4, HIGH);  // Motor 2 backward OFF
}

// Function to move backward
void backward() {
  Serial.println("Backward");
  digitalWrite(relay1, LOW);   // Motor 1 forward OFF
  digitalWrite(relay2, HIGH);  // Motor 1 backward
  digitalWrite(relay3, HIGH);  // Motor 2 forward
  digitalWrite(relay4, LOW);   // Motor 2 backward OFF
}

// Function to turn right
void turnRight() {
  Serial.println("Turn Right");
  digitalWrite(relay1, HIGH);  // Motor 1 forward
  digitalWrite(relay2, LOW);   // Motor 1 backward OFF
  digitalWrite(relay3, HIGH);  // Motor 2 forward
  digitalWrite(relay4, LOW);  // Motor 2 backward
}

// Function to turn left
void turnLeft() {
  Serial.println("Turn Left");
  digitalWrite(relay1, LOW);   // Motor 1 forward OFF
  digitalWrite(relay2, HIGH);  // Motor 1 backward
  digitalWrite(relay3, LOW);   // Motor 2 forward OFF
  digitalWrite(relay4, HIGH);  // Motor 2 backward
}

// Function to stop both motors
void Stop() {
  Serial.println("Stop");
  digitalWrite(relay1, LOW);   // Motor 1 forward OFF
  digitalWrite(relay2, LOW);   // Motor 1 backward OFF
  digitalWrite(relay3, LOW);   // Motor 2 forward OFF
  digitalWrite(relay4, LOW);   // Motor 2 backward OFF
}

// Variable to track servo position
int servoPosition = 0;  // Start at 0 degrees

// Function to toggle the servo between 0 and 90 degrees
void toggleServo() {
  if (servoPosition == 0) {
    servoPosition = 90;  // Move to 90 degrees
    myServo.write(90);
    Serial.println("Servo moved to 90 degrees");
  } else {
    servoPosition = 0;   // Move back to 0 degrees
    myServo.write(0);
    Serial.println("Servo moved to 0 degrees");
  }
}

