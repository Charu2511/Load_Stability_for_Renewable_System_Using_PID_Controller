# Load Stability for Renewable System Using PID Controller

An Arduino Mega 2560-based bidirectional buck-boost converter system designed to maintain stable load voltage in standalone solar power applications using a microcontroller-based PID controller.

## Overview

This project presents a voltage regulation system for standalone solar power applications, where fluctuations in solar energy availability can cause instability in the output voltage supplied to electrical loads.

The proposed system integrates a bidirectional buck-boost converter with a microcontroller-based PID controller to maintain a regulated 12V output. It manages power flow between an 18V DC source representing solar power and a rechargeable Li-Po battery, ensuring continuous power delivery during variations in solar availability.

When sufficient solar power is available, the converter operates in buck mode to supply the load and charge the battery. When solar power becomes unavailable or insufficient, the battery supplies the load through boost-mode operation.

The system uses voltage and current sensors for real-time feedback, an Arduino Mega 2560 for PID-based control and PWM generation, and a relay-based switching mechanism for source management. An LCD provides real-time information about the system's operating condition.

The reported simulation and hardware testing demonstrate output voltage regulation within approximately ±1% of the 12V reference under the tested operating conditions.

## Key Features

- **Bidirectional Power Conversion:** Supports power flow between the solar source, battery, and load through buck and boost operating modes.
- **PID-Based Voltage Regulation:** Uses a microcontroller-based PID controller to regulate the load voltage around a 12V reference.
- **Automatic Source Switching:** Uses relay-based control to manage the transition between solar and battery power.
- **Real-Time Voltage Monitoring:** Measures the load voltage through an analog voltage sensor.
- **Current Monitoring:** Uses an ACS712-30A current sensor to monitor charging and discharging current.
- **PWM-Based Converter Control:** Generates PWM signals through the Arduino Mega 2560 to control the converter switching circuitry.
- **Battery Energy Management:** Charges the battery when sufficient solar power is available and uses stored battery energy when solar power is insufficient.
- **LCD Monitoring:** Displays real-time voltage, current, and charging/discharging status.
- **Output Voltage Stability:** Demonstrates regulation of the output around 12V, with reported variation within approximately ±1%.

## System Specifications

| Parameter                  | Specification                                      |
| -------------------------- | -------------------------------------------------- |
| System Type                | Standalone Solar Power System                     |
| Converter Topology         | Bidirectional Buck-Boost Converter                 |
| Microcontroller            | Arduino Mega 2560                                  |
| Solar/DC Input             | 18V DC Source                                      |
| Battery Type               | Rechargeable Li-Po Battery                        |
| Battery Voltage            | 11.1V (Nominal)                                    |
| Battery Capacity           | 1500 mAh                                           |
| Reference Output Voltage   | 12V                                                |
| Controller                 | PID Controller                                     |
| Current Sensor             | ACS712-30A                                         |
| Voltage Sensor             | 25V Analog Voltage Sensor Module                   |
| Switching Devices          | IRFZ44N N-Channel MOSFETs                           |
| Inductor                   | 500 µH                                             |
| Output Load                | 12V, 5W SMD Bulb                                   |
| Source Switching           | 12V Relay                                          |
| Gate Driver                | TLP250                                             |
| Display                    | 16×2 LCD                                           |
| Control Signal             | PWM                                                |
| Reported Voltage Regulation| Approximately ±1% of 12V                           |

## Hardware Components

| Component                   | Function                                                  |
| --------------------------- | --------------------------------------------------------- |
| Arduino Mega 2560           | Main processing and PID-based control unit                |
| 18V DC Source               | Represents the solar panel input                          |
| Rechargeable Li-Po Battery | Stores energy and supplies the load when solar power is unavailable |
| IRFZ44N MOSFETs (2)         | Switching devices for bidirectional power conversion     |
| 500 µH Inductor             | Stores and transfers energy during converter operation   |
| 470 µF Capacitors (4)       | Energy storage and voltage ripple reduction               |
| ACS712-30A                  | Measures charging and discharging current                 |
| 25V Voltage Sensor Module   | Measures the load voltage for feedback control            |
| TLP250 Gate Driver          | Provides gate-driving and signal-level interfacing       |
| B1212S DC-DC Converter      | Provides electrical isolation                             |
| 12V Relay                   | Controls switching between solar and battery power sources |
| 16×2 LCD Display            | Displays voltage, current, and operating status            |
| 12V, 5W SMD Bulb            | Test load for the converter system                        |
| Resistors                   | Current limiting and circuit interfacing                  |
| PCB / Breadboard            | Hardware circuit implementation                           |

