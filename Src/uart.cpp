#include "uart.h"

UART::UART(USART_TypeDef* usart) : m_usart(usart) {}

void UART::init() {
    uint32_t pclk = 0;
    if (m_usart == USART2) {
    	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    	GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    	GPIOA->MODER |= 2U << GPIO_MODER_MODER2_Pos | 2U << GPIO_MODER_MODER3_Pos;

    	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);
    	GPIOA->AFR[0] |= 7UL << 8U | 7UL << 12U;

        pclk = 42000000;
    }
    else if (m_usart == USART1) {
    	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    	GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
		GPIOA->MODER |= 2U << GPIO_MODER_MODER9_Pos | 2U << GPIO_MODER_MODER10_Pos;

		GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH1 | GPIO_AFRH_AFRH2);
		GPIOA->AFR[1] |= 7UL << 4U | 7UL << 8U;

        pclk = 84000000;
    }

    m_usart->CR1 &= ~USART_CR1_M; // 8 бит данных
    m_usart->CR1 &= ~USART_CR1_PCE; // Без проверки четности
    m_usart->CR2 &= ~USART_CR2_STOP; // 1 стоп-бит

    m_usart->BRR = pclk / m_baud_rate;
    // 5. Включение передатчика (TE) и самого USART (UE)
    m_usart->CR1 |= USART_CR1_TE | USART_CR1_UE;
}

void UART::send_byte(uint8_t byte){
	while(!(m_usart->SR & USART_SR_TXE));
	m_usart->DR = byte;
}

void UART::send_buff(const uint8_t* buffer, uint16_t buff_size) {
    for (uint16_t i = 0; i < buff_size; ++i) {
        send_byte(buffer[i]);
    }
}
