/*
 * Klawiatura.c
 *
 * Created: 2014-08-13 17:59:15
 *  Author: tmf
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
	if((val & 0x7F)<11) tmp=DIGITS[val & 0x7F];  //Odczytaj kod znaku
	if(val & DP) tmp&=~DP;      //Sterowanie kropk¹ dziesiêtn¹ na LED
	LEDPORT=tmp;
}


#define LEDDISPNO	4          //Liczba wyœwietlaczy

volatile uint8_t LEDDIGITS[LEDDISPNO];  //Tablica zawieraj¹ca dane do wyœwietlania
volatile uint8_t key=0xFF;

ISR(TIMER0_OVF_vect)
{
	static uint8_t LEDNO;
	static uint8_t Counter;		//Debouncing
	static uint8_t row;			//Rz¹d w którym znajdowa³ siê ostatnio odczytany klawisz

	PORTC|=0x0F;		//Wy³¹cz wszystkie wywietlacze
	LEDNO=(LEDNO+1)%LEDDISPNO;
	ShowOnLED(LEDDIGITS[LEDNO]);
	PORTC=(PORTC & 0xF0) | (~(1<<LEDNO) & 0x0F);	//Wybierz kolejny wyœwietlacz i kolumnê klawiatury
//Tu zaczynamy obs³ugê klawiatury
	if(Counter==0)
	{
		uint8_t x=PINB;
		if((x & 0b1110)!=0b1110)	// Jakis klawisz bys wcisniety?
		{
			if((x & 0b0010)==0) x=0;	//Jesli tak, to z której kolumny?
				else if((x & 0b0100)==0) x=1;
					else x=2;
			key=x+LEDNO*3;			//Oblicz numer klawisza
			row=LEDNO;
			Counter=100;
		} else if (row==LEDNO) key=0xFF;
	} else Counter--;
}

/*ISR(TIMER0_COMPA_vect)
{
	PORTC|=0x0F;	//Wy³¹cz wszystkie wywietlacze
}*/

ISR(TIMER0_COMPA_vect, ISR_NAKED)
{
	PORTC|=0x01;	//Wy³¹cz wszystkie wywietlacze
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
	TIMSK0|=_BV(OCIE0A);		//W³¹cz przerwanie Compare Match A
	OCR0A=255;
	Timer0Init();
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
	LEDDDR=0xFF;		//Wszystkie piny portu s¹ wyjœciem
	PORTB|=_BV(PB1) | _BV(PB2) | _BV(PB3);	//W³¹cz pull up na pinie PB1, PB2 i PB3 - rzêdach klawiatury
	uint16_t liczba;
	DDRC|=0x0F;			//Piny PC0-PC3 jako wyjœcia
	//Timer0InitWithDimmer();
	Timer0Init();
	sei();

	while(1)
	{
		liczba=key;
		itoLED(liczba);
	}
}