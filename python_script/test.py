"""
RC Car Simple Test Controller

Basic directional control (Forward/Back/Left/Right) for testing RC car functionality.
Uses triggers for forward/backward and stick for steering.

This is a simplified version for testing - use gamepad_control_script.py for 
full proportional control.

Requirements:
    - pygame: For gamepad input handling
    - pyserial: For Bluetooth serial communication

Usage:
    1. Configure BT_PORT to match your HC-05 COM port
    2. Connect USB gamepad
    3. Run: python test.py
    4. RT = Forward, LT = Backward, Left Stick = Steering

Author: Your Name
Date: December 2025
License: MIT
"""

import pygame
import serial
import time

# --- CONFIG ---
BT_PORT = "COM7"   # Change to your HC-05 outgoing port
BT_BAUD = 9600     # Must match Arduino baud rate

# --- SETUP ---
# Initialize Bluetooth Serial connection
try:
    ser = serial.Serial(BT_PORT, BT_BAUD, timeout=0.1)
    time.sleep(2)  # Allow time for Arduino to reset
    print(f"Connected to {BT_PORT} at {BT_BAUD} baud")
except serial.SerialException as e:
    print(f"Error: Could not open {BT_PORT}")
    print(f"Details: {e}")
    exit()

# Initialize Gamepad
pygame.init()
pygame.joystick.init()

# Check for connected gamepad
if pygame.joystick.get_count() == 0:
    print("⚠️ No gamepad detected!")
    print("Please connect a USB gamepad and try again.")
    ser.close()
    exit()

# Initialize first detected gamepad
# Main control loop
print("Simple test mode active...\n")
try:
    while True:
        pygame.event.pump()  # Process pygame events

        # Read axis values
        lx = joystick.get_axis(0)   # Left stick X-axis (steering)
        rt = joystick.get_axis(5)   # Right trigger (forward)
        lt = joystick.get_axis(2)   # Left trigger (backward)

        # Normalize trigger values (some controllers report -1 to 1, others 0 to 1)
        rt = (rt + 1) / 2   # Normalize to 0..1
        lt = (lt + 1) / 2   # Normalize to 0..1

        # Determine command based on input
        command = None

        if rt > 0.5:
            command = "F"   # Forward
        elif lt > 0.5:
            command = "B"   # Backward
        elif lx < -0.5:
            command = "L"   # Left
        elif lx > 0.5:
            command = "R"   # Right

        # Send command if detected
        if command:
            ser.write(command.encode())
            print(f"Sent: {command}")

        # Delay before next check
        time.sleep(0.1)

except KeyboardInterrupt:
    print("\n\nExiting...")
finally:
    # Clean shutdown
    ser.close()
    pygame.quit()
    print("Disconnected. Goodbye!")rite(command.encode())
            print(f"Sent: {command}")

        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()
    pygame.quit()
