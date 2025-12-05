# Hardware Setup Guide

This guide provides detailed instructions for assembling the RC car hardware.

## 📦 Bill of Materials (BOM)

### Electronics
| Component | Quantity | Specifications | Estimated Cost |
|-----------|----------|----------------|----------------|
| Arduino Uno | 1 | ATmega328P | $20-25 |
| HC-05 Bluetooth Module | 1 | Class 2, ~10m range | $5-10 |
| L298N Motor Driver | 1 | Dual H-Bridge, 2A per channel | $5-8 |
| DC Motors | 2 | 3-6V, with wheels | $10-15 |
| Battery Holder | 1 | 6x AA or 2S LiPo | $3-5 |
| Batteries | 1 set | 9-12V total | $5-10 |
| Jumper Wires | ~20 | Male-to-Female, Male-to-Male | $3-5 |
| Resistors | 2 | 1kΩ and 2kΩ (for voltage divider) | $0.50 |

### Mechanical
| Component | Quantity | Notes |
|-----------|----------|-------|
| RC Car Chassis | 1 | DIY or pre-made |
| Mounting Hardware | - | Screws, standoffs, zip ties |
| Switch | 1 | Optional power switch |

**Total Estimated Cost**: $50-80 USD

## 🔌 Wiring Instructions

### Step 1: HC-05 Bluetooth Module Connection

⚠️ **IMPORTANT**: The HC-05 operates at 3.3V logic, but the Arduino Uno uses 5V. You MUST use a voltage divider on the RX pin to prevent damage.

```
Arduino          Voltage Divider          HC-05
-------          ---------------          -----
5V     ─────────────────────────────────> VCC
GND    ─────────────────────────────────> GND
RX     <────────────────────────────────── TX
                                          
TX     ──────┬──── 1kΩ ──────┬──────────> RX
              │               │
              └──── 2kΩ ──────┴──────────> GND
```

**Voltage Divider Calculation**: 
- Output = 5V × (2kΩ / (1kΩ + 2kΩ)) = 3.33V ✓

**Pin Connections**:
- HC-05 VCC → Arduino 5V
- HC-05 GND → Arduino GND
- HC-05 TX → Arduino RX (Pin 0) *or use SoftwareSerial pins 10/11*
- HC-05 RX → Arduino TX through voltage divider

### Step 2: L298N Motor Driver Connection

**Power Connections**:
```
L298N          Arduino/Power
-----          -------------
12V    ──────> Battery + (9-12V)
GND    ──────> Battery - and Arduino GND (common ground!)
5V     ──────> Arduino 5V (optional, if using onboard regulator)
```

**Control Pins**:
```
L298N Pin      Arduino Pin      Function
---------      -----------      --------
ENA            9 (PWM)          Left motor speed control
IN1            8                Left motor direction A
IN2            7                Left motor direction B
ENB            10 (PWM)         Right motor speed control
IN3            6                Right motor direction A
IN4            5                Right motor direction B
```

**Motor Connections**:
```
L298N Output   Connection
------------   ----------
OUT1           Left Motor (+)
OUT2           Left Motor (-)
OUT3           Right Motor (+)
OUT4           Right Motor (-)
```

### Step 3: Complete Wiring Diagram

```
                    +─────────────+
                    │  Arduino    │
                    │    Uno      │
                    +─────────────+
                     │ │ │ │ │ │ │
    ┌────────────────┘ │ │ │ │ │ └─────────┐
    │ 5V               │ │ │ │ │           GND
    │                  │ │ │ │ │            │
    │         Pin 9────┘ │ │ │ └────Pin 5   │
    │         Pin 8──────┘ │ └─────Pin 6    │
    │         Pin 7────────┘                 │
    │         Pin 10 ─────────┐              │
    │                         │              │
    ▼                         ▼              ▼
+───────+                +──────────+    +────────+
│ HC-05 │                │  L298N   │    │Battery │
│  VCC  │                │  Motor   │    │ 9-12V  │
│  GND  │                │  Driver  │    │  (-)   │
│  TX   │                │          │    └────┬───┘
│  RX   │◄──Volt.Div.    │  ENA IN1 │         │
└───────┘                │  IN2 ENB │         │
                         │  IN3 IN4 │         │
                         +──────────+         │
                          │  │  │  │          │
                          │  │  │  │          │
                         12V GND OUT OUT      │
                          │  │  1-2 3-4       │
                          └──┼────┼────┼──────┘
                             └────┼────┘
                                  │
                              +───────+
                              │Motors │
                              │ L   R │
                              +───────+
```

