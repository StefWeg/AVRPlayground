/*
 * Interrupt_playground.c
 *
 * Created: 08.07.2017 11:16:44
 * Author : Stefoo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

volatile char a = 0; // modyfikator volatile mowi kompilatorowi, ze zmienna zadeklarowana z tym modyfikatorem
						// moze byc modyfikowana w sposob niewynikajacy bezposrednio z otaczajacego kodu
						// (niezbedny w sytuacji, kiedy zmienna globalna jest modyfikowana w procedurze przerwania
						// lub kiedy wartosc moze zmieniac sie w sposob niewynikajacy z wykonywanego kodu - np.
						// odczyt rejestru IO)
						// zastosowanie volatile powoduje ze zmienna jest kazdorazowo odczytywana i zapisywana
						// (nie optymalizuje sie kodu w taki sposob by uzywal poprzednio odczytanej wartosci)
volatile int b = 100;

ISR(BADISR_vect) // domyslna funkcja obslugi przerwania (uruchamiana dla wlaczonych przerwan bez funkcji obslugi)
{
	a++;
	b++;
}

EMPTY_INTERRUPT(TCCO_CCA_vect); // zoptymalizowana pusta obsluga przerwania (zapewania skasowanie flagi przerwania)

ISR(TCCO_CCB_vect, ISR_ALIASOF(TCCO_CCA_vect)); // utworzenie aliasu (obsluga TCCO_CCB_vect = obsludze TCCO_CCA_vect)

int main(void)
{
	int tmp = 0;
	while (tmp == 0)
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON) // ATOMIC_FORCEON ustawia ustawia brak blokady przerwan po bloku
		{
			tmp = b; // nie zachodzi obrawa, ze wartosc zostanie niespodziewanie zmodyfikowana		
		}
		// blok nie moze obejmowac zalej petli bo wowczas przerwanie nie mogloby dokonac zmiany wartosci b
	}

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) // ATOMIC_RESTORESTATE ustawia stan blokady przerwa? sprzed bloku
	{
		b++; // inkrementacja obu bitów zmiennej int bedzie wykonywana atomowo
	}

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { void *ptr=malloc(100); }
	
    while (1) {}
}

