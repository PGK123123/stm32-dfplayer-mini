#include <potentiometer.h>
#include "clock.h"

void Potentiometer::init() {

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    (void)RCC->APB2ENR;

    GPIOA->MODER |= (3U << (5 * 2));
    GPIOA->PUPDR &= ~(3U << (5 * 2));


    ADC->CCR &= ~ADC_CCR_ADCPRE;
    ADC->CCR |= ADC_CCR_ADCPRE_0;


    ADC1->SMPR2 &= ~(7U << (5 * 3));
    ADC1->SMPR2 |= (4U << (5 * 3));


    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR3 = 5U;

    ADC1->CR2 |= ADC_CR2_ADON;
    Clock::delayMs(1);
}

uint16_t Potentiometer::readRaw() {
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while (!(ADC1->SR & ADC_SR_EOC));

    return (uint16_t)(ADC1->DR & 0x0FFF);
}

uint8_t Potentiometer::readVolume(uint8_t maxVolume) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < 4; ++i) {
        sum += readRaw();
    }
    uint32_t avg = sum / 4;

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
