/*
 * Fuses_avr_libc.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/lock.h>

FUSES =
{
	.low = LFUSE_DEFAULT,        //Domy?lne ustawienia
	.high = (FUSE_BOOTSZ0 & FUSE_BOOTSZ1 & FUSE_EESAVE & FUSE_DWEN),
	.extended = EFUSE_DEFAULT,   //Domy?lne ustawienia
};

LOCKBITS = LB_MODE_1 & BLB0_MODE_3 & BLB1_MODE_4; //Konfguruj bity zabezpieczaj?ce

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}
