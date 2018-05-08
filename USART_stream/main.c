/*
 * stream_custom.c
 *
 * Created: 27.08.2017 15:38:04
 * Author : Stefoo
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
  #define F_CPU 16000000UL


 int get(FILE *stream)
 {
	uint8_t c;
	while ((c = cb_Read(&recBuf)) == 0);
	return c;
 }

 int put(char c, FILE *stream)
 {
	 cb_Add(&sendBuf, c);
	 ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { if(!TxFlag) USART_TX_vect(); }
	 return 0;
 }

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
	 uart_9600();	// Ustal szybkosc transferu na 9600 bps
	 UCSR0B=_BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0) | _BV(TXCIE0);
					// Odblokuj nadajnik i odbiornik oraz przerwania
	 UCSR0C=_BV(UCSZ00) | _BV(UCSZ01);	// 8 bitów danych + 1 bit stopu
 }

 int main(void)
 {
	 USART_init();
	 sei();
	 
	 static FILE usartout = FDEV_SETUP_STREAM (put, get, _FDEV_SETUP_RW);
	 
	 while(1)
	 {
		 fgetc(&usartout);  //Zaczekaj na klawisz
		 fprintf(&usartout, "Oto strumien\r\n");
	 }
 }