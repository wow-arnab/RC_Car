# Wiring Diagrams

This document contains detailed circuit diagrams and connection schematics for the RC car project.

## 🔌 Complete System Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           RC Car System Architecture                         │
└─────────────────────────────────────────────────────────────────────────────┘

    PC/Laptop                  Bluetooth           Arduino Uno
  ┌──────────┐              ┌──────────┐         ┌───────────┐
  │          │              │  HC-05   │         │           │
  │ Gamepad  │──USB────────▶│ Module   │◀────────│  Board    │
  │ + Python │              │          │ Serial  │           │
  │  Script  │              └──────────┘         └─────┬─────┘
  └──────────┘                                         │
                                                       │ Control Signals
                  Battery Pack                         │
                 ┌──────────┐                          │
                 │ 9-12V DC │                          ▼
                 └─────┬────┘                   ┌─────────────┐
                       │                        │   L298N     │
                       │ Power                  │   Motor     │
                       └───────────────────────▶│   Driver    │
                                                └──────┬──────┘
                                                       │
                                         ┌─────────────┴─────────────┐
                                         │                           │
                                         ▼                           ▼
                                  ┌──────────┐                ┌──────────┐
                                  │ DC Motor │                │ DC Motor │
                                  │  (Left)  │                │ (Right)  │
                                  └──────────┘                └──────────┘
```

## 📍 Arduino Pin Mapping

```
Arduino Uno - Pin Configuration
================================

        ┌─────────────────┐
        │    [USB Port]   │
        │                 │
    ┌───┴─────────────────┴───┐
    │                         │
    │   ┌───────────────┐     │
    │   │  ATmega328P   │     │
    │   └───────────────┘     │
    │                         │
D0  │● RX        ◄──────────  │  (HC-05 TX)
D1  │● TX        ──────────▶  │  (HC-05 RX via divider)
D2  │●                        │
D3  │●                        │
D4  │●                        │
D5  │● PWM       ──────────▶  │  L298N IN4 (Right Dir B)
D6  │● PWM       ──────────▶  │  L298N IN3 (Right Dir A)
D7  │●           ──────────▶  │  L298N IN2 (Left Dir B)
D8  │●           ──────────▶  │  L298N IN1 (Left Dir A)
D9  │● PWM       ──────────▶  │  L298N ENA (Left Speed)
D10 │● PWM       ──────────▶  │  L298N ENB (Right Speed)
D11 │● PWM                    │
D12 │●                        │
D13 │●                        │
    │                         │
GND │● ─────────────────────▶ │  Common Ground
5V  │● ─────────────────────▶ │  HC-05 VCC
    └─────────────────────────┘
```

## 🔋 Power Distribution

```
Power System Schematic
======================

         ┌──[+]──────────────────────────────────┐
         │                                        │
    ┌────┴─────┐                                  │
    │ Battery  │                                  │
    │  9-12V   │                                  │
    │  Pack    │                                  │
    └────┬─────┘                                  │
         │                                        │
         └──[-]──────────────────┬────────────────┤
                                 │                │
                          ┌──────┴──────┐   ┌─────┴──────┐
                          │   L298N     │   │  HC-05     │
                          │   12V  GND  │   │  VCC  GND  │
                          └──────┬──────┘   └─────┬──────┘
                                 │                │
                          Common Ground ──────────┴────────▶ Arduino GND
                                 │
                                 ▼
                            Earth/Chassis
```

**Important**: All components share a common ground!

## 🧮 HC-05 Voltage Divider Circuit

```
Voltage Divider for HC-05 RX Pin
=================================

Arduino TX (5V)                        HC-05 RX (3.3V max)
      │                                       │
      │                                       │
      ├───────┬───── R1: 1kΩ ────────────────┤
      │       │                               │
      │       │                               │
      │       └───── R2: 2kΩ ─────┐          │
      │                            │          │
      │                            │          │
    Input                         GND      Output
     5V                                    3.33V ✓


Formula: Vout = Vin × (R2 / (R1 + R2))
         Vout = 5V × (2kΩ / 3kΩ) = 3.33V

Alternative Values:
- R1: 2kΩ, R2: 3.3kΩ → 3.34V
- R1: 1.8kΩ, R2: 3.3kΩ → 3.24V
```

**Note**: HC-05 TX → Arduino RX doesn't need voltage divider (3.3V is safe for 5V input)

## 🚗 L298N Motor Driver Pinout

```
L298N Module - Top View
=======================

    ┌────────────────────────────────────┐
    │                                    │
12V │●  ┌─────────────────────┐         │
5V  │●  │                     │   ┌───┐ │
GND │●  │    L298N Chip       │   │ M │ │ OUT1 ──▶ Left Motor (+)
    │   │                     │   │ O │ │ OUT2 ──▶ Left Motor (-)
ENA │●◄─┤ Enable A (PWM)      │   │ T │ │
IN1 │●◄─┤ Input 1             │   │ O │ │
IN2 │●◄─┤ Input 2             │   │ R │ │ OUT3 ──▶ Right Motor (+)
IN3 │●◄─┤ Input 3             │   │   │ │ OUT4 ──▶ Right Motor (-)
IN4 │●◄─┤ Input 4             │   │ A │ │
ENB │●◄─┤ Enable B (PWM)      │   └───┘ │
    │   │                     │         │
    │   └─────────────────────┘         │
    │                                    │
    └────────────────────────────────────┘

