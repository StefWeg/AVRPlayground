#ifndef _MULTI_H_
#define _MULTI_H_

#define LED	B
#define LEDDISPNO	4

extern volatile uint8_t LEDDIGITS[LEDDISPNO];
void Timer0Init();

#endif
