#include <avr\io.h>
#include <stdio.h>
#include <avr\interrupt.h>
#include <util\atomic.h>

#define NOOFSAMPLES	128

volatile uint32_t ADCVal;

ISR(ADC_vect)
{
 static uint32_t ADCaccum;
 static uint8_t sampleno;

 ADCaccum+=ADC;
 sampleno++;
 if(sampleno==NOOFSAMPLES)
 {
  ADCVal=ADCaccum;
  ADCaccum=0;
  sampleno=0;
 }
}

void ADC_init()
{
	ADMUX=_BV(REFS0) | _BV(REFS1) | 0b0011;	//Wew. referencyjne, kana- 3, wyr¡wnanie do prawej
	ADCSRA=_BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);	//W-¦cz ADC, przerwania, preskaler 128
	ADCSRA|=(_BV(ADATE) | _BV(ADSC));
}

uint16_t GetTemperature()
{
 uint32_t adc;
 ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
 {
  adc=ADCVal;
 }

 return (adc*11000UL)/(1024UL*NOOFSAMPLES);
}
