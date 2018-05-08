/*
 * usart.c
 *
 * Created: 27.08.2017 15:40:43
 *  Author: Stefoo
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