## 🔨 Assembly Steps

### Step 1: Prepare the Chassis
1. Attach motors to chassis using mounting brackets
2. Install wheels on motor shafts
3. Create mounting platform for electronics (cardboard, acrylic, or 3D printed)

### Step 2: Mount Components
1. **Arduino**: Secure to chassis with standoffs or double-sided tape
2. **L298N**: Mount near motors, ensure heat sink has airflow
3. **HC-05**: Position for easy access (may need to pair/reset)
4. **Battery Pack**: Secure firmly, consider weight distribution

### Step 3: Wire Power System
1. Connect battery to L298N 12V input
2. **CRITICAL**: Connect all grounds together (Arduino, L298N, battery)
3. Add power switch between battery and L298N (optional)
4. Connect Arduino power (USB during programming, 5V from L298N later)

### Step 4: Wire Control Signals
1. Build voltage divider for HC-05 RX pin (use perfboard or breadboard)
2. Connect HC-05 to Arduino (see Step 1 above)
3. Connect L298N control pins to Arduino (see Step 2 above)
4. Use color-coded wires for easier troubleshooting

### Step 5: Connect Motors
1. Connect left motor to OUT1 and OUT2
2. Connect right motor to OUT3 and OUT4
3. **Note**: If motor spins backward, swap its two wires

### Step 6: Secure and Test
1. Secure all wires with zip ties or cable management
2. Double-check all connections
3. **Do not power on yet** - proceed to testing

## ✅ Testing Procedure

### Test 1: Power Check (No Motors)
1. Disconnect motors from L298N
2. Power on Arduino via USB
3. Check 5V at HC-05 with multimeter
4. Arduino LED should light up

### Test 2: Bluetooth Pairing
1. Upload `bluetooth_test.ino` to Arduino
2. Pair HC-05 from PC (PIN: 1234 or 0000)
3. Open Serial Monitor at 9600 baud
4. Send characters - should echo back
5. **If successful**: HC-05 communication works ✓

### Test 3: Motor Test (One at a time)
1. Connect ONE motor to L298N
2. Upload `rc_car_controller.ino`
3. Power L298N with battery
4. From Serial Monitor, send: `100,0` (left motor forward)
5. Motor should spin - if backward, swap wires
6. Send: `-100,0` (reverse)
7. Repeat for other motor: `0,100` and `0,-100`

### Test 4: Full System Test
1. Connect both motors
2. Run Python script: `python gamepad_control_script.py`
3. Test all movements:
   - Forward: Both motors forward
   - Backward: Both motors reverse
   - Left: Left motor slower/reverse
   - Right: Right motor slower/reverse

## 🛡️ Safety Considerations

1. **Voltage Protection**: Always use voltage divider for HC-05 RX
2. **Common Ground**: All components MUST share common ground
3. **Motor Stall**: Don't block motors - can damage driver
4. **Battery Safety**: 
   - Don't short circuit
   - Use appropriate voltage (9-12V max)
   - Monitor temperature during use
5. **Wire Insulation**: Prevent shorts with heat shrink or electrical tape

## 🔍 Common Issues and Solutions

### HC-05 won't pair
- **Solution**: Hold button during power-on for AT mode, reset settings
- Check voltage divider resistor values

### Motors don't spin
- Check L298N jumpers (ENA/ENB enable pins)
- Verify battery voltage >7V
- Test motor directly with battery

### One motor spins backward
- **Solution**: Swap the two wires for that motor

### Weak motor power
- Check battery charge
- Verify PWM connections (pins 9, 10)
- Ensure common ground

### Random disconnects
- Check HC-05 power supply stability
- Reduce distance to controller
- Check for interference from motors (add capacitors)

## 📸 Recommended Setup Photos

Consider taking photos of:
1. Voltage divider assembly
2. Completed wiring (top view)
3. Motor connections
4. Mounted components on chassis
5. Finished RC car

## 🔄 Next Steps

After hardware assembly:
1. Test with `bluetooth_test.ino`
2. Upload `rc_car_controller.ino`
3. Configure Python script with your COM port
4. Start driving!

---

**Need help?** Open an issue on GitHub with photos of your setup.
