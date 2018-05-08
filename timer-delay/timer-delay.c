/*
 * timer_delay.c
 *
 *  Author: Stefoo
 */ 
 

#include <avr/io.h>

void Wait_for(uint16_t delay)              //Realizuje opóznienie o zadanym czasie liczonym
{                                          //w odcinkach co 100 us
	TCCR1B = _BV(CS12) | _BV(CS10);         //Preskaler timera 1024
	TCNT1=0;                               //Pobierz aktualny stan licznika timera
	uint16_t val=delay*(F_CPU/10000UL)/1024;   //Przelicz opóznienie
	
	while(TCNT1 < val);                    //Czekaj
}


int main(void)
{
	DDRB = _BV(PB5);  //Port sterujucy LEDem     UWAGA: _BV(PB5) to to samo co 1<<PB5  : 00000001 przesuni?te o 4/5
	while(1)
	{
		PORTB ^= _BV(PB5);     //Zmien stan wyjscia sterujacego LED na przeciwny
		Wait_for(10000);     //Odczekaj sekunde
	}
}
