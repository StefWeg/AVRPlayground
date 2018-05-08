/*
 * EEPROM_int.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

uint8_t *EEPROMPtr;                  //Wskaünik do zapisywanego bloku
uint8_t *SRAMPtr;                    //Wskaünik do danych w SRAM
volatile uint16_t EEPROMWrBytes;     //Liczba bajtÛw do zapisu

ISR(EE_READY_vect)
{
	eeprom_write_byte(EEPROMPtr, *SRAMPtr);
	EEPROMPtr++; SRAMPtr++;
	EEPROMWrBytes--;
	if(EEPROMWrBytes) EECR|=_BV(EERIE); //Wlacz przerwania EEPROM
}

void EEPROM_write_int(uint8_t *src, uint8_t *dst, uint16_t size)
{
	while(EEPROMWrBytes); //Czekaj na zakonczenie poprzednich operacji
	EEPROMPtr=dst;
	SRAMPtr=src;
	EEPROMWrBytes=size-1;
	eeprom_write_byte(EEPROMPtr, *SRAMPtr);
	EEPROMPtr++; SRAMPtr++;
	EECR|=_BV(EERIE); //Wlacz przerwania EEPROM
}

char napis[12] EEMEM;

int main(void)
{
	sei();
	char first[12] = "Test EEPROM";
	char second[11] = "Nowy zapis";
	EEPROM_write_int((uint8_t*)first, (uint8_t*)napis, 12);
	EEPROM_write_int((uint8_t*)second, (uint8_t*)napis, 11);
    while(1)
    {
        //TODO:: Please write your application code 
    }
}
