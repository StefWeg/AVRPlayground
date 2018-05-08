#ifndef _LM35_H_
#define _LM35_H_

#include <stdint.h>

#define NOOFSAMPLES	128
#define LM35NO 2

extern volatile uint32_t ADCVal[LM35NO];

void ADC_init();
uint16_t GetTemperature(uint8_t LMno);

#endif
