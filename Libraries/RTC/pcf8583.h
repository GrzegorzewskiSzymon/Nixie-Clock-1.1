/*
 * pcf8583.h
 *
 *  Created on: 27 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_RTC_PCF8583_H_
#define LIBRARIES_RTC_PCF8583_H_


uint8_t DecToBCD(uint8_t dec);
uint8_t BCDToDec(uint8_t bcd);

void RtcInit();
void RtcReadData(uint8_t *seconds, uint8_t *minutes, uint8_t *hours);


typedef enum
{
	ENTER_HOURS = 0,
	ENTER_MINUTES,
	ENTER_SECONDS

}TIME_TO_ENTER;


#endif /* LIBRARIES_RTC_PCF8583_H_ */