Pin Functions:
- 12V: Battery positive (9-12V)
- 5V: Regulated 5V output (optional use)
- GND: Common ground
- ENA: Left motor speed (PWM 0-255)
- IN1, IN2: Left motor direction
- IN3, IN4: Right motor direction  
- ENB: Right motor speed (PWM 0-255)
```

### Motor Control Truth Table

```
Left Motor Control (ENA, IN1, IN2)
===================================
ENA    IN1    IN2    Result
----   ----   ----   -----------
0      X      X      Stop
PWM    HIGH   LOW    Forward (speed = PWM)
PWM    LOW    HIGH   Reverse (speed = PWM)
PWM    HIGH   HIGH   Brake
PWM    LOW    LOW    Brake/Coast

Right Motor Control (ENB, IN3, IN4)
====================================
ENB    IN3    IN4    Result
----   ----   ----   -----------
0      X      X      Stop
PWM    HIGH   LOW    Forward (speed = PWM)
PWM    LOW    HIGH   Reverse (speed = PWM)
PWM    HIGH   HIGH   Brake
PWM    LOW    LOW    Brake/Coast
```

## 🔗 HC-05 Bluetooth Module Pinout

```
HC-05 Module - Top View
=======================

    ┌─────────────────┐
    │   [Antenna]     │
    │                 │
    │  ┌──────────┐   │
    │  │ Bluetooth│   │
    │  │   Chip   │   │
    │  └──────────┘   │
    │                 │
VCC │● ◄─────────────  5V
GND │● ◄─────────────  GND
TX  │● ─────────────▶  Arduino RX (Pin 0)
RX  │● ◄─────────────  Arduino TX via Divider
    │                 │
KEY │● (Optional)      Pull HIGH for AT mode
    └─────────────────┘

Default Settings:
- Baud Rate: 9600 (can be changed)
- Device Name: HC-05
- PIN: 1234 or 0000
- Mode: Slave (default)
```

## 🔀 Complete Wiring Table

```
Connection Reference Table
==========================

From Component   Pin/Terminal   Wire Color   To Component     Pin/Terminal
--------------   ------------   ----------   -------------    ------------
Battery +        Positive       Red          L298N            12V
Battery -        Negative       Black        L298N            GND
L298N            GND            Black        Arduino          GND
Arduino          5V             Red          HC-05            VCC
Arduino          GND            Black        HC-05            GND
Arduino          RX (D0)        Green        HC-05            TX
Arduino          TX (D1)        Yellow       Voltage Divider  Input
Voltage Divider  Output         Yellow       HC-05            RX
Arduino          D5             Orange       L298N            IN4
Arduino          D6             Yellow       L298N            IN3
Arduino          D7             Green        L298N            IN2
Arduino          D8             Blue         L298N            IN1
Arduino          D9 (PWM)       Purple       L298N            ENA
Arduino          D10 (PWM)      White        L298N            ENB
L298N            OUT1           Red          Left Motor       Terminal 1
L298N            OUT2           Black        Left Motor       Terminal 2
L298N            OUT3           Red          Right Motor      Terminal 1
L298N            OUT4           Black        Right Motor      Terminal 2
```

## 🛠️ Breadboard Layout (for testing)

```
Voltage Divider on Breadboard
==============================

Row    Column (a-j)
---    -------------
1      [Empty]
2      Arduino TX ──▶ a2
3      1kΩ ─────────▶ a3 (connects a2-a3)
4      HC-05 RX ◄──── a4 (connects a3-a4)
5      2kΩ ─────────▶ a5 (connects a4-a5)
6      GND ◄───────── a6 (connects a5-a6)
```

## 📊 Signal Flow Diagram

```
Control Signal Flow
===================

Gamepad              Python                Bluetooth             Arduino            L298N              Motors
───────              ──────                ─────────             ───────            ─────              ──────

Joystick  ──Read──▶ Pygame  ──Process──▶  Serial   ──Wireless──▶ UART   ──Parse──▶ GPIO  ──Power──▶  Spin
 Input               Library              HC-05                  Pins               PWM               Wheels
                        │                   │                      │                  │                 │
                        │                   │                      │                  │                 │
                   Calculate            Encode             Decode Command         Set Direction        │
                   L/R Speed            Message            Speed Values          Apply PWM            Move
                        │                   │                      │                  │                 │
                     [-255                "\n"               Parse Comma        analogWrite()          │
                      to                 Newline             Separator          digitalWrite()         │
                      255]               Delimiter                                                   Rotate
```

## 🧪 Testing Points

For troubleshooting with a multimeter:

```
Measurement Points
==================

Test Point              Expected Voltage    Notes
----------              ----------------    -----
Battery terminals       9-12V DC            Check battery health
L298N 12V input         9-12V DC            Battery connected
L298N 5V output         ~5V DC              If using onboard regulator
Arduino 5V pin          5V DC               Power good
HC-05 VCC               5V DC               Proper power
HC-05 RX (at pin)       3.3V (when HIGH)    Voltage divider working
Arduino D9 (ENA)        0-5V (varies)       PWM signal present
Motor terminals         Variable            When running
```

## 🔐 Safety Checks

Before powering on:

- [ ] All grounds connected together
- [ ] HC-05 voltage divider in place
- [ ] No short circuits (use continuity test)
- [ ] Battery polarity correct
- [ ] Motor wires secured
- [ ] No loose connections

---

**Tip**: Print this document and check off connections as you wire them!
