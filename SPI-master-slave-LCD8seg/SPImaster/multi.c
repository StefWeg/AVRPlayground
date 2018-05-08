#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "multi.h"

#define GLUE(a, b)     a##b

#define LEDPORT1(s)	GLUE(PORT,s)
#define LEDPORT LEDPORT1(LED)
#define LEDDDR1(s)	GLUE(DDR,s)
#define LEDDDR	LEDDDR1(LED)

// Cyfry 0,1,2,3,4,5,6,7,8,9 i blank
static const uint8_t __flash DIGITS[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00};
// Segmenty podlaczane do PD0-PD6 (+ kropka na PD7)

static inline void ShowOnLED(uint8_t val)
{
	uint8_t tmp = 0xFF;
	if((val & 0x7F)<11) tmp = DIGITS[val & 0x7F];
	LEDPORT = tmp;
}

volatile uint8_t LEDDIGITS[LEDDISPNO];

ISR(TIMER0_OVF_vect)
{
	static uint8_t LEDNO;

	PORTC |= 0xF0;		// Wylacz wszystkie wyswietlacze
	LEDNO = (LEDNO+1)%LEDDISPNO;
	ShowOnLED(LEDDIGITS[LEDNO]);
	PORTC = (PORTC & 0xF0) | ((1<<LEDNO) & 0x0F);	// Wybierz kolejny wyswietlacz
	
	// Wyswietl kropke po drugiej cyfrze (stan wysoki na PD7)
	if (PORTC & 0x02) LEDPORT |= 0x80;
	else LEDPORT &= ~0x80;
}

void Timer0Init()
{
    LEDDDR = 0xFF;			// Wszystkie piny portu sa wyjsciem
	DDRC |= 0x0F;			// Piny sterujace cyframi jako wyjscie
	TCCR0B = _BV(CS01);		// Preskaler CLKIO/8
	TIMSK0 |= _BV(TOIE0);	// Odblokuj przerwanie nadmiaru timera 0
}
