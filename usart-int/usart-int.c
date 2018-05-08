/*
 * usart_int.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "bufusart.h"
#include "RingBuffer.h"

#define MAX_CMD_LEN 10      // Maksymalna dlugosc polecenia w bajtach

static void uart_9600()
{
	#define BAUD 9600
	#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
}

void USART_init()
{
	uart_9600();  //Ustal szybkoœæ transferu na 9600 bps
	UCSR0C=_BV(UCSZ00) | _BV(UCSZ01); //8 bitów danych + 1 bit stopu
	UCSR0B=_BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0) | _BV(TXCIE0);  //Odblokuj nadajnik i odbiornik oraz przerwania
}

const char __flash cmd1[] = {"nazwa"};
const char __flash cmd2[] = {"FLASH"};
const char __flash * const __flash cmds[] = {cmd1, cmd2};

bool GetToken(char txt[MAX_CMD_LEN], uint8_t len)
{
	char ch;
	for(uint8_t i=0; i<len; i++)        // Przepisz token do zmiennej txt w formacie NULLZ
	{
		if(cb_IsEmpty(&recBuf)) return false;	// Blad
		ch=cb_Read(&recBuf);
		if(ch==' ') ch=0;
		txt[i]=ch;
		if(ch==0) break;
	}
	return true;
}

int32_t GetHEXArg()
{
	char txt[MAX_CMD_LEN];
	if(GetToken(txt, MAX_CMD_LEN)==false) return -1;
	return strtol(txt, NULL, 16);   // Przekonwertuj argument hex na liczbe
}

uint8_t TranslateCommand()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   //Licznik odebranych komend
	{
		if (cmdrec) --cmdrec;
	}
	
	uint8_t indeks;
	char txt[MAX_CMD_LEN];
    if (GetToken(txt, MAX_CMD_LEN) == false) return -1;  // Wystapil blad
	
	for(indeks=0; indeks<sizeof(cmds)/sizeof(cmds[0]); indeks++)  // Porównaj token z lista
		if (strcmp_P(txt, cmds[indeks])==0) break;
	
	return indeks;
}

void SendFLASH(int32_t start, int32_t end)
{
	if((start==-1) || (end==-1)) 
	{
		USART_send_buf(&sendBuf, PSTR("Blednie okreslone adresy pamieci\r\n"));
		return;
	}
	uint8_t byte;
	char txt[5];
	bool ret;
	
	for(int32_t indeks=start; indeks<end; indeks++)
	{
		byte=pgm_read_byte(indeks);
		sprintf(txt, "%02x, ", byte);
		for(uint8_t x=0; x<4; x++)
		{
			if(txt[x]==0) break;
			while((ret=cb_Add(&sendBuf, txt[x]))==false);
		}
	}

	while((ret=cb_Add(&sendBuf, '\r'))==false);
	while((ret=cb_Add(&sendBuf, '\n'))==false);
	// ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { if(!TxFlag) USART_TX_vect(); }
}

int main(void)
{
	USART_init();
	sei();
	
	while(1)
	{
		if(cmdrec)
		{
			switch(TranslateCommand())
			{
				case 0 : USART_send_buf(&sendBuf, PSTR("Nazywam sie ATMega328P\r\n")); break;
				case 1 : USART_send_buf(&sendBuf, PSTR("Zawartosc pamieci FLASH:\r\n"));
				         SendFLASH(GetHEXArg(), GetHEXArg());
				         break;
				default: USART_send_buf(&sendBuf, PSTR("Nieznane polecenie\r\n"));
			}
		}
	}
}
