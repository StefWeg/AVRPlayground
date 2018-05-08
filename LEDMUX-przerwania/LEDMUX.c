/*
 * LEDMUX.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>


#define LED	D

#define GLUE(a, b)     a##b

#define LEDPORT1(s)	GLUE(PORT,s)
#define LEDPORT LEDPORT1(LED)
#define LEDDDR1(s)	GLUE(DDR,s)
#define LEDDDR	LEDDDR1(LED)


// -----------------------------


#define PINCOL (((PINC &= _BV(4)) >> 4) | ((PINC &= _BV(5)) >> 4) | ((PINB &= _BV(4)) >> 2))


// -----------------------------


//Cyfry 0,1,2,3,4,5,6,7,8,9 i blank
static const uint8_t __flash DIGITS[11]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00};
const uint8_t DP = 0x80;

static inline void ShowOnLED(uint8_t val)
{
	uint8_t tmp=0xFF;   // (dla x == 11 zostaje)
	if((val & 0x7F)<11) tmp = DIGITS[val & 0x7F];  // Odczytaj kod znaku
	if(val & DP) tmp = 0xFF;      // Sterowanie kropka dziesiestna na LED
	LEDPORT=tmp;
}


// -----------------------------


#define LEDDISPNO	4          //Liczba wyswietlaczy

volatile uint8_t LEDDIGITS[LEDDISPNO];  //Tablica zawierajaca dane do wyswietlania

ISR(TIMER0_OVF_vect)
{
	static uint8_t LEDNO; // Zmianna statyczna domyslnie inicjalizowana jako 0

	PORTC &= 0xF0;		//Wylacz wszystkie wywietlacze
	LEDNO = (LEDNO+1)%LEDDISPNO;
	ShowOnLED(LEDDIGITS[LEDNO]);
	// PORTC = (PORTC & 0xF0) | (~(1<<LEDNO) & 0x0F);	//Wybierz kolejny wyswietlacz
	PORTC = (PORTC & 0xF0) | ((1<<LEDNO) & 0x0F);	//Wybierz kolejny wyswietlacz
}

ISR(TIMER0_COMPA_vect)
{
	PORTC &= 0xF0;	//Wylacz wszystkie wywietlacze
}

// OPTIMIZED
/* ISR(TIMER0_COMPA_vect, ISR_NAKED)
{
	PORTC &= 0xFE;	//Wylacz wszystkie wywietlacze
	PORTC &= 0xFD;
	PORTC &= 0xFB;
	PORTC &= 0xF7;
	asm volatile("RETI\n\t"::);
} */

void Timer0Init()
{
	TCCR0B = _BV(CS01);	//Preskaler CLKIO/8
	TIMSK0 |= _BV(TOIE0);	//Odblokuj przerwanie nadmiaru timera 0
}

void Timer0InitWithDimmer()
{
	TIMSK0|=_BV(OCIE0A);		//W³¹cz przerwanie Compare Match A
	OCR0A=255;
	Timer0Init();
}

// -----------------------------


void scankbd()
{
	uint8_t y = 0;
	uint8_t x;
	while (y < 4)					// Sa 4 rzedy
	{
		x = PORTB;
		x = (x | 0x0F) ^ (1 << y);	// Wybierz aktywny rzad, nie zmieniajac stanu innych pinów portu
		PORTB = x;
		asm volatile ("nop");		// Synchronizator (mozna odczytac dopiero po odczekaniu jednego cyklu zegara po zapisie)
		x = PINCOL;
		if(x != 0x07)		// Jakis klawisz byl wcisniety?
		{
			if((x & 0x01) == 0) x=0;	// Jesli tak, to z której kolumny?
			else if((x & 0x02) == 0) x=1;
			else x=2;
			x=x+y*3+1;
			if (x == 10) x = 0x80;		//Oblicz numer klawisza
			else if (x == 11) x = 0;
			else if (x == 12) x = 0x80;
			
			LEDDIGITS[3] = x;
			return;
		}
		y++;
	}
	return;
}



int main()
{
	// Sygnaly ustawiajace wskazywana liczbe
	LEDDDR = 0xFF;		// Wszystkie piny portu sa wyjsciem PD0-PD7

	// Sygnaly odpytujace rzedy klawiatury + wejscie skanujace stan kolumny (3)
	DDRB = 0x0F;		// 4 rzedy sa wyjsciami PB0-PB3 + kolumna jest wejsciem PB4
	PORTB |= 0x1F;		// Ustaw rzedy w stanie 1 PB0-PB3 + Wlacz pull up na kolumnie PB4

	// Wyjscia ustawiajace wyswietlana cyfre + 2 wejscia skanujace stan kolumn (1 i 2)
	DDRC = 0x08;		// 4 wyjscia steruja wyswietlana cyfra PC0-PC3 + 2 kolumny sa wejsciami PC4-PC5
	PORTC = 0x30;		// Ustaw stan niski na wyjsciach sterujacych PC0-PC3 + Wlacz pull upy na kolumnach PC4-PC5

	Timer0InitWithDimmer();
	//Timer0Init();
	sei();

	LEDDIGITS[0]=1;
	LEDDIGITS[1]=2;
	LEDDIGITS[2]=3;
	LEDDIGITS[3]=4;

	while(1) {
		// scankbd();

		OCR0A=200;
		_delay_ms(1000);
		OCR0A=100;
		_delay_ms(1000);
	}
}
