# Project Structure

This document explains the organization of the RC Car project repository.

## 📁 Directory Structure

```
arduino-rc-car/
│
├── arduino/                          # Arduino sketches
│   ├── rc_car_controller/           # Main motor controller
│   │   └── rc_car_controller.ino    # Production control code
│   └── bluetooth_test/              # HC-05 testing
│       └── bluetooth_test.ino       # Bluetooth connectivity test
│
├── python_script/                    # Python control scripts
│   ├── gamepad_control_script.py    # Main proportional control (recommended)
│   ├── test.py                       # Simple directional control
│   └── myenv/                        # Virtual environment (not in repo)
│
├── docs/                             # Documentation
│   ├── HARDWARE_SETUP.md            # Assembly and wiring guide
│   └── WIRING_DIAGRAM.md            # Circuit diagrams and schematics
│
├── sketch1/                          # Legacy folder (keep for reference)
│   └── sketch1.ino                   # Original sketch (deprecated)
│
├── test_sketch/                      # Legacy folder (keep for reference)
│   └── test_sketch.ino               # Original test (deprecated)
│
├── .gitignore                        # Git ignore rules
├── CONTRIBUTING.md                   # Contribution guidelines
├── LICENSE                           # MIT License
├── README.md                         # Main project documentation
└── requirements.txt                  # Python dependencies
```

## 📄 File Descriptions

### Root Files

- **README.md**: Main project documentation with quick start guide
- **LICENSE**: MIT License for the project
- **CONTRIBUTING.md**: Guidelines for contributing to the project
- **requirements.txt**: Python package dependencies
- **.gitignore**: Excludes virtual environments, cache files, etc.

### Arduino Folder

Contains all Arduino sketches organized by function:

#### `arduino/rc_car_controller/`
- **Primary sketch** for production use
- Implements motor control with smooth acceleration
- Includes failsafe timeout (500ms)
- Parses Bluetooth commands in format: `leftSpeed,rightSpeed\n`

#### `arduino/bluetooth_test/`
- Test sketch for verifying HC-05 connectivity
- Echoes data between Serial Monitor and Bluetooth
- Use this first to confirm HC-05 is working

### Python Script Folder

Contains Python control programs:

#### `gamepad_control_script.py`
- **Recommended for normal use**
- Full proportional control (-255 to 255)
- Tank-style steering algorithm
- 20 Hz update rate
- Smooth, responsive driving

#### `test.py`
- Simple test program
- Basic directional commands (F/B/L/R)
- Good for initial testing
- Less precise control

#### `myenv/`
- Python virtual environment
- **Not tracked in Git** (in .gitignore)
- Contains pygame and pyserial packages

### Documentation Folder

Detailed guides and references:

#### `HARDWARE_SETUP.md`
- Complete Bill of Materials
- Step-by-step assembly instructions
- Voltage divider explanation
- Testing procedures
- Troubleshooting guide

#### `WIRING_DIAGRAM.md`
- ASCII circuit diagrams
- Pin mapping tables
- Motor control truth tables
- Power distribution schematics
- Connection reference table

### Legacy Folders

**Note**: These folders contain the original sketches before reorganization. They are kept for backward compatibility but should not be used for new development.

- `sketch1/`: Original motor controller sketch
- `test_sketch/`: Original Bluetooth test sketch

**Recommended**: Use files in `arduino/` folder instead.

## 🔄 File Relationships

```
Gamepad → Python Script → Bluetooth → Arduino → Motor Driver → Motors
   │           │              │          │           │           │
   USB     pygame lib      HC-05      Serial     L298N      DC Motors
            pyserial                   Parser     PWM
```

## 🚀 Typical Workflow

1. **Hardware Assembly**: Follow `docs/HARDWARE_SETUP.md`
2. **Test Bluetooth**: Upload `arduino/bluetooth_test/bluetooth_test.ino`
3. **Upload Controller**: Upload `arduino/rc_car_controller/rc_car_controller.ino`
4. **Install Python Deps**: `pip install -r requirements.txt`
5. **Configure Script**: Edit COM port in `python_script/gamepad_control_script.py`
6. **Run Controller**: `python python_script/gamepad_control_script.py`

## 📦 For GitHub Repository

When pushing to GitHub, exclude:
- `python_script/myenv/` (virtual environment)
- `__pycache__/` (Python cache)
- `.vscode/` (IDE settings)
- Any personal COM port configurations

These are automatically excluded by `.gitignore`.

## 🔧 Development Setup

For contributors:

1. Clone repository
2. Create virtual environment: `python -m venv myenv`
3. Activate: `myenv\Scripts\activate` (Windows) or `source myenv/bin/activate` (Linux/Mac)
4. Install dependencies: `pip install -r requirements.txt`
5. Create feature branch: `git checkout -b feature/your-feature`
6. Make changes and test
7. Submit pull request

## 📝 Documentation Standards

- **Code comments**: Explain "why", not "what"
- **Function docstrings**: Describe purpose, parameters, and return values
- **README updates**: Keep in sync with code changes
- **Markdown formatting**: Use headers, code blocks, and lists

## 🏷️ Version Control

- Use semantic versioning for releases
- Tag important milestones
- Write clear commit messages
- Keep commits focused and atomic

---

**Questions?** See [CONTRIBUTING.md](CONTRIBUTING.md) or open an issue!
