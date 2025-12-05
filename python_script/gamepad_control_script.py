"""
RC Car Gamepad Controller

Reads USB gamepad input and sends motor control commands to Arduino via Bluetooth.
Uses proportional control for smooth, responsive driving with tank-style steering.

Requirements:
    - pygame: For gamepad input handling
    - pyserial: For Bluetooth serial communication
    - Paired HC-05 Bluetooth module

Usage:
    1. Configure PORT variable to match your HC-05 COM port
    2. Connect USB gamepad
    3. Run: python gamepad_control_script.py
    4. Use left stick: Y-axis for forward/back, X-axis for turning

Author: Your Name
Date: December 2025
License: MIT
"""

import pygame
import serial
import time

# === SETTINGS ===
PORT = "COM7"           # Change this to your HC-05 outgoing COM port
BAUD_RATE = 9600        # Must match Arduino serial baud rate
SEND_INTERVAL = 0.05    # Seconds between sends (20 Hz update rate)

# === SETUP ===
# === SETUP ===
# Initialize pygame and joystick subsystem
pygame.init()
pygame.joystick.init()

# Check if gamepad is connected
if pygame.joystick.get_count() == 0:
    print("No joystick detected!")
    print("Please connect a USB gamepad and try again.")
    exit()

# Initialize first detected gamepad
joystick = pygame.joystick.Joystick(0)
joystick.init()

# === MAIN CONTROL LOOP ===
print("RC Car control active...")

try:
    while True:
        pygame.event.pump()  # Process pygame events (required for joystick updates)

        # Read joystick axes (values range from -1.0 to 1.0)
        forward = -joystick.get_axis(1)  # Y-axis: negative is up, so invert for intuitive control
        turn = joystick.get_axis(0)       # X-axis: positive is right

        # Calculate individual motor speeds using tank steering algorithm
        # Left motor: forward speed minus turn (turning right slows left motor)
        # Right motor: forward speed plus turn (turning right speeds up right motor)
        left_speed = int((forward - turn) * 255)
        right_speed = int((forward + turn) * 255)

        # Constrain values to valid PWM range (-255 to 255)
        left_speed = max(min(left_speed, 255), -255)
        right_speed = max(min(right_speed, 255), -255)

        # Format command and send via Bluetooth serial
        # Format: "leftSpeed,rightSpeed\n"
        command = f"{left_speed},{right_speed}\n"
        ser.write(command.encode())

        # Optional: Print current speeds for debugging
        # print(f"L: {left_speed:4d} | R: {right_speed:4d}", end="\r")

        # Wait before sending next command (maintain 20 Hz update rate)
        time.sleep(SEND_INTERVAL)

except KeyboardInterrupt:
    # Clean shutdown on Ctrl+C
    print("\n\nShutting down...")
    ser.write(b"0,0\n")  # Stop motors
    ser.close()
    pygame.quit()
    print("Disconnected. Goodbye!")
    # Convert joystick values (-1 to 1) into PWM (-255 to 255)
    left_speed = int((forward - turn) * 255)
    right_speed = int((forward + turn) * 255)

    # Limit values
    left_speed = max(min(left_speed, 255), -255)
    right_speed = max(min(right_speed, 255), -255)

    # Send command (always, even if no change)
    command = f"{left_speed},{right_speed}\n"
    ser.write(command.encode())

    last_left_speed = left_speed
    last_right_speed = right_speed

    # Wait before sending again
    time.sleep(SEND_INTERVAL)
