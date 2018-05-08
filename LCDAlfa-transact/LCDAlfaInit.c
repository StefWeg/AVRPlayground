/*
 * LCDAlfa_transact.c
 *
 *  Author: Stefoo
 */ 


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "hd44780.h"
#include <avr\pgmspace.h>
#include <util\delay.h>
#include <avr/interrupt.h>
#include "Alloc_safe.h"
#include "LCD_trans.h"

int main(void)
{
	LCD_Init();
	LCD_Timer_init();
	sei();
	
	LCD_PutText(0,0, "Numer:");
	
	uint16_t cnt=0;
	LCD_trans *buf=malloc_re(sizeof(LCD_trans) + 7);
	char bufstr[6];
	
	while(1)
	{
		utoa(cnt++, bufstr, 10);
		while(strlen(bufstr)<6) strcat(bufstr, " ");
		LCD_PutText_B(6,0, bufstr, buf, false);
		while(!LCD_IsTransCompleted(buf));
	}
}


