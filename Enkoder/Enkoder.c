/*
 * Enkoder.c
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
	if((val & 0x7F)<11) tmp=DIGITS[val & 0x7F];  //Odczytaj kod znaku
	if(val & DP) tmp&=~DP;      //Sterowanie kropk¹ dziesiêtn¹ na LED
	LEDPORT=tmp;
}


#define LEDDISPNO	4          //Liczba wyœwietlaczy

volatile uint8_t LEDDIGITS[LEDDISPNO];  //Tablica zawieraj¹ca dane do wyœwietlania

static inline void ReadEncoder();       //Prototyp funkcji odczytu enkodera

ISR(TIMER0_OVF_vect)
{
	static uint8_t LEDNO;

	PORTC|=0x0F;		//Wy³¹cz wszystkie wywietlacze
	LEDNO=(LEDNO+1)%LEDDISPNO;
	ShowOnLED(LEDDIGITS[LEDNO]);
	PORTC=(PORTC & 0xF0) | (~(1<<LEDNO) & 0x0F);	//Wybierz kolejny wywietlacz
	ReadEncoder();    //Odczytaj stan klawiszy
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

int8_t enc_delta;

void ReadEncoder()
{
	static int8_t last;
	static uint8_t laststate;
	static uint8_t counters[2];	//Tablica zawieraj¹ca liczniki
	int8_t newpos, diff;
	
	uint8_t state=PINB;
	if(((state^laststate) & _BV(PB0)) && (counters[0]==0))
	{
		counters[0]=200;
		laststate&=(~_BV(PB0));
		laststate|=(state & _BV(PB0));
	}

	if(((state^laststate) & _BV(PB7)) && (counters[1]==0))
	{
		counters[1]=200;
		laststate&=(~_BV(PB7));
		laststate|=(state & _BV(PB7));
	}
	
	for(uint8_t c=0;c<sizeof(counters)/sizeof(counters[0]);c++)
	    if(counters[c])	counters[c]--;

	newpos=0;
	if((PINB & _BV(PB0))==0) newpos=3;
	if((PINB & _BV(PB7))==0) newpos^=1;	// konwersja kodu Graya na binarny
	diff=last-newpos;
	if(diff & 1)
	{				// bit 0 = krok
		last=newpos;
		enc_delta+=(diff & 2)-1;	//bit 1 - kierunek
	}
}

int8_t Read1StepEncoder()
{
	ReadEncoder();
	int8_t val=enc_delta;
	enc_delta=0;
	return val;
}

int8_t Read2StepEncoder()
{
	ReadEncoder();
	int8_t val=enc_delta;
	enc_delta=val & 1;
	return val>>1;
}

int8_t Read4StepEncoder()
{
	ReadEncoder();
	int8_t val=enc_delta;
	enc_delta=val & 3;
	return val>>2;
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
	LEDDDR=0xFF;		//Wszystkie piny portu s¹ wyjciem
	PORTB|=_BV(PB0) | _BV(PB0);	//W³¹cz pull up na pinie PB0 i PB7
	uint16_t liczba=1234;

	DDRC|=0x0F;			//Piny PC0-PC3 jako wyjœcia
	//Timer0InitWithDimmer();
	Timer0Init();
	sei();

	while(1)
	{
		liczba+=Read4StepEncoder();
		itoLED(liczba);
	}
}
