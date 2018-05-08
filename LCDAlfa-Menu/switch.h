#ifndef _SWITCH_H
#define _SWITCH_H

#include <stddef.h>
#include <stdbool.h>

void SwitchInit();
void Timer0Init();
void ReadSwitches()

bool upClick();
bool downClick();
bool chooseClick();

#endif
