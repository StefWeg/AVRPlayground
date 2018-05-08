/*
 * menudef.c
 *
 * Created: 2013-08-20 13:05:36
 *  Author: tmf
 */

#include "menu.h"
#include "LCD.h"
#include <util/delay.h>
#include <avr\pgmspace.h>

//Prototypy funkcji obslugi wybranej pozycji menu
void menufunc1();
void menufunc2();
void menufunc3();
void menufunc4();
void menufunc5();
void menufunc6();
void menufunc7();

#define PGM_STR(X) ((const __flash char[]) { X })

struct _menuitem const __flash menu;
struct _menuitem const __flash menuA1;
struct _menuitem const __flash menuB1;

struct _menuitem const __flash menuB3 = {PGM_STR("<z powrotem>"), Menu_Back, &menuB1, 0, 0};
struct _menuitem const __flash menuB2 = {PGM_STR("Podmenu B2"), menufunc7, &menuB1, 0, &menuB3};
struct _menuitem const __flash menuB1 = {PGM_STR("Podmenu B1"), menufunc6, &menuA1, 0, &menuB2};

struct _menuitem const __flash menuA4 = {PGM_STR("<z powrotem>"), Menu_Back, &menuA1, 0, 0};
struct _menuitem const __flash menuA3 = {PGM_STR("Podmenu A3"), menufunc5, &menuA1, 0, &menuA4};
struct _menuitem const __flash menuA2 = {PGM_STR("Podmenu A2"), 0, &menuA1, &menuB1, &menuA3};
struct _menuitem const __flash menuA1 = {PGM_STR("Podmenu A1"), menufunc4, &menu, 0, &menuA2};

struct _menuitem const __flash menu4 = {PGM_STR("Menu5"), 0, &menu, 0, 0};
struct _menuitem const __flash menu3 = {PGM_STR("Menu4"), menufunc3, &menu, 0, &menu4};
struct _menuitem const __flash menu2 = {PGM_STR("Menu3"), menufunc2, &menu, 0, &menu3};
struct _menuitem const __flash menu1 = {PGM_STR("Menu2"), 0, &menu, &menuA1, &menu2};

const __flash struct _menuitem const __flash menu = {PGM_STR("Menu1"), menufunc1, 0, 0, &menu1};

void menufunc1()
{
	lcd_cls();
	lcd_puttext(PSTR("Menu 1"));
	_delay_ms(2000);
}

void menufunc2()
{
	lcd_cls();
	lcd_puttext(PSTR("Menu 3"));
	_delay_ms(2000);
}

void menufunc3()
{
	lcd_cls();
	lcd_puttext(PSTR("Menu 4"));
	_delay_ms(2000);
}

void menufunc4()
{
	lcd_cls();
	lcd_puttext(PSTR("Podmenu A1"));
	_delay_ms(2000);
}

void menufunc5()
{
	lcd_cls();
	lcd_puttext(PSTR("Podmenu A3"));
	_delay_ms(2000);
}

void menufunc6()
{
	lcd_cls();
	lcd_puttext(PSTR("Podmenu B1"));
	_delay_ms(2000);
}

void menufunc7()
{
	lcd_cls();
	lcd_puttext(PSTR("Podmenu B2"));
	_delay_ms(2000);
}

