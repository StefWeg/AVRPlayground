/*
 * I2CBase.h
 *
 * Created: 07.09.2017 16:20:55
 *  Author: Stefoo
 */ 


#ifndef I2CBASE_H_
#define I2CBASE_H_


#include <stdint.h>
#include <util/twi.h>

#define I2CBUSCLOCK		50000UL

#define I2C_OK          0
#define I2C_STARTError	1
#define I2C_NoNACK		3
#define I2C_NoACK		4

extern uint8_t I2C_Error;
inline void I2C_SetError(uint8_t err) { I2C_Error=err;};

void I2C_Init();
void I2C_Start();
static inline void I2C_Stop() {TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);};	//Nadaj sygna� STOP
void I2C_SetBusSpeed(uint16_t speed);	//Ustaw pr�dko�� magistrali I2C (Pr�dko�� wyniesie speed*100[Hz])
void I2C_SendAddr(uint8_t address);		//Wy�lij adres urz�dzenia slave
void I2C_SendByte(uint8_t byte);		//Wy�lij bajt danych do slave
uint8_t I2C_ReceiveData_ACK();			//Odbierz bajt danych od slave, wysy�aj�c ACK
uint8_t I2C_ReceiveData_NACK();			//Odbierz bajt danych od slave, nie wysy�aj ACK
static inline void I2C_WaitForComplete() {while (!(TWCR & _BV(TWINT)));};		//Zaczekaj na koniec bie��cej operacji I2C
static inline void I2C_WaitTillStopWasSent() {while (TWCR & _BV(TWSTO));};	//Zaczekaj na koniec nadawania sygna�u STOP
void I2C_SendStartAndSelect(uint8_t addr);				//Wy�lij START i adres urz�dzenia slave


#endif /* I2CBASE_H_ */