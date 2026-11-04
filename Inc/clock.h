#pragma once

#include "stm32f4xx.h"

namespace Clock {

    enum class Source {
        HSI,
        HSE,
        PLL
    };

    void init(Source source);
    uint32_t getMillis();          // Возвращает количество миллисекунд с момента старта
    void delayMs(uint32_t ms);      // Миллисекундная задержка

}
