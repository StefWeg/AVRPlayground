/*
 * EEPROM_playgoround.c
 *
 * Created: 01.07.2017 13:13:04
 * Author : Stefoo
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>


EEMEM int variable = 0x0F; // initialization not working
uint16_t variable2 __attribute__ ((section(".eeprom"))) = 0x0A;

EEMEM char tekst[] = "Text in EEPROM";

typedef struct { int x; int y; } xy;
EEMEM xy punkt = {12, 20};


int main(void)
{
	int second = -3;
	unsigned first = 36345;

	xy internal_pkt = {25,50};
	
	char internal_str[15] = "Text in EEPROM";

	// writing

	// eeprom_is_redy(); // returns true is no writing to EEPROM is conducted
	eeprom_busy_wait(); // waiting for EEPROM to be ready

	eeprom_write_block(internal_str,(uint8_t*)&tekst,15); // internally waiting for eeprom to be ready

	eeprom_write_word((uint16_t*)&variable2,(uint16_t)second);
	
	eeprom_write_word((uint16_t*)&variable,(uint16_t)first);

	eeprom_update_block(&internal_pkt, &punkt, sizeof(xy)); // starts from comparing values in memory & argument

	// reading

	second = (int)eeprom_read_word(&variable2);
	first = (unsigned)eeprom_read_word((uint16_t*)&variable);

	internal_pkt.x = 1;
	internal_pkt.y = 2;
	eeprom_read_block(&internal_pkt, &punkt, sizeof(xy));

	// stop

	int stop = 0;

    while (1) 
    {}
}

