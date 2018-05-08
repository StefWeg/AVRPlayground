/*
 * bufusart.h
 *
 * Created: 2013-01-24 18:12:26
 *  Author: tmf
 */ 


#ifndef BUFUSART_H_
#define BUFUSART_H_

#include "RingBuffer.h"

extern CircBuffer recBuf;
extern CircBuffer sendBuf;

extern volatile uint8_t cmdrec;
extern volatile bool TxFlag;

void USART_send_buf(CircBuffer *buf, const char __memx *txt);


#endif /* BUFUSART_H_ */