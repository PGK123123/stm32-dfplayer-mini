#include <potentiometer.h>
#include "clock.h"

void Potentiometer::init() {
    // 1. Включаем тактирование GPIOA и ADC1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    (void)RCC->APB2ENR; // Задержка шины

    // 2. Настраиваем PA5 в режим Analog Mode (11) без подтяжки
    GPIOA->MODER |= (3U << (5 * 2));
    GPIOA->PUPDR &= ~(3U << (5 * 2));

    // 3. Предделитель АЦП: APB2 (84 МГц) / 4 = 21 МГц (предел 36 МГц)
    ADC->CCR &= ~ADC_CCR_ADCPRE;
    ADC->CCR |= ADC_CCR_ADCPRE_0; // 01: f_PCLK2 / 4

    // 4. Настройка канала и выборки
    // Канал 5, время выборки 84 цикла для стабильности
    ADC1->SMPR2 &= ~(7U << (5 * 3));
    ADC1->SMPR2 |= (4U << (5 * 3)); // 100: 84 cycles

    // Длина последовательности: 1 преобразование
    ADC1->SQR1 &= ~ADC_SQR1_L;
    // 1-е преобразование в регулярной группе — канал 5
    ADC1->SQR3 = 5U;

    // 5. Включаем АЦП
    ADC1->CR2 |= ADC_CR2_ADON;
    Clock::delayMs(1); // Время на стабилизацию опорных источников АЦП
}

uint16_t Potentiometer::readRaw() {
    // Запуск регулярного преобразования софтверным триггером
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Ждем окончания преобразования (флаг EOC)
    while (!(ADC1->SR & ADC_SR_EOC));

    // Чтение регистра DR автоматически сбрасывает флаг EOC
    return (uint16_t)(ADC1->DR & 0x0FFF);
}

uint8_t Potentiometer::readVolume(uint8_t maxVolume) {
    // Простое усреднение из 4 замеров для фильтрации аналогового шума
    uint32_t sum = 0;
    for (uint8_t i = 0; i < 4; ++i) {
        sum += readRaw();
    }
    uint32_t avg = sum / 4;

    // Линейное масштабирование диапазона 0..4095 в 0..maxVolume
    uint32_t vol = (avg * maxVolume) / 4095;
    if (vol > maxVolume) {
        vol = maxVolume;
    }
    return (uint8_t)vol;
}

bool Potentiometer::hasChanged(uint8_t& outVolume, uint8_t maxVolume) {
    uint8_t current = readVolume(maxVolume);
    if (current != m_lastVolume) {
        m_lastVolume = current;
        outVolume = current;
        return true;
    }
    return false;
}
