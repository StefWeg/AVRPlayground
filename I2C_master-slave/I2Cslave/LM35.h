#ifndef _LM35_H_
#define _LM35_H_
#include <stdio.h>

volatile extern uint32_t ADCVal;
void ADC_init();
uint16_t GetTemperature();

#endif
