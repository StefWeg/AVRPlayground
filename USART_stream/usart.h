/*
 * usart.h
 *
 * Created: 27.08.2017 15:41:01
 *  Author: Stefoo
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

void USART_putchar(char ch);                                           //Wyœlij znak
void USART_send(const char __memx *txt);                               //Wyœlij ³añcuch w formacie NULLZ
void USART_send_block(const uint8_t __memx *block, uint8_t size);      //Wyœlij blok o d³ugoœci size z pamiêci SRAM

static inline void waitforTx()            //Zaczekaj na koniec nadawania znaku
{
	while(!(UCSR0A & _BV(UDRE0)));
}

#endif /* USART_H_ */