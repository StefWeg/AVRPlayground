/*
 * Klawisze.c
 *
 *  Author: Stefoo
 */ 


#include <avr\io.h>
#include <avr/interrupt.h>
#include <util\delay.h>

#define LED	D

#define GLUE(a, b)     a##b

#define LEDPORT1(s)	GLUE(PORT,s)
#define LEDPORT LEDPORT1(LED)
#define LEDDDR1(s)	GLUE(DDR,s)
#define LEDDDR	LEDDDR1(LED)

//Cyfry 0,1,2,3,4,5,6,7,8,9 i symbol -
static const uint8_t __flash DIGITS[11]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0xBF};
const uint8_t DP=0x80;

static inline void ShowOnLED(uint8_t val)
{
	uint8_t tmp=0xFF;
	if((val & 0x7F)<11) tmp=DIGITS[val & 0x7F];  // Odczytaj kod znaku
	if(val & DP) tmp&=~DP;      // Sterowanie kropka dziesiêtna na LED
	LEDPORT=tmp;
}


#define LEDDISPNO	4          // Liczba wyœwietlaczy

volatile uint8_t LEDDIGITS[LEDDISPNO];  // Tablica zawieraj¹ca dane do wyœwietlania

static inline void ReadButtons();       // Prototyp funkcji odczytu klawiszy

ISR(TIMER0_OVF_vect)
{
	static uint8_t LEDNO;

	PORTC|=0x0F;		// Wylacz wszystkie wywietlacze
	LEDNO=(LEDNO+1)%LEDDISPNO;
	ShowOnLED(LEDDIGITS[LEDNO]);
	PORTC=(PORTC & 0xF0) | (~(1<<LEDNO) & 0x0F);	// Wybierz kolejny wywietlacz
	ReadButtons();    // Odczytaj stan klawiszy
}

/*ISR(TIMER0_COMPA_vect)
{
	PORTC|=0x0F;	// Wylacz wszystkie wywietlacze
}*/

ISR(TIMER0_COMPA_vect, ISR_NAKED)
{
	PORTC|=0x01;	// Wylacz wszystkie wywietlacze
	PORTC|=0x02;
	PORTC|=0x04;
	PORTC|=0x08;
	asm volatile("RETI\n\t"::);
}

void Timer0Init()
{
	TCCR0B=_BV(CS01);	//Preskaler CLKIO/8
	TIMSK0|=_BV(TOIE0);	//Odblokuj przerwanie nadmiaru timera 0
}

void Timer0InitWithDimmer()
{
	TIMSK0|=_BV(OCIE0A);		//Wlacz przerwanie Compare Match A
	OCR0A=255;
	Timer0Init();
}

volatile uint8_t key=0xFF;

void ReadButtons()
{
	static uint8_t Counters[2];

	uint8_t k=PINB;
	if(Counters[0]==0)
	{
		if(((k^key) & _BV(PB0)) && ((k & _BV(PB0))==0)) Counters[0]=100;  // Czas przez jaki stan przycisku bêdzie ignorowany
		key&=~_BV(PB0);
		key|=(k & _BV(PB0));
	} else Counters[0]--;

	if(Counters[1]==0)
	{
		if(((k^key) & _BV(PB7)) && ((k & _BV(PB7))==0)) Counters[1]=100;  // Czas przez jaki stan przycisku bêdzie ignorowany
		key&=~_BV(PB7);
		key|=(k & _BV(PB7));
	} else Counters[1]--;
}

void itoLED(uint16_t l)
{
	for(uint8_t i=0;i<LEDDISPNO;i++)
	{
		LEDDIGITS[i]=l%10;
		l/=10;
	}
}

int main()
{
	LEDDDR=0xFF;		// Wszystkie piny portu sa wyjsciem
	PORTB|=_BV(PB0) | _BV(PB7);	// Wlacz pull up na pinie PB0 i PB7
	uint16_t liczba=1234;	// Poczatkowa wyœwietlana wartoœæ

	DDRC|=0x0F;			// Piny PC0-PC3 jako wyjœcia
	// Timer0InitWithDimmer();
	Timer0Init();
	sei();

	uint8_t oldkey=0xFF;	// Przechowuje poprzedni stan klawiszy

	while(1)
	{
		itoLED(liczba);
		if(((oldkey ^ key) & _BV(PB0)) && (key & _BV(PB0))==0) liczba++;
		if(((oldkey ^ key) & _BV(PB7)) && (key & _BV(PB7))==0) liczba--;
		oldkey=key;
	};
}
