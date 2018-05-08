/*
 * MyMiernik.c
 *
 * Created: 19.07.2017 17:30:58
 * Author : Stefoo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>


void timer_init ()
{
	// SYGNAL GENEROWANY
	// TIMER0 (8-bitowy) - wyjscie OC0A (PIN PD6)
	TCCR0A = (_BV(COM0A1) | _BV(WGM01) | _BV(WGM00)); // Wyzerowanie pinu po zdarzeniu Compare Match,
													// ustawienie po osi?gni?ciu przez licznik warto?ci minimalnej
													// FastPWM
	TCCR0B = _BV(CS02); // Preskaler 256 (czetotliwosc: 16000000/256/256 ~= 244.14 Hz, okres: 65.53 tys. taktow zegara)
	OCR0A = 80; // Compare Match = 180(/255)
	DDRD |= _BV(PD6); // PD6 jako wyjscie

	// MIERNIK SYGNALU
	// TIMER1 (16-bitowy) - wejscie Input Capture Pin (ICP1 = PB0)
	TCCR1B = _BV(ICNC1) | _BV(ICES1);  // Wlacz filtrowanie ICP, zdarzenie na zboczu narastajacym
	TCCR1B |= _BV(CS11) | _BV(CS10);   // Preskaler 64, tryb normalny (OC1A/OC1B disconnected)
										// (probkowanie: 16000000/64/65536 ~= 3.81 Hz, okres: 4.19 mln taktow zegara)
	TIMSK1 = _BV(ICIE1) | _BV(TOIE1);  // Przerwania ICP i overflow
}




volatile uint8_t isOk;

ISR(TIMER1_OVF_vect)
{
	isOk=0;  // Blad przepelnienia licznika
}

volatile uint16_t timestamp;
volatile uint16_t period;

ISR(TIMER1_CAPT_vect)
{
	static uint16_t loctimestamp;

	if(TCCR1B & _BV(ICES1)) // Reakcja na zbocze narastajace
	{
		GTCCR = _BV(PSRSYNC); // Zerowanie prescalera
		TCNT1 = 0; // Zerowanie timera
		TCCR1B &= ~_BV(ICES1); // Nastepne zdarzenie na zboczu opadajacym
		period = ICR1;
		timestamp = loctimestamp;
		isOk=255;
	} else // Reakcja na zbocze opadajace
	{
		TCCR1B = TCCR1B | _BV(ICES1); // Nastepne zdarzenie na zboczu narastajacym
		loctimestamp = ICR1;
	}
}




int main(void)
{
    uint16_t ts, pr;
    uint8_t ok;

	uint16_t dutyCycle;
	uint16_t periodHz;

    timer_init();
    sei();

    while(1)
    {
	    ATOMIC_BLOCK(ATOMIC_FORCEON)
	    {
		    ok=isOk;
		    ts=timestamp;
		    pr=period;
	    }
	    if(ok == 0)
	    {
		    ts=0;
			period=0;
	    }

	    if(ts) dutyCycle = 100*ts/pr;
		if(pr) periodHz = 16000000/64/pr;
    }
}

