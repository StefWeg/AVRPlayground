#include <avr\io.h>
#include <avr\pgmspace.h>
#include <avr\interrupt.h>
#include <stdlib.h>
#include <stdbool.h>
#include "switch.h"
#include "defines.h"

void SwitchInit()	// switche podlaczone do PD5-PD7
{
	SET(PORT, SWITCH_UP);
	SET(PORT, SWITCH_DOWN);
	SET(PORT, SWITCH_CHOOSE);
}

static inline void ReadSwitches();
static uint8_t switchesState = 0;

void Timer0Init()
{
	TCCR0B = (_BV(CS01) | _BC(CS00));	// Preskaler CLKIO/64 (T = 1.024 ms)
	TIMSK0 |= _BV(TOIE0);				// Odblokuj przerwanie nadmiaru timera 0 (tryb normalny)
}

void ReadSwitches()
{
	uint8_t readout = (SWITCH_PIN & 0xE0);
	static uint8_t last = readout;
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
	if(switchesState & 0x01) {return true;}
	else {return false;}
}
bool downClick()
{
	if(switchesState & 0x02) {return true;}
	else {return false;}
}
bool chooseClick()
{
	if(switchesState & 0x04) {return true;}
	else {return false;}
}

