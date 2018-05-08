/*
 * LCDAlfaAnim.c
 *
 *  Author: Stefoo
 */ 


#include <stdbool.h>
#include <stdint.h>
#include "defines.h"
#include "hd44780.h"
#include <avr\pgmspace.h>
#include <util\delay.h>

void lcd_init()
{

	hd44780_init();				//Podstawowa inicjalizacja modu³u
	hd44780_outcmd(HD44780_CLR);	//Wyczyœæ pamiêæ DDRAM
	hd44780_wait_ready(1000);
	hd44780_outcmd(HD44780_ENTMODE(1, 0));	//Tryb autoinkrementacji AC
	hd44780_wait_ready(1000);
	hd44780_outcmd(HD44780_DISPCTL(1, 0, 0));	//W³¹cz wyœwietlacz, wy³¹cz kursor
	hd44780_wait_ready(1000);
}

void lcd_putchar(char c)
{
	static bool second_nl_seen;
	static uint8_t line=0;
	
	if ((second_nl_seen) && (c != '\n')&&(line==0))
	{//Odebrano pierwszy znak
		hd44780_wait_ready(40);
		hd44780_outcmd(HD44780_CLR);
		hd44780_wait_ready(1600);
		second_nl_seen=false;
	}
	if (c == '\n')
	{
		if (line==0)
		{
			line++;
			hd44780_outcmd(HD44780_DDADDR(64));	//Adres pierwszego znaku drugiej linii
			hd44780_wait_ready(1000);
		}
		else
		{
			second_nl_seen=true;
			line=0;
		}
	}
	else
	{
		hd44780_outdata(c);
		hd44780_wait_ready(40);
	}
}

void lcd_puttext_P(const char __flash *txt)
{
	char ch;
	while((ch=*txt))
	{
		lcd_putchar(ch);
		txt++;
	}
}

void lcd_puttext(const char __memx *txt)
{
	char ch;
	while((ch=*txt))
	{
		lcd_putchar(ch);
		txt++;
	}
}

void lcd_goto(uint8_t x, uint8_t y)
{
 hd44780_outcmd(HD44780_DDADDR(0x40*y+x));
 hd44780_wait_ready(1000);
}

void lcd_cls()
{
 hd44780_outcmd(HD44780_CLR);
 hd44780_wait_ready(false);
}

void lcd_defchar(uint8_t charno, const uint8_t __memx *chardef)
{
	hd44780_outcmd(HD44780_CGADDR(charno*8));
	hd44780_wait_ready(40);
	for(uint8_t c=0;c<8;c++)
	{
		hd44780_outdata(*chardef++);
		hd44780_wait_ready(40);
	}
}

void lcd_box(uint8_t y)
{
	hd44780_outcmd(HD44780_CGADDR(0)); // Zaczynamy od znaku o kodzie 0
	hd44780_wait_ready(40);
	for(uint8_t c=0;c<y;c++) // Stwórz linie z wlaczonymi pikselami
	{
		hd44780_outdata(0xFF);
		hd44780_wait_ready(40);
	}
	for(uint8_t c=y;c<8;c++)   // Stwórz linie z wylaczonymi pikselami
	{
		hd44780_outdata(0x00);
		hd44780_wait_ready(40);
	}
}

int main()
{
 lcd_init();
 for (uint8_t i = 0; i < 16; i++) lcd_putchar(0);

 while(1)
 {
	 for(uint8_t x=0;x<8;x++)
	 {
		 lcd_box(x);
		 _delay_ms(100);
	 }
	 for(int8_t x=7;x>=0;x--)
	 {
		 lcd_box(x);
		 _delay_ms(100);
	 }
 }
}


