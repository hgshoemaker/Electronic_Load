# Arduino Electronic Load - Simplified Version

A clean and intuitive programmable electronic load controller built with Arduino. Features constant current and constant power modes with a streamlined menu-driven interface using a rotary encoder and LCD display.

## Features

- **Two Operating Modes:**
  - Constant Power Mode
  - Constant Current Mode
- **Rotary Encoder Navigation** with intuitive menu system
- **16x2 LCD Display** with custom arrow characters
- **Real-time Monitoring** of voltage, current, and power
- **Simple Interface** - easy to use and understand
- **Precise Control** using 16-bit ADC and 12-bit DAC
- **Fast Response** with optimized control loops

## Hardware Requirements

### Components
- Arduino Nano/Uno
- **ADS1115** - 16-bit I2C ADC module
- **MCP4725** - 12-bit I2C DAC module  
- **16x2 LCD with I2C backpack** (address 0x27)
- **Rotary Encoder** with push button
- **Current sense resistor** for load measurement
- **Voltage divider** for input voltage sensing
- **Power MOSFET** for load control

### Pin Connections

| Component | Arduino Pin | Notes |
|-----------|-------------|-------|
| Encoder CLK | D2 | Clock pin with interrupt |
| Encoder DT | D3 | Data pin |
| Encoder SW | D4 | Push button with pullup |
| LCD | I2C (A4/A5) | Address: 0x27 |
| ADS1115 | I2C (A4/A5) | Address: 0x48 (default) |
| MCP4725 | I2C (A4/A5) | Address: 0x60 |

## Software Dependencies

Install these libraries through the Arduino Library Manager:

```
Adafruit ADS1X15
Adafruit MCP4725  
LiquidCrystal I2C
Wire (built-in)
```

## Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/hgshoemaker/Electronic_Load.git
   ```

2. **Install required libraries** in Arduino IDE

3. **Open** `src/main.cpp` in Arduino IDE or PlatformIO

4. **Configure I2C address** if needed:
   - LCD: Line 7 - Change `0x27` to `0x3F` if your display uses that address

5. **Upload** to your Arduino

## Usage

### Menu Navigation
- **Rotate encoder** to navigate between options
- **Press encoder** to select/enter menus
- **Navigate through** power and current setting screens

### Main Menu
```
> Const. Power
  Const. Current
```

### Operating Modes

#### 1. Constant Power Mode
1. Select "Const. Power" from main menu
2. Press encoder to enter power setting
3. Rotate encoder to adjust power value (±0.1W increments)
4. Press encoder to confirm and start power mode
5. Monitor real-time power, voltage display
6. Press encoder to stop and return to menu

#### 2. Constant Current Mode  
1. Select "Const. Current" from main menu
2. Press encoder to enter current setting
3. Rotate encoder to adjust current value (±0.1A increments)
4. Press encoder to confirm and start current mode
5. Monitor real-time current, voltage display
6. Press encoder to stop and return to menu

### Display Information
- **Setting Screen:** Shows target value with up/down indicator
- **Active Mode:** 
  - **Top line:** Setpoint and input voltage
  - **Bottom line:** Actual measured values

## Calibration

### Current Reading Calibration
Adjust the multiplier constants in the code for your specific hardware:

```cpp
// In currentmode section (around line 95)
curcurrent = ads.readADC_Differential_0_1() * 0.1875 / 1000.00 / 0.0975;
```

### Voltage Reading Calibration  
```cpp
// In powermode section (around line 125)
curvoltage = ads.readADC_SingleEnded(2) * 0.1875 * 11.13 / 1000.00;
```

Compare LCD readings with an external multimeter and adjust the multiplier values for accuracy.

## Safety Considerations

⚠️ **Important Safety Notes:**
- Ensure proper heat sinking for the power MOSFET
- Monitor component temperatures during operation
- Use appropriate fuses and protection circuits
- Start with low power loads for testing
- Verify all connections before applying power

## Technical Specifications

- **Current Control:** Variable based on sense resistor and MOSFET ratings
- **Power Control:** Variable based on input voltage and current limits  
- **Control Resolution:** 12-bit DAC (4096 steps)
- **Measurement Resolution:** 16-bit ADC (ADS1115)
- **Update Rate:** Fast response with 100μs delays in control loops
- **Display Update:** Real-time during operation

## Code Structure

```
Electronic_Load/
├── src/
│   └── main.cpp          # Main Arduino code
├── include/
│   └── README            # Include directory
├── lib/
│   └── README            # Library directory  
├── test/
│   └── README            # Test directory
├── platformio.ini        # PlatformIO configuration
├── LICENSE              # MIT License
└── README.md            # This file
```

### Key Functions

- `setup()` - Initialize hardware and display
- `loop()` - Main control loop with mode handling
- `screen0()` to `screen6()` - Display functions for different menu screens
- `isr0()` - Rotary encoder interrupt handler
- `ISR(PCINT2_vect)` - Button interrupt handler

## Control Algorithm

- **Incremental DAC adjustment** based on error magnitude
- **Real-time feedback** using differential ADC measurements
- **Smooth control** with microsecond timing
- **Automatic limiting** to prevent DAC overflow

## Troubleshooting

### Common Issues

**Display not working:**
- Check I2C address (try 0x3F if 0x27 doesn't work)
- Verify wiring connections
- Run I2C scanner to detect devices

**Encoder not responding:**
- Check pin connections (D2, D3, D4)
- Verify encoder wiring and pullup resistors
- Test encoder with multimeter

**No load control:**
- Verify MCP4725 connections and address
- Check MOSFET gate connection to DAC output
- Test DAC output with multimeter

**Incorrect readings:**
- Calibrate multiplier values in code
- Check sense resistor connections
- Verify voltage divider values

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Arduino community for excellent libraries
- Adafruit for robust I2C sensor libraries
- Electronic load design inspiration from various open-source projects

## Version History

- **v2.0** - Complete rewrite with simplified menu system
- **v1.0** - Initial complex menu-driven version

---

**⚡ Simple, Clean, Effective! ⚡**
