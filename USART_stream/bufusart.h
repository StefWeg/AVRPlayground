/*
 * bufusart.h
 *
 * Created: 27.08.2017 15:45:26
 *  Author: Stefoo
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