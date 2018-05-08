/*
 * menu.h
 *
 * Created: 04.08.2017 22:06:08
 *  Author: Stefoo
 */ 


#ifndef _MENU_H
#define _MENU_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*menuitemfuncptr)();

struct _menuitem
{
	const __flash char * const  text;            //Nazwa pozycji menu
	menuitemfuncptr menuitemfunc;                //Funkcja callback zwiazana z pozycja menu
	const __flash struct _menuitem *parent;      //Obiekt - rodzic
	const __flash struct _menuitem *submenu;     //Podmenu, lub NULL jesli nie ma
	const __flash struct _menuitem *next;        //Kolejna pozycja lub NULL jezli nie ma
};

void Menu_Show();
void Menu_SelectNext();
void Menu_SelectPrev();
void Menu_Click();
void Menu_Back();

extern struct _menuitem const __flash menu;		//Struktura menu

#endif
