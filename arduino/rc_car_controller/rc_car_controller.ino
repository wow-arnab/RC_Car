/*
 * RC Car Motor Controller
 * 
 * Receives speed commands via Bluetooth (HC-05) and controls two DC motors
 * using L298N motor driver. Features smooth acceleration and failsafe timeout.
 * 
 * Hardware:
 * - Arduino Uno
 * - HC-05 Bluetooth module (connected to hardware serial)
 * - L298N motor driver
 * - 2x DC motors
 * 
 * Command Format: "leftSpeed,rightSpeed\n"
 * Speed Range: -255 (full reverse) to 255 (full forward)
 * 
 * Author: Your Name
 * Date: December 2025
 * License: MIT
 */

// Motor Driver Pin Definitions
#define ENA 9   // Left motor PWM (speed control)
#define IN1 8   // Left motor direction pin 1
#define IN2 7   // Left motor direction pin 2
#define ENB 10  // Right motor PWM (speed control)
#define IN3 6   // Right motor direction pin 1
#define IN4 5   // Right motor direction pin 2

// Serial Communication
String inputString = "";              // Buffer for incoming serial data
unsigned long lastCommandTime = 0;    // Timestamp of last received command
const unsigned long FAILSAFE_TIMEOUT = 500; // Stop motors if no command for 500ms

// Motor State Variables
int currentLeftSpeed = 0;   // Current PWM value for left motor
int currentRightSpeed = 0;  // Current PWM value for right motor

/**
 * Initial setup - configure serial communication and motor pins
 */
void setup() {
  // Initialize serial communication at 9600 baud (matches HC-05 default)
  Serial.begin(9600);
  
  // Configure all motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
/**
 * Main loop - read serial commands and implement failsafe
 */
void loop() {
  // Read incoming serial data character by character
  while (Serial.available()) {
    char c = Serial.read();
    
    // Newline character indicates end of command
    if (c == '\n') {
      processInput(inputString);
      inputString = "";           // Clear buffer for next command
      lastCommandTime = millis(); // Update timestamp
    } else {
      inputString += c;            // Build command string
    }
  }

  // Safety failsafe: stop motors if no command received within timeout period
  if (millis() - lastCommandTime > FAILSAFE_TIMEOUT) {
    stopMotors();
  }
}   } else {
      inputString += c;
    }
  }

  // Failsafe check
/**
 * Parse incoming command and set motor speeds
 * Expected format: "leftSpeed,rightSpeed" (e.g., "150,-100")
 */
void processInput(String data) {
  // Find comma separator
  int commaIndex = data.indexOf(',');
  if (commaIndex == -1) return; // Invalid format, ignore
  
  // Extract and convert speed values
  int leftSpeed = data.substring(0, commaIndex).toInt();
/**
 * Gradually change motor speed to target value for smooth acceleration
 * 
 * @param pwmPin - Enable pin for PWM speed control
 * @param dir1 - Direction control pin 1
 * @param dir2 - Direction control pin 2
 * @param targetSpeed - Desired speed (-255 to 255)
 * @param currentSpeed - Current speed (passed by reference, will be updated)
 */
void setMotorSmooth(int pwmPin, int dir1, int dir2, int targetSpeed, int &currentSpeed) {
  // Determine step direction (accelerate or decelerate)
  int step = (targetSpeed > currentSpeed) ? 5 : -5;
  
  // Gradually change speed in small increments
  while (currentSpeed != targetSpeed) {
    currentSpeed += step;
    
    // Prevent overshooting target
    if ((step > 0 && currentSpeed > targetSpeed) || (step < 0 && currentSpeed < targetSpeed)) {
      currentSpeed = targetSpeed;
    }

    // Set motor direction based on speed sign
    if (currentSpeed > 0) {
      // Forward direction
/**
 * Emergency stop - immediately halt both motors
 * Called by failsafe timeout or can be used for emergency stop
 */
void stopMotors() {
  // Reset speed tracking variables
  currentLeftSpeed = 0;
  currentRightSpeed = 0;
  
  // Set all direction pins LOW (motor coast/stop)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  // Set PWM to zero (no power)
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}     digitalWrite(dir2, LOW);
    }

    // Apply PWM speed (use absolute value, direction set above)
    analogWrite(pwmPin, constrain(abs(currentSpeed), 0, 255));
    
    // Small delay for smooth acceleration (adjust for responsiveness)
    delay(10);
  }
}     digitalWrite(dir2, LOW);
    } else if (currentSpeed < 0) {
      digitalWrite(dir1, LOW);
      digitalWrite(dir2, HIGH);
    } else {
      digitalWrite(dir1, LOW);
      digitalWrite(dir2, LOW);
    }

    analogWrite(pwmPin, constrain(abs(currentSpeed), 0, 255));
    delay(10); // small delay for smooth change
  }
}

void stopMotors() {
  currentLeftSpeed = 0;
  currentRightSpeed = 0;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
