/*
 * LED.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

void timer_init()
{
	TIMSK1=_BV(TOIE1);
	TCCR1B=_BV(CS12);  //Preskaler 256
	DDRB|=(_BV(PB1) | _BV(PB2));
}

ISR(TIMER1_OVF_vect)
{
	static uint8_t LED=0b010;
	LED^=0b110;
	uint8_t tmp=PORTB & 0b11111001;
	PORTB=(tmp | LED);
}


int main(void)
{
 timer_init();
 sei();
 while(1);

}
