#include <avr\io.h>
#include <avr\interrupt.h>
#include <util\atomic.h>
#include "lm35.h"

volatile uint32_t ADCVal[LM35NO];

ISR(ADC_vect, ISR_NOBLOCK)
{
 static uint32_t ADCaccum;
 static uint8_t sampleno;
 static uint8_t LM35No;

 ADCaccum+=ADC;
 sampleno++;
 if(sampleno==NOOFSAMPLES)
 {
  ADCVal[LM35No]=ADCaccum;
  ADCaccum=0;
  sampleno=0;
  LM35No=(LM35No+1)%LM35NO;
  ADMUX=((ADMUX & 0b11110000) | LM35No);
 }
 ADCSRA|=_BV(ADSC); //Rozpocznij konwersjê
}

void ADC_init()
{
	ADMUX=_BV(REFS0) | _BV(REFS1);	//Wew. Ÿród³o referencyjne, kana³ 0, wyrównanie do prawej
	ADCSRA=_BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);	//W³¹cz ADC, przerwania, preskaler 128
	ADCSRA|=_BV(ADSC); //Rozpocznij konwersjê
}

uint16_t GetTemperature(uint8_t LMno)
{
 uint32_t adc;
 ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
 {
  adc=ADCVal[LMno];
 }

 return (adc*11000UL)/(1024UL*NOOFSAMPLES);
}

