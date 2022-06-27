/*
 * i2c.c
 *
 *  Created on: 27 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#include <avr/io.h>
#include "i2c.h"

#define ACK 1
#define NACK 0

void TWI_Start(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
	while (!(TWCR & (1 << TWINT)));

}
void TWI_Stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while ((TWCR & (1 << TWSTO)));

}
void TWI_Write(uint8_t byte) {
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

}
uint8_t TWI_Read(uint8_t ack) {
	TWCR = (1 << TWINT) | (ack << TWEA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

void TWI_Write_Buf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf) {
	TWI_Start();
	TWI_Write(sla); //adres uk³adu Slave
	TWI_Write(adr); //pierwszy adres bajtu do którego nast¹pi zapis
	while (len--)
		TWI_Write(*buf++);
	TWI_Stop();
}

void TWI_Read_Buf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf) {
	TWI_Start();
	TWI_Write(sla);
	TWI_Write(adr);
	TWI_Start();
	TWI_Write(sla + 1);
	while (len--) *buf++ = TWI_Read(len ? ACK : NACK);
	TWI_Stop();
}



