#pragma once

#include <cstdint>
#include "stm32f4xx.h"

class Potentiometer {
private:
    uint8_t m_lastVolume = 255; // Заведомо невалидное начальное значение

public:
    void init();
    uint16_t readRaw();
    uint8_t readVolume(uint8_t maxVolume = 30);
    bool hasChanged(uint8_t& outVolume, uint8_t maxVolume = 30);
};
