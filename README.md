# Arduino Electronic Load

A programmable electronic load controller built with Arduino, featuring constant current, constant resistance, and constant power modes. This project provides precise control for testing power supplies, batteries, and other DC sources.

## Features

- **Three Operating Modes:**
  - Constant Load (Resistance) Mode
  - Constant Current Mode  
  - Constant Power Mode
- **Interactive Menu System** with rotary encoder navigation
- **16x2 LCD Display** with custom characters
- **Pause/Resume Functionality** 
- **Precise ADC readings** using ADS1115 16-bit ADC
- **DAC Control** using MCP4725 12-bit DAC
- **Audio Feedback** with buzzer tones

## Hardware Requirements

### Components
- Arduino (Uno/Nano recommended)
- **ADS1115** - 16-bit I2C ADC module
- **MCP4725** - 12-bit I2C DAC module  
- **16x2 LCD with I2C backpack**
- **Rotary Encoder** with push button
- **Push buttons** (2x - menu and pause/resume)
- **Buzzer** for audio feedback
- **1Ω current sense resistor** (precision resistor recommended)
- **Voltage divider** (10kΩ/100kΩ) for voltage sensing
- **Power MOSFET** for load control

### Pin Connections

| Component | Arduino Pin | Notes |
|-----------|-------------|-------|
| Buzzer | D3 | Audio feedback |
| Encoder SW | D8 | Push button |
| Encoder DT | D9 | Data pin |
| Encoder CLK | D10 | Clock pin |
| Red Button | D11 | Pause/Resume |
| Blue Button | D12 | Menu/Back |
| LCD | I2C (A4/A5) | Address: 0x3F or 0x27 |
| ADS1115 | I2C (A4/A5) | Address: 0x48 |
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
   git clone https://github.com/yourusername/Electronic_Load.git
   ```

2. **Install required libraries** in Arduino IDE

3. **Open** `src/main.cpp` in Arduino IDE or PlatformIO

4. **Configure I2C addresses** if needed:
   - LCD: Line 6 - Change `0x3f` to `0x27` if needed
   - ADS1115: Uses `0x48` (configured in setup)
   - MCP4725: Uses `0x60` (configured in setup)

5. **Upload** to your Arduino

## Calibration

### Current Reading Calibration
The current is measured across a 1Ω sense resistor. Adjust the multiplier on **line 76**:
```cpp
const float multiplier = 0.0001827;
```
Compare LCD readings with an external multimeter and adjust this value for accuracy.

### Voltage Reading Calibration  
Voltage is measured through a 10kΩ/100kΩ divider. Adjust the multiplier on **line 84**:
```cpp
const float multiplier_A2 = 0.0020645;
```
Measure actual voltage with a multimeter and adjust for precision.

## Usage

### Menu Navigation
- **Rotate encoder** to navigate menu options
- **Press encoder** to select/confirm
- **Blue button** to go back/cancel
- **Red button** to pause/resume operation

### Operating Modes

#### 1. Constant Load Mode
- Set desired resistance value (0.000001 - 9.999999 Ω)
- Load maintains constant resistance regardless of input voltage
- Useful for testing voltage regulation

#### 2. Constant Current Mode  
- Set desired current (0-9999 mA)
- Load draws constant current regardless of input voltage
- Ideal for battery capacity testing

#### 3. Constant Power Mode
- Set desired power (0-99999 mW) 
- Load consumes constant power
- Good for thermal testing and power supply evaluation

### Display Information
- **Top line:** Setpoint value and input voltage
- **Bottom line:** Actual current, power, and pause status

## Safety Considerations

⚠️ **Important Safety Notes:**
- Ensure proper heat sinking for the power MOSFET
- Monitor component temperatures during operation
- Use appropriate fuses and protection circuits
- Verify voltage divider ratings for your input voltage range
- Test with low power loads before high-power operation

## Technical Specifications

- **Current Range:** 0-4000+ mA (depends on MOSFET and sense resistor)
- **Voltage Range:** 0-50V+ (limited by voltage divider and ADS1115)  
- **Power Range:** 0-200W+ (depends on cooling and MOSFET ratings)
- **Resolution:** 12-bit DAC control (4096 steps)
- **ADC Resolution:** 16-bit (ADS1115)
- **Update Rate:** 300ms (configurable)

## File Structure

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
└── README.md            # This file
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Based on ELECTRONOOBS Electronic Load design
- Uses Adafruit libraries for I2C communication
- Inspired by commercial electronic load designs

## Troubleshooting

### Common Issues

**LCD not displaying:**
- Check I2C address (try 0x27 if 0x3F doesn't work)
- Verify wiring connections
- Run I2C scanner to detect devices

**Incorrect readings:**
- Calibrate multiplier values
- Check sense resistor value and connections
- Verify voltage divider ratios

**No encoder response:**
- Check pin connections (D9, D10)  
- Verify encoder wiring
- Test with multimeter for continuity

**DAC not controlling load:**
- Verify MCP4725 I2C address
- Check MOSFET gate connection
- Test DAC output with multimeter

## Version History

- **v1.0** - Initial release with basic functionality
- More versions to be added...

---

**⚡ Happy Testing! ⚡**
