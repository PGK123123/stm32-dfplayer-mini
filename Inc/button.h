#ifndef BUTTON_H
#define BUTTON_H

#include "main.h"

class Button {
private:
    GPIO_TypeDef* m_port;
    uint8_t       m_pin_number;
    bool          m_lastState = false;
    uint32_t m_lastPressTime = 0;
    static constexpr uint32_t debounce_time_ms = 50;
public:
    Button(GPIO_TypeDef* port, uint8_t pin_number);

    void init();
    bool isPressed() const;
    bool wasClicked();
};


#endif
