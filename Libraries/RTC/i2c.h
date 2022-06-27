/*
 * i2c.h
 *
 *  Created on: 27 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_RTC_I2C_H_
#define LIBRARIES_RTC_I2C_H_


void TWI_Start(void);
void TWI_Stop(void);
void 	TWI_Write(uint8_t byte);
void 	TWI_Write_Buf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf);
uint8_t TWI_Read(uint8_t ack);
void 	TWI_Read_Buf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf);



#endif /* LIBRARIES_RTC_I2C_H_ */
