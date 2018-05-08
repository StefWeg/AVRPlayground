/*
 * master.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/crc16.h>
#include <string.h>

#include <util/delay.h>

#define BUF_MAX 10
#define UART_REC_ERROR 3

static void uart_9600()
{
	#define BAUD 9600
	#include <util/setbaud.h>
	UBRR0H=UBRRH_VALUE;
	UBRR0L=UBRRL_VALUE;
	#if USE_2X
	UCSR0A|=_BV(U2X0);
	#else
	UCSR0A&=~(_BV(U2X0));
	#endif
}

void UART_master_init()
{
	uart_9600();
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);	// 8 bitów danych, 2 bity stopu, bez parzystosci
	
	DDRD |= _BV(PD4);		// Uklad generuje zegar dla slave
	UCSR0C |= _BV(UMSEL00);	// Tryb synchroniczny
}

void uart_sendchar(uint8_t c)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

void uart_Send_Frame(char *buf, uint8_t n)
{
	uint8_t crc = _crc_ibutton_update(0, n);
	uart_sendchar(n);
	while(n--)
	{
		uart_sendchar(*buf);
		crc = _crc_ibutton_update(crc, *buf);
		buf++;
	}
	uart_sendchar(crc);
}

int main()
{
	char bufor[BUF_MAX];
	uint16_t licznik = 0;

	UART_master_init();
	
	while(1)
	{
		_delay_ms(1000);
		sprintf(bufor,"%04d",licznik);	// ?a?cuch wynikowy sk?ada si? z 4 cyfr i jest dope?niany z lewej zerami
		for(uint8_t i = 0; i < 4; i++) bufor[i] -= '0';	// konwersja znak_ASCII -> cyfra
		uart_Send_Frame(bufor, 4);
		licznik++;
	}
}
