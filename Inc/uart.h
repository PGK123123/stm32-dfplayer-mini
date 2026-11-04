#pragma once

#include "main.h"

class UART{
	private:
		USART_TypeDef* m_usart;
		static constexpr uint32_t m_baud_rate = 9600;
	public:
		UART(USART_TypeDef* usart);
		void init();
		void send_byte(uint8_t byte);
		void send_buff(const uint8_t* buffer, uint16_t buff_size);

};
