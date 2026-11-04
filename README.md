# STM32 Modern C++ MP3 Player (Bare-Metal CMSIS)



---

## 🛠️ Hardware Pinout & Wiring

| Peripheral | STM32 Pin | Connected Component | Function / Details |
| :--- | :---: | :--- | :--- |
| **HSE Quartz** | `PH0` / `PH1` | 25.000 MHz Oscillator | High-Speed External Clock source |
| **Button: Play/Pause** | `PA0` | Onboard Key / Tact Switch | Active-LOW, internal pull-up enabled |
| **Button: Next Track** | `PA1` | External Tact Switch | Active-LOW, shorted to GND on press |
| **Button: Prev Track** | `PA4` | External Tact Switch | Active-LOW, shorted to GND on press |
| **UART TX** | `PA2` | DFPlayer Pin 2 (`RX`) | 9600 baud, 8N1, AF7 |
| **UART RX** | `PA3` | DFPlayer Pin 3 (`TX`) | 9600 baud, 8N1, AF7 |
| **ADC Volume Pot** | `PA5` | Potentiometer Wiper | Analog Input (`ADC1_IN5`), 0..3.3V |
| **Power Supply** | `GND` | Common Ground Bus | Common ground between STM32, DFPlayer & Bench PSU |
| **DFPlayer VCC** | External 5V | Dedicated 5V / 2A Supply | Prevents audio amplifier current spikes (Brown-Out) |
| **Speaker** | DFPlayer 6 & 8 | 8 $\Omega$ 1W Audio Speaker | Differential BTL output (`SPK_1` / `SPK_2`) |

---

## 📂 Architecture & Project Structure

```
├── Inc/
│   ├── Button.h         # GPIO input & debouncing driver
│   ├── Clock.h          # System clock & SysTick timebase namespace
│   ├── DFPlayer.h       # Audio player protocol & state machine
│   ├── Potentiometer.h  # 12-bit ADC SAR volume sampling driver
│   ├── uart.h           # USART peripheral driver
│   └── main.h           # Core CMSIS includes
├── Src/
│   ├── Button.cpp       # Button implementation
│   ├── Clock.cpp        # HSE, PLL, Flash Latency & SysTick setup
│   ├── DFPlayer.cpp     # Packet assembly & 16-bit checksum
│   ├── Potentiometer.cpp# ADC prescaler, sequence & sampling
│   ├── uart.cpp         # Register-level USART configuration
│   └── main.cpp         # System orchestrator & event loop
├── Startup/
│   └── startup_stm32f401ccux.s
├── STM32F401CCUX_FLASH.ld
└── README.md
```

