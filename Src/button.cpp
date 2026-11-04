#include "button.h"
#include "clock.h"

bool Button::isPressed() const
{
	return (m_port->IDR & (1U << m_pin_number)) == 0;
}

Button::Button(GPIO_TypeDef* port, uint8_t pin_number){
	this->m_port = port;
	this->m_pin_number = pin_number;
}

bool Button::wasClicked(){
	uint32_t now = Clock::getMillis();
	bool current_state = isPressed();
	if (current_state != m_lastState){
		if (now - m_lastPressTime >= debounce_time_ms){
			m_lastPressTime = now;
			m_lastState = current_state;
			return current_state;
		}
	}
	return false;
}

void Button::init(){
	uint32_t port_index = ((uint32_t)m_port - GPIOA_BASE) / 0x400;
	RCC->AHB1ENR |= (1U << port_index);
	(void)RCC->AHB1ENR; // задержка для стабилизации тактирования на шине

	m_port->MODER &= ~(3U << (m_pin_number * 2));

	m_port->PUPDR &= ~(3U << (m_pin_number * 2));
	m_port->PUPDR |= 1U << (m_pin_number * 2);
}
