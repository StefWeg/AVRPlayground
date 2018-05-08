/*
 * Sterowanie_LCD7Seg.c
 *
 *  Author: Stefoo
 */ 


#include <avr\io.h>
#include <util\delay.h>

#define LED	D

#define GLUE(a, b)     a##b    // The ## operator takes two separate tokens and pastes them together to form a single token.
								// The resulting token could be a variable name, class name or any other identifier.
								// ex. #define type i##nt
								//     type a; // same as int a; since i##nt pastes together to "int"

#define LEDPORT1(s)	GLUE(PORT,s)
#define LEDPORT LEDPORT1(LED)
#define LEDDDR1(s)	GLUE(DDR,s)
#define LEDDDR	LEDDDR1(LED)

//Cyfry 0,1,2,3,4,5,6,7,8,9 i blank
static const uint8_t DIGITS[11]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00};
const uint8_t DP = 0x80;

void testDisp()
{
	LEDPORT = 0x00;
	_delay_ms(1000);
	for (int i = 0; i < 8; i++)
	{
		LEDPORT = _BV(i);
		_delay_ms(1000);
	}
}

void ShowOnLED(uint8_t val)
{
	uint8_t tmp=0xFF;   // (dla x == 11 zostaje)
	if((val & 0x7F)<11) tmp = DIGITS[val & 0x7F];  // Odczytaj kod znaku
	if(val & DP) tmp &= ~DP;      // Sterowanie kropka dziesiestna na LED
	LEDPORT=tmp;
}

uint8_t counters[2];	// Tablica zawierajaca liczniki (zmienne globalne s? incjalizowane jako "0")

int main()
{
	LEDDDR = 0xFF;		// Wszystkie piny portu sa wyjsciem
	// DDRB = 0x00;   // po resecie mikrokontrolera wszystkie bity rejestru s? zerowane
	PORTB|= (_BV(PB0) | _BV(PB1));	//Wlacz pull upy na pinach PB0 i PB7

	uint8_t oldkey = PINB & (_BV(PB0) | _BV(PB1));
	uint8_t key;
	uint8_t x=0;
	uint8_t przerwa;
	
	while(1)
	{
		ShowOnLED(x);

		if(counters[0]==0)
		{
			key = PINB & _BV(PB0);		//Odczytaj stan klawisza
			if(((key^oldkey) & _BV(PB0)) && ((key & _BV(PB0))==0))
			{
				if(x>0) x--;		//Jezli sie zmienil i jest wcisniety to zmien x
				counters[0]=200;	//Czas przez jaki stan przycisku bedzie ignorowany
			}
			oldkey &= ~_BV(PB0);
			oldkey |= key;
		}

		if(counters[1]==0)
		{
			key = PINB & _BV(PB1);		//Odczytaj stan klawisza
			if(((key^oldkey) & _BV(PB1)) && ((key & _BV(PB1))==0))
			{
				if(x<9) x++;		//Jezli sie zmienil i jest wcisniety to zmien x
				counters[1]=200;	//Czas przez jaki stan przycisku bedzie ignorowany
			}
			oldkey &= ~_BV(PB1);
			oldkey |= key;
		}

		przerwa=0;
		for(uint8_t c=0;c<2;c++)
		if(counters[c])
		{
			counters[c]--;
			przerwa=1;			//Robimy opóznienie tylko jezli którys z liczników byl !=0
		}
		if(przerwa) _delay_ms(1);


		/*
		if ((PINB & _BV(PB0)) == 0) {
			x--;
			if (x == 255) x = 9;	// -1 is interpreted as 255
		}
		if ((PINB & _BV(PB1)) == 0) x++;
		x = x%10;
		_delay_ms(100);
		*/

		// x = (x+1)%12;
		// _delay_ms(1000);
	}
}
