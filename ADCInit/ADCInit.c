/*
 * ADCInit.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t res[100];

ISR(ADC_vect)
{
	static uint8_t n;
	res[n++] = ADC;
	if(n == 100)
	{
		ADCSRA = 0;  //Wylacz dalsze przetwarzanie
		n = 0;
	}
}

void ADCInit()
{
	ADMUX = _BV(REFS0) | _BV(MUX0);		// Nap. referencyjne równe Vcc, wejscie pierwsze multipleksera
	DIDR0 = _BV(ADC1D);					// Wylacz funkcje cyfrowa pinu nr 1
	ADCSRB = 0;							// Tryb free running
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADATE) | _BV(ADPS2) | _BV(ADPS0);
										// Preskaler ADC - 32, wlacz ADC i przerwania, free running
}

int main(void)
{
	ADCInit();  //Inicjalizacja ADC
	sei();	
	ADCSRA |= _BV(ADSC);  //Rozpocznij przetwarzanie
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}