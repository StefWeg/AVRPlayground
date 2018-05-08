/*
 * RingBuffer.c
 *
 * Created: 27.08.2017 15:43:54
 *  Author: Stefoo
 */ 


 #include "RingBuffer.h"

 #include <util/atomic.h>

 bool cb_Add(CircBuffer *cb, CB_Element elem)
 {
	 ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	 {
		 if(cb_IsFull(cb)) return false;        // Czy jest miejsce w kolejce?
		 uint8_t end = (cb->Beg + cb->Count) % CB_MAXTRANS;
		 cb->elements[end] = elem;              // Dodaj transakcje
		 ++cb->Count;                           // Liczba elementów w buforze
	 }
	 return true;      // Wszystko ok
 }

 CB_Element cb_Read(CircBuffer *cb)
 {
	 CB_Element elem;
	 ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	 {
		 if(cb_IsEmpty(cb)) return 0;       // Bufor pusty, nie mo?na zwróci? elementu
		 elem = cb->elements[cb->Beg];
		 cb->Beg = (cb->Beg + 1) % CB_MAXTRANS;
		 -- cb->Count;                      // Zmniejszamy liczb? elementów pozosta?ych
	 }		                               // w buforze
	 return elem;
 }