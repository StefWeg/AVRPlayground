/*
 * bufusart.c
 *
 * Created: 27.08.2017 15:45:12
 *  Author: Stefoo
 */ 


 #include "bufusart.h"
 #include "usart.h"
 #include <avr/pgmspace.h>
 #include <util/atomic.h>

 CircBuffer recBuf, sendBuf;
 volatile bool TxFlag=false;

 ISR(USART_RX_vect)
 {
	 uint8_t ch=UDR0;
	 if(ch=='\r')
	 {
		 ch=0;
	 }
	 cb_Add(&recBuf, ch);
 }

 ISR(USART_TX_vect)
 {
	 if(!cb_IsEmpty(&sendBuf))
	 {
		 UDR0=cb_Read(&sendBuf);
		 TxFlag=true;
	 } else TxFlag=false;
 }

 void USART_send_buf(CircBuffer *buf, const char __memx *txt)
 {
	 uint8_t ch;
	 while ((ch=*txt++)!=0) cb_Add(&sendBuf, ch);
	 ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	 {
		 if(!TxFlag) USART_TX_vect();
	 }
 }
