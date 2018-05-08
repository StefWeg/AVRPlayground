/*
 * SPIslave.c
 *
 *  Author: Stefoo
 */ 


#include <avr\io.h>
#include <avr\interrupt.h>
#include "LM35.h"

uint8_t byteno;

ISR(SPI_STC_vect)
{
	static uint16_t temperature;
	
	if(byteno==0) temperature=GetTemperature(SPDR);
	SPDR=((uint8_t*)&temperature)[byteno];
	byteno=(byteno+1) % 3;
}

ISR(PCINT0_vect)
{
	byteno=0;
}

void SPI_slave_init()
{
	DDRB|=_BV(PB4);	//Pin MISO jako wyjœcie
	SPCR=_BV(SPE) | _BV(SPIE); //Tryb master, CLK/4, przerwania
	SPSR;
	SPDR; //Skasuj flagê SPIF
	PCMSK0|=_BV(PCINT2); //Odblokuj przerwanie PCINT2
	PCICR|=_BV(PCIE0);
}

int main()
{
	SPI_slave_init();
	sei();
	ADC_init();
	while(1);
}
