# Arduino RC Car with Bluetooth Gamepad Control

A wireless remote-controlled car project using Arduino, HC-05 Bluetooth module, and gamepad control via Python. Features smooth proportional control, tank-style steering, and safety failsafe mechanisms.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Arduino](https://img.shields.io/badge/Arduino-00979D?logo=Arduino&logoColor=white)
![Python](https://img.shields.io/badge/Python-3.8+-3776AB?logo=python&logoColor=white)

## 📋 Table of Contents
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Hardware Setup](#hardware-setup)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

- **Proportional Speed Control**: Full PWM range (0-255) for variable motor speeds
- **Tank-Style Steering**: Differential motor control for smooth turning
- **Smooth Acceleration**: Gradual speed changes prevent jerky movements
- **Safety Failsafe**: Automatic motor shutdown if connection is lost (500ms timeout)
- **Real-time Response**: 20 Hz command update rate for responsive control
- **Bidirectional Drive**: Forward and reverse movement support
- **Wireless Control**: Bluetooth connectivity up to 10 meters (typical HC-05 range)

## 🔧 Hardware Requirements

### Electronics
- **Arduino Uno** (or compatible board)
- **HC-05 Bluetooth Module**
- **L298N Motor Driver** (or similar dual H-bridge)
- **2x DC Motors** (with wheels)
- **Power Supply**:
  - 9-12V battery for motors (2S LiPo or 9V battery pack)
  - USB or separate 5V for Arduino
- **Chassis** (RC car body or DIY frame)

### Connections Reference
| Component | Arduino Pin | Notes |
|-----------|-------------|-------|
| HC-05 RX | TX (via voltage divider) | Use 1kΩ & 2kΩ resistors |
| HC-05 TX | RX | Direct connection |
| L298N ENA | Pin 9 (PWM) | Left motor speed |
| L298N IN1 | Pin 8 | Left motor direction |
| L298N IN2 | Pin 7 | Left motor direction |
| L298N ENB | Pin 10 (PWM) | Right motor speed |
| L298N IN3 | Pin 6 | Right motor direction |
| L298N IN4 | Pin 5 | Right motor direction |

### Controller
- **USB Gamepad/Joystick** (Xbox, PlayStation, or generic USB controller)

## 💻 Software Requirements

### PC Side
- **Python 3.8+**
- **Windows** (tested) / Linux / macOS
- Required Python packages:
  - `pygame` - For gamepad input
  - `pyserial` - For Bluetooth serial communication

### Arduino Side
- **Arduino IDE** 1.8+ or Arduino CLI
- No additional libraries required (uses built-in SoftwareSerial)

## 📦 Installation

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/arduino-rc-car.git
cd arduino-rc-car
```

### 2. Set Up Python Environment
```bash
# Create virtual environment (recommended)
python -m venv venv

# Activate virtual environment
# Windows:
venv\Scripts\activate
# Linux/Mac:
source venv/bin/activate

# Install dependencies
pip install -r requirements.txt
```

### 3. Upload Arduino Code
1. Open `arduino/rc_car_controller/rc_car_controller.ino` in Arduino IDE
2. Select your Arduino board and COM port
3. Upload the sketch to your Arduino

### 4. Pair HC-05 Bluetooth Module
1. Power on your RC car
2. On your PC, go to Bluetooth settings
3. Pair with "HC-05" (default PIN: 1234 or 0000)
4. Note the outgoing COM port assigned (e.g., COM7)

## 🔌 Hardware Setup

### Circuit Diagram
```
Arduino Uno          L298N Motor Driver         Motors
-----------          ------------------         ------
Pin 9 (PWM) ------>  ENA                    |-> Left Motor (+)
Pin 8       ------>  IN1                    |
Pin 7       ------>  IN2                    |-> Left Motor (-)
Pin 10 (PWM)------>  ENB                    |
Pin 6       ------>  IN3                    |-> Right Motor (+)
Pin 5       ------>  IN4                    |-> Right Motor (-)
GND         ------>  GND

HC-05 Module
------------
VCC -> 5V
GND -> GND
TX  -> Arduino RX
RX  -> Arduino TX (via voltage divider: 1kΩ to TX, 2kΩ to GND)
```

### Assembly Steps
1. Mount motors on chassis
2. Connect motor driver to Arduino (see pin connections above)
3. Wire HC-05 module with proper voltage divider on RX pin
4. Connect motors to L298N outputs
5. Power the motor driver with battery (ensure common ground with Arduino)
6. Secure all components to chassis

## 🎮 Usage

### 1. Configure COM Port
Edit `python_script/gamepad_control_script.py`:
```python
PORT = "COM7"  # Change to your HC-05 COM port
```

### 2. Connect Controller
- Plug in your USB gamepad
- Ensure it's recognized by your system

### 3. Run the Control Script
```bash
cd python_script
python gamepad_control_script.py
```

### 4. Control the Car
- **Left Stick Y-axis**: Forward/Backward
- **Left Stick X-axis**: Left/Right steering
- The car uses tank-style steering (differential motor speeds)

### Alternative Simple Test Mode
For basic directional control (F/B/L/R):
```bash
python test.py
```
- **Right Trigger (RT)**: Forward
- **Left Trigger (LT)**: Backward
- **Left Stick**: Steering

## 📁 Project Structure

```
arduino-rc-car/
├── arduino/
│   ├── rc_car_controller/
│   │   └── rc_car_controller.ino    # Main motor control sketch
│   └── bluetooth_test/
│       └── bluetooth_test.ino        # HC-05 connectivity test
├── python_script/
│   ├── gamepad_control_script.py    # Main proportional control
│   ├── test.py                       # Simple directional control
│   └── myenv/                        # Virtual environment (not in repo)
├── docs/
│   ├── HARDWARE_SETUP.md             # Detailed hardware guide
│   └── WIRING_DIAGRAM.md             # Circuit diagrams
├── .gitignore
├── requirements.txt
├── LICENSE
└── README.md
```

## 🔍 How It Works

### Communication Protocol
- **Baud Rate**: 9600
- **Command Format**: `<left_speed>,<right_speed>\n`
- **Speed Range**: -255 (full reverse) to 255 (full forward)
- **Update Rate**: 20 Hz (every 50ms)

### Control Algorithm
1. Python script reads gamepad input using pygame
2. Joystick axes mapped to forward/turn values (-1.0 to 1.0)
3. Values converted to motor speeds:
   - `left_speed = (forward - turn) * 255`
   - `right_speed = (forward + turn) * 255`
4. Commands sent via Bluetooth serial
5. Arduino parses commands and applies smooth acceleration
6. If no command received for 500ms, motors stop (failsafe)

### Tank Steering Example
- **Forward**: Both motors at same positive speed
- **Forward + Left**: Left motor slower, right motor faster
- **Spin Left**: Left motor reverse, right motor forward

## 🐛 Troubleshooting

### Python Script Issues
**"No joystick detected!"**
- Ensure gamepad is connected and recognized by your OS
- Try unplugging and reconnecting

**"Permission denied" on COM port**
- Close any other programs using the port (Arduino Serial Monitor, etc.)
- Verify the correct COM port number

**Car not responding**
- Check Bluetooth connection (HC-05 LED should be solid, not blinking)
- Verify COM port in script matches paired HC-05 port
- Test with `bluetooth_test.ino` and Arduino Serial Monitor

### Arduino Issues
**Motors not spinning**
- Check motor driver connections
- Verify power supply voltage (9-12V for motors)
- Test motors directly with battery

**Erratic movement**
- Check for loose connections
- Ensure common ground between Arduino and motor driver
- Verify PWM pins are connected correctly

**Bluetooth won't pair**
- Reset HC-05 (power cycle)
- Try default PINs: 1234 or 0000
- Check voltage divider on RX pin (prevents damage from 5V logic)

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes:

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Arduino community for extensive documentation
- pygame developers for excellent gamepad support
- HC-05 Bluetooth module manufacturers

## 📧 Contact

Your Name - [@me__arnab](https://www.instagram.com/me__arnab/)

Project Link: [https://github.com/wow-arnab/RC_Car](https://github.com/wow-arnab/RC_Car)

---

**⭐ If you found this project helpful, please consider giving it a star!**
