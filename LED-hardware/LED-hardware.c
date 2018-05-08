/*
 * LED_hardware.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>

// TRYB NORMALNY
/* void timer_init()
{
	TCCR1A|= (_BV(COM1A0) | _BV(COM1B0));
	OCR1A = 32678;
	OCR1B = 10000;
	TCCR1B = _BV(CS12);  //Preskaler 256
	TCCR1C = _BV(FOC1A);
	DDRB|= (_BV(PB1) | _BV(PB2));
}*/

// TRYB CTC
/* void timer_init() {
	TCCR1A=(_BV(COM1A0) | _BV(COM1B0));
	TCCR1C=_BV(FOC1A);
	OCR1A=F_CPU/1024/1;
	TCCR1B=_BV(WGM12) | _BV(CS12) | _BV(CS10); // Preskaler 1024, CTC
	DDRB|=(_BV(PB1) | _BV(PB2));
} */

// FAST PWM
/* void timer_init ()
{
	TCCR1A = (_BV(COM1A1) | _BV(COM1B1));
	OCR1A = 180;
	OCR1B = 80;
	TCCR1A |=_BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS10); // Preskaler 1, FastPWM 8-bitowy
	DDRB |= (_BV(PB1) | _BV(PB2));
}*/

// PWM z korekcja fazy
void timer_init ()
{
	TCCR1A = (_BV(COM1A1) | _BV(COM1B1) | _BV(WGM10) | _BV(WGM11));
	TCCR1B = _BV(WGM13) | _BV(CS10); // Preskaler 1, PWM 8-bitowy
	OCR1A = 200;
	OCR1B = 80;
	DDRB |= (_BV(PB1) | _BV(PB2));
}


int main(void)
{
    timer_init();
}
