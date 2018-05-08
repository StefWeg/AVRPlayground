/*
 * FLASH_playground.c
 *
 * Created: 03.07.2017 17:24:54
 * Author : Stefoo
 */ 

#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

const char txt[] PROGMEM = "ABC";

int main(void)
{
	const char __ATTR_PROGMEM__ number = '9';

	int stop = 0;

    while (1) 
    {
    }
}

