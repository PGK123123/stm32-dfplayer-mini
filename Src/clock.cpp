#include "Clock.h"

uint32_t SystemCoreClock = 16000000UL;
static volatile uint32_t s_ticks = 0;

namespace Clock {

void init(Source source) {
    switch (source) {

        case Source::HSI:
            RCC->CR |= RCC_CR_HSION; // включение тактирования
            while (!(RCC->CR & RCC_CR_HSIRDY)); // ждем пока флаг не сработает

            RCC->CFGR &= ~(RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_HPRE);

            RCC->CFGR &= ~RCC_CFGR_SW; // очистка регистра
            RCC->CFGR |= RCC_CFGR_SW_HSI; // установка флага SW_HSI

            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI); // проверка на подтверждение

            FLASH->ACR = FLASH_ACR_LATENCY_0WS;

            SystemCoreClock = 16 * 1000000UL;
            break;

        case Source::HSE:
            RCC->CR |= RCC_CR_HSEON;
            while (!(RCC->CR & RCC_CR_HSERDY));

            RCC->CFGR &= ~(RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_HPRE);

            RCC->CFGR &= ~RCC_CFGR_SW;
            RCC->CFGR |= RCC_CFGR_SW_HSE;

            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);

            FLASH->ACR = FLASH_ACR_LATENCY_0WS;

            SystemCoreClock = 25 * 1000000UL;
            break;

        case Source::PLL:
            RCC->CR |= RCC_CR_HSEON;
            while (!(RCC->CR & RCC_CR_HSERDY));

            // 1. Настройка Flash Latency ДО переключения на высокую частоту!
            // Для частот 60..84 МГц требуется 2WS (Wait States).
            // Включаем также предварительную выборку (PRFTEN) и кэш команд/данных (ICEN, DCEN).
            FLASH->ACR = FLASH_ACR_LATENCY_2WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

            // 2. Настройка делителей шин: APB1 max 42 МГц (делим на 2), APB2 max 84 МГц
            RCC->CFGR &= ~(RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_HPRE);
            RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB1 = 42 МГц
            RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // APB2 = 84 МГц
            RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB  = 84 МГц

            // 3. Конфигурация PLL: Вход 25 МГц -> /25 (M) * 336 (N) / 4 (P) = 84 МГц
            // Делитель P: 00 -> /2, 01 -> /4 (значение 1), 10 -> /6, 11 -> /8 (значение 3)
            RCC->PLLCFGR = (25 << RCC_PLLCFGR_PLLM_Pos)      |  // M = 25 (25MHz / 25 = 1MHz)
                           (336 << RCC_PLLCFGR_PLLN_Pos)     |  // N = 336 (1MHz * 336 = 336MHz)
                           (1 << RCC_PLLCFGR_PLLP_Pos)       |  // P = 4 -> 336 / 4 = 84 МГц
                           RCC_PLLCFGR_PLLSRC_HSE;              // Источник - HSE

            RCC->CR |= RCC_CR_PLLON;
            while (!(RCC->CR & RCC_CR_PLLRDY));

            RCC->CFGR &= ~RCC_CFGR_SW;
            RCC->CFGR |= RCC_CFGR_SW_PLL;
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

            SystemCoreClock = 84 * 1000000UL;
            break;
    }
    SysTick_Config(SystemCoreClock / 1000);
}

	uint32_t getMillis(){
		return s_ticks;
	}

	void delayMs(uint32_t ms){
		uint32_t start = getMillis();
		while ((getMillis() - start) < ms);
	}
}

extern "C" void SysTick_Handler(void) {
    s_ticks++;
}
