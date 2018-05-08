#include <avr\io.h>
#include <avr\interrupt.h>
#include <util\atomic.h>
#include "lm35.h"

volatile uint32_t ADCVal[LM35NO];

ISR(ADC_vect, ISR_NOBLOCK)
{
	 static uint32_t ADCaccum;	// domysnie = 0 (zmienna statyczna)
	 static uint8_t sampleno;	// domysnie = 0 (zmienna statyczna)
	 static uint8_t LM35No;		// domysnie = 0 (zmienna statyczna)

	 ADCaccum += ADC;
	 sampleno++;
	 if(sampleno == NOOFSAMPLES)
	 {
		ADCVal[LM35No] = ADCaccum; // Zapisz zakumulowana wartosc do odpowiedniego miejsca w tabeli wynikow
		ADCaccum = 0;
		sampleno = 0;
		LM35No = (LM35No+1) % LM35NO; // Wybierz kolejny czujnik temperatury
		ADMUX = ((ADMUX & 0b11110000) | LM35No); // Wybierz kolejny kana?
	 }

	 ADCSRA |= _BV(ADSC); // Rozpocznij konwersje
}

void ADC_init()
{
	ADMUX = _BV(REFS0) | _BV(REFS1);	// Wewnetrzne zrodlo referencyjne, kanal 0, wyrównanie do prawej
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
										// Wlacz ADC, przerwania, preskaler 128
	ADCSRA |= _BV(ADSC); // Rozpocznij konwersje
}

uint16_t GetTemperature(uint8_t LMno)
{
	 uint32_t adc;
	 ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	 {
		 adc = ADCVal[LMno];
	 }

	 return (adc * 11000UL) / (1024UL*NOOFSAMPLES);	// Wyznacz usredniona wartosc i przelicz na temperature
}

