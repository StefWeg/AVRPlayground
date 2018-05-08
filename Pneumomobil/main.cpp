/*
 * Pneumomobil_v2.cpp
 *
 * Created: 17.08.2017 22:06:45
 * Author : Stefoo
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t * const currentPIND = &GPIOR0;
volatile uint8_t * const currentPINC = &GPIOR1;

volatile uint8_t* const flags = &GPIOR2;
#define PROLONG 0
#define ECOMODE 1


void pinInit()
{
	DDRD = 0x00; // wszystkie piny ustawione jako wejscia
	PORTD = 0x43; // zewnetrzny pull-up na kolektorze fototranzystora dla pinów D2-D5 oraz na przycisku dla pinu D7
	asm volatile ("nop"); // wewnetrzny pull-up dla reszty pinow: w tym D7 (AIN1)

	DDRC = 0x0E; // piny C1-C3 jako wyjscia
	PORTC = 0b11111101; // wejscia z pull-up, wyjscie C1 ustawione na stan niski (wysoki stan na wyj?ciu przeka?nika)
	asm volatile ("nop");

	DDRB = 0x01; // pin B0 jako wyjscie, piny B1-B7 jako wejscia
	PORTB = 0xFE; // wejscia z pull-up, wyjcie B0 ustawione na stan niski
	asm volatile ("nop");
}

// PINOUT DESCRIPTION  
/*
// D5 - sensor 1 (full retraction) | D4 - sensor 2 (Eco retraction)
// D3 - sensor 3 (Eco extension) | D2 - sensor 4 (full extension)

// D7 - AIN1: wejscie odwracajace komparatora analogowego (PD6 = AIN0)

// C1(A1) - divider A | C2(A2) - divider Eco | C3(A3) - divider B
// WARNING! Low voltage on PIN means that there is high voltage on relay OUTPUT (OUTPUT is NC to HIGH VOLTAGE)

// B0 - ECOMODE signal diode
*/

ISR (ANALOG_COMP_vect)
{
	*flags ^= _BV(ECOMODE);	// change ECOMODE flag state
	*flags |= _BV(PROLONG);	// set PROLONG flag to 1

	if (*flags & _BV(ECOMODE)) PORTB |= _BV(PORTB0);
	else PORTB &= ~_BV(PORTB0);

	GTCCR = _BV(PSRSYNC);	// reset prescaler for timers 0 & 1
	TCNT1 = 0;				// reset counter 1
	TIMSK1 |= (_BV(TOIE1));	// enable overflow interrupt for timer 1

	ACSR &= ~_BV(ACIE);		// disable comparator interrupt
}

void comparator_init()
{
	ACSR |= (_BV(ACIE) | _BV(ACBG) | _BV(ACIS1) | _BV(ACIS0));
	// odblokowanie przerwan komparatora, ustawienie napiecia bandgap reference (1.1V) na wejsciu nieodwracajacym
	// oraz okreslenie zdarzenia wyzwalajacego przerwanie (zmiana stanu wyjscia komparatora z 0 na 1)
}

ISR (TIMER1_OVF_vect)
{
	if (!(*flags & _BV(PROLONG)))
	{
		TIMSK1 &= ~_BV(TOIE1);	// disable overflow interrupt for timer 1
		ACSR |= _BV(ACIE);		// enable comparator interrupt
	}

	*flags &= ~_BV(PROLONG);	// set PROLONG flag to 0
}

void timer1_init()
{
	// TCCR1B = (_BV(CS12));	// prescaler 256 (~f = 0.9537 Hz -> T = 1.05 s)
	TCCR1B = (_BV(CS11) | _BV(CS10));	// prescaler 64 (~f = 3.8147 Hz -> T = 0.26 s)
}



int main(void)
{
	pinInit();

	comparator_init();
	timer1_init();

	*flags = 0x00; // flagi ECOMODE i BUSY ustawione na 0;

	sei();

    while (1) 
    {
		*currentPIND = PIND; // get current sensor config
		*currentPINC = PINC; // get current divider config

		if (*flags & _BV(ECOMODE))
		{
			if (!(*currentPINC & 0x02)) // if piston is moving out
			{
				if (!(*currentPIND & 0x08)) PORTC = 0b11111001; // 3rd sensor active: dividers A and Eco active
				else if (!(*currentPIND & 0x04)) PORTC = 0b11110111; // 4th sensor active: only divider B active
			}
			else if (!(*currentPINC & 0x08)) // if piston is moving in
			{
				if (!(*currentPIND & 0x10)) PORTC = 0b11110011; // 2nd sensor active: dividers B and Eco active
				else if (!(*currentPIND & 0x20)) PORTC = 0b11111101; // 1st sensor active: only divider A active
			}
		}
		else
		{
			if (!(*currentPINC & 0x02)) // if piston is moving out
			{
				if (!(*currentPIND & 0x04)) PORTC = 0b11110111; // 4th sensor active: only divider B active
			}
			else if (!(*currentPINC & 0x08)) // if piston is moving in
			{
				if (!(*currentPIND & 0x20)) PORTC = 0b11111101; // 1st sensor active: only divider A active
			}
		}
    }
}


