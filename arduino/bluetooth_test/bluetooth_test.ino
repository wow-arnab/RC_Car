/*
 * HC-05 Bluetooth Test Sketch
 * 
 * Simple test program to verify HC-05 Bluetooth module connectivity.
 * Allows bidirectional communication between Arduino and paired device.
 * 
 * Usage:
 * 1. Upload this sketch to Arduino
 * 2. Pair HC-05 from PC (PIN: 1234 or 0000)
 * 3. Open Serial Monitor at 9600 baud
 * 4. Send characters - they should echo back via Bluetooth
 * 
 * Hardware:
 * - HC-05 RX -> Arduino D11 (via voltage divider)
 * - HC-05 TX -> Arduino D10
 * - HC-05 VCC -> 5V
 * - HC-05 GND -> GND
 * 
 * Author: Your Name
 * Date: December 2025
 * License: MIT
 */

#include <SoftwareSerial.h>

// Define software serial pins for HC-05
// RX: Pin 10 receives data from HC-05 TX
// TX: Pin 11 sends data to HC-05 RX (use voltage divider!)
SoftwareSerial BT(10, 11);

/**
 * Initial setup - configure serial communication
 */
void setup() {
  // Initialize USB serial for debugging (Arduino IDE Serial Monitor)
  Serial.begin(9600);
  
  // Initialize Bluetooth serial (HC-05 default baud rate)
  BT.begin(9600);
  
  // Startup message
  Serial.println("Arduino ready. Waiting for Bluetooth data...");
  Serial.println("Type in Serial Monitor to send to HC-05");
  Serial.println("Data from HC-05 will appear here");
}

/**
 * Main loop - relay data between Bluetooth and USB Serial
 */
void loop() {
  // If data received from HC-05 Bluetooth module
  if (BT.available()) {
    char c = BT.read();
    Serial.print("BT Received: ");
    Serial.println(c);
  }

  // If data received from USB Serial Monitor (for testing)
  // Send it to HC-05 to transmit via Bluetooth
  if (Serial.available()) {
    char c = Serial.read();
    BT.write(c);
    Serial.print("Sent to BT: ");
    Serial.println(c);
  }
}
