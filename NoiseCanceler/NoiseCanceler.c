/*
 * NoiseCanceler.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/sleep.h>

ISR(ADC_vect)
{
}

void ADCInit()
{
	ADMUX=_BV(REFS1) | _BV(REFS0);    // Wewnetrzne nap. referencyjne, kanal 0, wyrównanie do prawej
	ADCSRA=_BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);    //Wlacz ADC, przerwania, preskaler 128
}

int GetADC()
{
	set_sleep_mode(SLEEP_MODE_ADC);    //Tryb noise canceller
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {sleep_enable();};     //Odblokuj mozliwosc wejscia w tryb sleep
	sleep_cpu();                       //Wejdz w tryb uœpienia
	sleep_disable();                   //Zablokuj mozliwosc wejscia w tryb sleep
	return ADC;
}


int main(void)
{
	sei();
	ADCInit();
	static int res;
	res=GetADC();
	res=GetADC();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}
