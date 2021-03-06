/*
 * LCD.h
 *
 * Created: 04.08.2017 22:05:55
 *  Author: Stefoo
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr\pgmspace.h>

void lcd_init();
void lcd_putchar(char c);
void lcd_puttext(const char __memx *txt);
void lcd_defchar_P(uint8_t charno, const uint8_t *chardef);
void lcd_goto(uint8_t x, uint8_t y);
void lcd_cls();

#endif /* LCD_H_ */