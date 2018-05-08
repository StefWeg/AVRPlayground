/*
 * switch.c
 *
 * Created: 04.08.2017 22:12:33
 *  Author: Stefoo
 */ 

 #include <avr\io.h>
 #include <avr\pgmspace.h>
 #include <avr\interrupt.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include "switch.h"
 #include "defines.h"
 
 //#include <util\atomic.h>


 void SwitchInit()
 {
	
	 SET(PORT, SWITCH_UP);
	 SET(PORT, SWITCH_DOWN);
	 SET(PORT, SWITCH_CHOOSE);
	 // switche podlaczone do PD5-PD7 (wejscia, PULL-UP)
 }

inline void ReadSwitches();
static volatile uint8_t switchesState = 0x07;

 void Timer0Init()
 {
	 TCCR0B = (_BV(CS01) | _BV(CS00));	// Preskaler CLKIO/64 (T = 1.024 ms)
	 TIMSK0 |= _BV(TOIE0);	// Odblokuj przerwanie nadmiaru timera 0 (tryb normalny)
 }

 void ReadSwitches()
 {
	 uint8_t readout = (SWITCH_PIN & 0xE0);
	 static uint8_t last = 0;
	 static uint8_t counter = 0;

	 if (readout == last)
	 {
		 counter++;
		 if (counter == 4)
		 {
			 switchesState = (readout >> 5);
			 counter = 0;
		 }
	 }
	 else counter = 0;

	 last = readout;
 }

 bool upClick()
 {
	 if(switchesState & 0x01) {return false;}
	 else {return true;}
 }
 bool downClick()
 {
	 if(switchesState & 0x02) {return false;}
	 else {return true;}
 }
 bool chooseClick()
 {
	 if(switchesState & 0x04) {return false;}
	 else {return true;}
 }

