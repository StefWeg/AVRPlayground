/*
 * LCD_switch.cpp
 *
 * Created: 04.08.2017 22:03:33
 * Author : Stefoo
 */ 

#include <avr/interrupt.h>
#include <util/delay.h>
#include "menu.h"
#include "LCD.h"
#include "switch.h"

ISR(TIMER0_OVF_vect)
{
	ReadSwitches();
}

int main()
{

	SwitchInit();
	Timer0Init();  // Obsluga przyciskow
	lcd_init();
	Menu_Show();
	sei();

	while(1)
	{
		if(upClick()) {Menu_SelectPrev();}
		else if(downClick()) {Menu_SelectNext();}
		else if(chooseClick()) {Menu_Click();}
		_delay_ms(200);
	}

}


