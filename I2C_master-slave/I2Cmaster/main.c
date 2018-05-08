/*
 * I2Cmaster.c
 *
 * Created: 25.09.2017 14:40:08
 * Author : Stefoo
 */ 


 #include <util\delay.h>
 #include <util\twi.h>
 #include "I2CBase.h"
 #include <string.h>
 #include <stdio.h>

 #define DEVADDR 190

 void I2C_SendTxt(char *txt)
 {
	 while(*txt) I2C_SendByte(*txt++);
	 I2C_SendByte(0);	// Wyslij znak konca lancucha
 }

 void I2C_StartSelectWait(uint8_t addr)
 {
	 do
	 {
		 I2C_SendStartAndSelect(addr);
	 } while(I2C_Error==I2C_NoACK);
 }

 volatile uint16_t temper;

 int main()
 {
	 char wynik[7];

	 _delay_ms(1000);
	 I2C_Init();	// Zainicjuj TWI z domyslna szybkoscia 100kHz
	 I2C_StartSelectWait(DEVADDR);
	 I2C_SendByte('c');
	 I2C_Stop();
	 I2C_WaitTillStopWasSent();

	 I2C_StartSelectWait(DEVADDR);
	 I2C_SendByte('g'); I2C_SendByte(0); I2C_SendByte(0);
	 I2C_Stop();
	 I2C_WaitTillStopWasSent();

	 I2C_StartSelectWait(DEVADDR);
	 I2C_SendByte('t'); I2C_SendTxt("Temperatura po I2C");
	 I2C_Stop();
	 I2C_WaitTillStopWasSent();

	 while(1)
	 {
		 I2C_StartSelectWait(DEVADDR | TW_READ);
		 temper=I2C_ReceiveData_ACK();
		 temper+=256*I2C_ReceiveData_NACK();
		 I2C_Stop();
		 
		 I2C_StartSelectWait(DEVADDR);
		 I2C_SendByte('g'); I2C_SendByte(0); I2C_SendByte(1);
		 I2C_Stop();
		 I2C_WaitTillStopWasSent();

		 sprintf(wynik, "%5d", temper);
		 uint8_t len=strlen(wynik);
		 memmove(&wynik[len-1], &wynik[len-2], 3);
		 wynik[len-2]=',';

		 I2C_StartSelectWait(DEVADDR);
		 I2C_SendByte('t'); I2C_SendTxt(wynik);
		 I2C_Stop();
		 I2C_WaitTillStopWasSent();

		 _delay_ms(100);
	 }
 }

