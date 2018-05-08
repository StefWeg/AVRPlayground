/*
 * main.c
 *
 * Created: 04.08.2017 22:06:36
 *  Author: Stefoo
 */ 

 #include <avr/interrupt.h>
 #define F_CPU 16000000UL
 #include <util/delay.h>
 #include "menu.h"
 #include "LCD.h"
 #include "switch.h"

 #include <util\atomic.h>
 //#include <stdbool.h>


 ISR(TIMER0_OVF_vect)
 {
	 ReadSwitches();
 }

 int main()
 {
	 //SwitchInit();
	 lcd_init();
	 //Timer0Init();  // Obsluga przyciskow
	 Menu_Show();
	 //sei();

	 DDRB = 0x01;

	 while(1)
	 {
	/*	if(upClick())
		{
			PORTB = 0x01;
			// cli();
			//TIMSK0 = 0;
			Menu_SelectPrev();
			// sei();
			//TIMSK0 |= _BV(TOIE0);
		}
		else PORTB = 0x00;

		if(downClick())
		{
			PORTB = 0x01;
			Menu_SelectNext();
		}
		else  PORTB = 0x00;
		
		if(chooseClick())
		{
			PORTB = 0x01;
			Menu_Click();
		}
		else PORTB = 0x00;*/

		_delay_ms(1000);
		Menu_SelectNext();
		PORTB = 0x01;
	 }

 }
