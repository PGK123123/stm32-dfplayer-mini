# STM32 Modern C++ MP3 Player (Bare-Metal CMSIS)

![C++20](https://img.shields.io/badge/Standard-C%2B%2B17%2F20-blue.svg)
![Target](https://img.shields.io/badge/MCU-STM32F401CCU6%20(Blackpill)-brightgreen.svg)
![HAL](https://img.shields.io/badge/Abstractions-Zero--HAL%20%7C%20Pure%20CMSIS-red.svg)
![Architecture](https://img.shields.io/badge/Design-Object--Oriented%20%2F%20Zero--Cost-orange.svg)

Standalone embedded audio player built from the ground up on **STM32F401CCU6 (ARM Cortex-M4 @ 84 MHz)** using **Modern Embedded C++** on **pure CMSIS register level** (no STM32 HAL / LL libraries).

Controls an external **DFPlayer Mini** MP3 decoder module via binary serial protocol (UART 9600 baud, 8N1) with custom hardware-accelerated clock tree, non-blocking debounce drivers, and an analog 12-bit SAR ADC volume controller.

---

## 🚀 Features

- **Pure CMSIS Register-Level Implementation:** Zero dependency on bloated HAL drivers; zero-cost C++ abstractions with minimal binary footprint.
- **Clock Tree & Flash Acceleration:** Configured from external 25 MHz HSE quartz via PLL to core **84 MHz**, 2 Wait States Flash latency, ART Accelerator (ICache, DCache, Prefetch enabled).
- **Millisecond System Timer:** Non-blocking Cortex-M4 `SysTick` timebase (1 kHz) for timestamp-based events and deadband debouncing.
- **Hardware Button Edge & Debounce Driver (`Button`):** Encapsulated input driver with internal pull-up (`PUPDR`) and configurable time-window hysteresis (deadband filter).
- **Universal USART Driver (`UART`):** Hardware USART2 driver (`PA2`/`PA3`) configured for 9600 baud, 8N1 frame format, transmit buffer empty (`TXE`) polling.
- **DFPlayer Binary Protocol Driver (`DFPlayer`):** 10-byte serial packet serializer with dynamic 16-bit negative checksum calculation, tracking playback state (Play / Pause / Resume / Next / Prev).
- **12-bit Analog Volume Control (`Potentiometer`):** Direct sampling of `PA5` (`ADC1_IN5`) with 4-sample averaging filter and threshold-based change detection to eliminate bus flooding.

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

---

## 💡 Future Roadmap: Physical Mini-Cassette Player

The next development phase transforms this prototype into a standalone, retro-futuristic audio player:

1. **Physical "Mini-Cassettes" with RFID/NFC:**
   - 3D-printed miniature cassette cartridges (45 × 30 × 5 mm) embedded with NFC tags (NTAG213 / Mifare UID).
   - Integration of an **MFRC522 (RC522)** reader over **SPI** to automatically trigger albums/folders on cartridge insertion.
2. **Two-Way UART Feedback:**
   - Asynchronous interrupt-driven reception (`USART2_IRQHandler` + RingBuffer) to handle track completion packets (`0x3D`) for seamless album auto-play.
3. **Headphone Jack & Belt Clip:**
   - Stereo 3.5mm mini-jack output (`DAC_R` / `DAC_L`) with automatic mechanical speaker cutoff.
   - EDC-grade spring steel belt clip for wearable retro-portability.
4. **Custom PCB & Power Management:**
   - Single-board design in KiCad with USB Type-C charging (TP4056 + DW01A protection) and Li-Po battery power.

---

## 📜 License & Author

Developed by **Georgiy Pergushev** (SPbETU "LETI", Control Systems & Automation).
Released under the MIT License.
