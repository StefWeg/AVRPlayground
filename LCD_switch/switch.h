/*
 * switch.h
 *
 * Created: 04.08.2017 22:12:46
 *  Author: Stefoo
 */ 


#ifndef _SWITCH_H
#define _SWITCH_H

#include <stddef.h>
#include <stdbool.h>

void SwitchInit();
void Timer0Init();
void ReadSwitches();

bool upClick();
bool downClick();
bool chooseClick();

#endif
