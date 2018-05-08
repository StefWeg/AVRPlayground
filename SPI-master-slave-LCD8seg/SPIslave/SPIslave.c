/*
 * SPIslave.c
 *
 *  Author: Stefoo
 */ 


#include <avr\io.h>
#include <avr\interrupt.h>
#include "LM35.h"

uint8_t byteno;	// domyslnie = 0 (zmienna globalna)

ISR(SPI_STC_vect)
{
	static uint16_t temperature;
	
	if(byteno == 0) temperature = GetTemperature(SPDR);
	// Jezeli odbierany jest pierwszy bajt - odczytaj temperature z kanalu o numerze otrzymanym od mastera
	SPDR = ((uint8_t*)&temperature)[byteno];
	// Zapisz do rejestru SPI Data Register starszy bajt zmiennej 'temperature'
	// Wartosc zostanie przeslana do mastera przy kolejnej transakcji,
	// po ktorej ponownie wywolane zostanie to przerwanie
	byteno = (byteno+1) % 3;
	// Powieksz numer bajtu o jeden tak, by przy nastepnym przerwaniu
	// do rejestru SPI Data Resiter zapisany zostal mlodszy bajt zmiennej 'temperature'
	// UWAGA: dzielenie modulo 3 powoduje, ze nadaniu mlodszego bajtu byteno = 2, a nie 0
	// (nie spowoduje to proby odczytania wartosci spoza zmiennej 'temperature',
	// bo master zakonczy wczesniej transmisje)
}

ISR(PCINT0_vect)
{
	byteno = 0;
}

void SPI_slave_init()
{
	DDRB |= _BV(PB4);	// Pin MISO jako wyjscie
	SPCR = _BV(SPE) | _BV(SPIE);	// Tryb slave, CLK/4, przerwania
	SPSR;
	SPDR;	// Skasuj flage SPIF (odczyt SPI Status Register oraz SPI Data Register)

	PCMSK0 |= _BV(PCINT2); // Pin Change Mask Register: reagowanie na zmiane stanu pinu PCINT2 [PB2], czyli SS
	PCICR |= _BV(PCIE0); // Odblokuj przerwanie od zmiany stanu na pinach PCINT[0:7] (z uwzglednieniem maski)
}

int main()
{
	SPI_slave_init();
	sei();
	ADC_init();
	while(1);
}