## Working Methodology

The system operates through four primary functional stages:

### 1. Voltage and Current Measurement

The system continuously monitors the electrical parameters required for feedback control and power management.

- The voltage sensor measures the load voltage.
- The ACS712-30A current sensor measures the current flowing through the system.
- The Arduino Mega 2560 reads the analog sensor signals.
- The measured voltage is compared with the 12V reference value.
- The measured current is used to monitor the charging and discharging conditions.

The sensor measurements provide feedback to the controller and support the source management process.

### 2. PID-Based Output Voltage Regulation

A PID controller is implemented in the Arduino Mega 2560 to regulate the output voltage around the desired 12V reference.

The controller continuously evaluates the difference between the reference voltage and the measured load voltage.

**Voltage Error:**

e(t) = V_ref − V_out

Where:

- V_ref = Reference output voltage (12V)
- V_out = Measured output voltage
- e(t) = Voltage regulation error

The PID controller generates a control signal based on the voltage error.

**PID Control Equation:**

u(t) = Kp e(t) + Ki ∫e(t)dt + Kd de(t)/dt

Where:

- Kp = Proportional gain
- Ki = Integral gain
- Kd = Derivative gain
- e(t) = Voltage error
- u(t) = Controller output

The controller adjusts the PWM duty cycle according to the feedback signal to regulate the converter output.

### 3. Bidirectional Buck-Boost Converter Operation

The converter supports two operating modes depending on solar power availability and battery operating conditions.

**Buck Mode — Solar Power Available**

When sufficient solar power is available:

1. The 18V DC source supplies the converter.
2. The converter operates in buck mode to regulate the voltage for the 12V load.
3. The battery receives charging power.
4. The PID controller adjusts the PWM duty cycle to maintain the desired output voltage.
5. The relay-based source management system selects the solar supply path.

**Boost Mode — Solar Power Unavailable**

When solar power becomes unavailable or insufficient:

1. The battery becomes the primary energy source.
2. The converter operates in boost mode.
3. The battery supplies energy to the load through the converter.
4. The PID controller regulates the output around the 12V reference.
5. The relay-based switching mechanism manages the transition between power sources.

### 4. Source Management and System Monitoring

The system uses relay-based switching logic to manage the power source according to solar availability.

| Operating Condition       | Converter Mode | System Operation                                  |
| ------------------------- | -------------- | ------------------------------------------------- |
| Sufficient solar power    | Buck           | Supplies the load and charges the battery         |
| Solar power unavailable   | Boost          | Battery supplies the load                         |
| Solar power insufficient  | Boost          | Battery supports the load                         |

The 16×2 LCD displays important operating parameters, including:

- Load voltage
- Charging current
- Discharging current
- Charging/discharging status

This allows the operating condition of the system to be monitored during testing.

## Software and Development Tools

- **Arduino IDE:** Embedded programming and implementation of the PID controller.
- **C/C++:** Microcontroller programming and PWM-based control.
- **MATLAB/Simulink:** System modeling, simulation, and control-system analysis.
- **Proteus:** Circuit design and hardware simulation.
- **PID Control:** Feedback-based output voltage regulation.
- **PWM Generation:** Switching control of the bidirectional converter.

## Repository Structure

```text
Load-Stability-for-Renewable-System-Using-PID-Controller/
│
├── README.md
│
├── Arduino_Code/
│   └── main.ino
│
├── Circuit_Diagram/
│   ├── circuit_diagram.png
│   └── circuit_connection.png
│
├── Simulation/
│   ├── matlab_simulink/
│   └── proteus/
│
├── Documentation/
│   └── project_report.pdf
│
└── Images/
    └── hardware_implementation.jpg
