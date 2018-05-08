/*
 * usart.c
 *
 * Created: 2013-01-17 21:39:25
 *  Author: tmf
 */ 

#include "usart.h"
#include <avr/pgmspace.h>


void USART_putchar(char ch)
{
	while(!(UCSR0A & _BV(UDRE0)));  //Zaczekaj na miejsce w buforze nadawczym
	UDR0=ch;
}

void USART_send(const char __memx *txt)
{
	while(*txt)
	{
		USART_putchar(*txt);
		++txt;
	}
}

void USART_send_block(const uint8_t __memx *block, uint8_t size)
{
	while(size--)
	{
		USART_putchar(*block);
		++block;
	}
}
