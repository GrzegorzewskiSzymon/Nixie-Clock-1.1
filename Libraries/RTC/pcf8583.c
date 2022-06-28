/*
 * pcf8583.c
 *
 *  Created on: 27 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#include <avr/io.h>
#include "pcf8583.h"
#include "i2c.h"
#include "../NixieDrivers/SN74141.h"
#include "../Utils/utils.h"

uint8_t DecToBCD(uint8_t dec) {
	return ((dec / 10) << 4) | (dec % 10);
}
uint8_t BCDToDec(uint8_t bcd){
	return ((((bcd)>>4) & 0x0F) *10 ) + ((bcd) & 0x0F);
}

void RtcInit()
{
	//[0]milis, [1]seconds, [2]minutes, [3]hours
	int8_t buffer[4] = {0,0,0,12};
	uint64_t millis_Switch = millis;
	while(millis - millis_Switch <5000)
	{
		LED_ON;

		//check state of buttons
		StateMachine(&Switch_0);
		StateMachine(&Switch_1);

		if(buffer[2] != Switch_0.counterData - Switch_1.counterData) //if any of buttons were pressed
			millis_Switch = millis; //reset millis to give time to change time

		buffer[2] = Switch_0.counterData - Switch_1.counterData;

		if(buffer[2] >= 60)//60 min turns to 1 hour
		{
			Switch_0.counterData = 0;
			Switch_1.counterData = 0;
			buffer[3]++;
		}

		if(buffer[3] > 23) buffer[3] = 0;

		if(buffer[2] <= -1) //-1 in minutes need to be compensated by decreasing hour and setting minutes to 59
		{
			Switch_0.counterData = 59; //set minutes to 59
			Switch_1.counterData = 0;

			if(buffer[3])
				buffer[3]--;
			else
				buffer[3] = 23;//decreasing from 00:00 goes to 23:59
		}

		DisplayTime(buffer[3], buffer[2]);
	}

	/*********only if user tried to change time********/
	if(buffer[3] != 12 || buffer[2] != 0)
	{
		/*************uploading new time to RTC************/
		for(uint8_t i=0; i<4; i++)
			buffer[i] = DecToBCD(buffer[i]);

		TWI_Write_Buf(0xA0, 0x01, 4, buffer);
	}
	/******************************************************/

	LED_OFF;
}

void RtcReadData(uint8_t *seconds, uint8_t *minutes, uint8_t *hours )
{
	uint8_t buffer2[3];
	TWI_Read_Buf(0xA0, 0x02, 3, buffer2);
	*seconds = BCDToDec(buffer2[0]);
	*minutes = BCDToDec(buffer2[1]);
	*hours   = BCDToDec(buffer2[2]);

}

