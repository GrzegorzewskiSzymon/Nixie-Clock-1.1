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
#include "../NixieDrivers/AnodesMultiplexer.h"

uint8_t DecToBCD(uint8_t dec) {
	return ((dec / 10) << 4) | (dec % 10);
}
uint8_t BCDToDec(uint8_t bcd){
	return ((((bcd)>>4) & 0x0F) *10 ) + ((bcd) & 0x0F);
}

uint8_t timeToEnter;
void RtcInit()
{
	//[0]hours, [1]minutes, [2]seconds, [3]millis
	int8_t buffer[4] = {0,0,0,0};
	uint64_t millis_DisplayTime_RTCINIT = millis;
	uint64_t millis_Switch = millis;
	while(millis - millis_Switch <TIME_FOR_SETTING_TIME)
	{
		LED_ON;

		//check state of button
		StateMachine(&Switch_0); //To switch between h/m/s to enter

		if(Switch_0.counterData)
		{
			millis_Switch = millis;
			++timeToEnter;
			timeToEnter %=3; //There are only three variations
			Switch_0.counterData = 0;
			Switch_1.counterData = 0;
		}


		switch(timeToEnter){
			case ENTER_HOURS:
					Switch_1.counterData = buffer[0]; // restore previously entered value
					StateMachine(&Switch_1); //check if user increased value
					if(buffer[0] != Switch_1.counterData)//if user increased value
					{
						millis_Switch = millis;//reset timer to enter time
					}
					buffer[0] = Switch_1.counterData;
				break;

			case ENTER_MINUTES:
					Switch_1.counterData = buffer[1];
					StateMachine(&Switch_1);
					if(buffer[1] != Switch_1.counterData)
					{
						millis_Switch = millis;
					}
					buffer[1] = Switch_1.counterData;
				break;

			case ENTER_SECONDS:
					Switch_1.counterData = buffer[2];
					StateMachine(&Switch_1);
					if(buffer[2] != Switch_1.counterData)
					{
						millis_Switch = millis;
					}
					buffer[2] = Switch_1.counterData;
				break;
		}

		//overflow
		if(buffer[0] > 23) buffer[0] = 0; //hours
		if(buffer[1] > 59) buffer[1] = 0; //minutes
		if(buffer[2] > 59) buffer[2] = 0; //seconds


		/*START*OF******************DISPLAYING***************************/
		switch (timeToEnter) {

			case ENTER_HOURS:
				//entering overflowed data to sn74141 couse tuning off all outputs
				PORTB |= 0b11110000;//Turn off diplaying minutes
				PORTC |= 0b00001111;//Turn off diplaying seoncds

				if(millis-millis_DisplayTime_RTCINIT>20)//Its time to display units
				{
					NIXIE_UNITS_ON;
					NIXIE_TENS_OFF;

					//displaying  units of hours
					PORTB &=~ 0b00001111;
					PORTB |= buffer[0]%10;
					millis_DisplayTime_RTCINIT = millis;
					break;
				}
				if(millis-millis_DisplayTime_RTCINIT>10)//Its time to display tens
				{
					NIXIE_UNITS_OFF;
					NIXIE_TENS_ON;

					//displaying tens of hours
					PORTB &=~ 0b00001111;
					PORTB |= buffer[0]/10;
				}
				break;

			case ENTER_MINUTES:
				//entering overflowed data to sn74141 couse tuning off all outputs
				PORTB |= 0b00001111;//Turn off diplaying hours
				PORTC |= 0b00001111;//Turn off diplaying seoncds

				if(millis-millis_DisplayTime_RTCINIT>20)//Its time to display units
				{
					NIXIE_UNITS_ON;
					NIXIE_TENS_OFF;

					//displaying  units of minutes
					PORTB &=~ 0b11110000;
					PORTB |= (buffer[1]%10)<<4;
					millis_DisplayTime_RTCINIT = millis;
					break;
				}
				if(millis-millis_DisplayTime_RTCINIT>10)//Its time to display tens
				{
					NIXIE_UNITS_OFF;
					NIXIE_TENS_ON;

					//displaying tens of minutes
					PORTB &=~ 0b11110000;
					PORTB |= (buffer[1]/10)<<4;
				}

				break;

			case ENTER_SECONDS:
				//entering overflowed data to sn74141 couse tuning off all outputs
				PORTB |= 0b11111111;//Turn off diplaying hours and minutes

				if(millis-millis_DisplayTime_RTCINIT>20)//Its time to display tens
				{
					NIXIE_UNITS_ON;
					NIXIE_TENS_OFF;

					//displaying  tens of minutes
					PORTC &=~ 0b00001111;
					PORTC |= buffer[2]%10;
					millis_DisplayTime_RTCINIT = millis;
					break;
				}
				if(millis-millis_DisplayTime_RTCINIT>10)//Its time to display units
				{
					NIXIE_UNITS_OFF;
					NIXIE_TENS_ON;

					//displaying units of minutes
					PORTC &=~ 0b00001111;
					PORTC |= buffer[2]/10;
				}
				break;
		}

		/*END*OF******************DISPLAYING***************************/
	}

	/*********only if user tried to change time********/
	if(buffer[0] || buffer[1] || buffer[2])
	{
		//adds 8 sec, to upload exact time at which the last time button was pressed
		buffer[2] += TIME_FOR_SETTING_TIME;

		if(buffer[2] > 59)
		{
			buffer[1]++;
			buffer[2] = 0;
		}
		if(buffer[1] > 59)
		{
			buffer[0]++;
			buffer[1] = 0;
		}

		if(buffer[0] > 23) buffer[0] = 0; //hours

		/*************uploading new time to RTC************/
		int8_t bufferToSend[4];
		for(uint8_t i=0; i<4; i++)
			bufferToSend[i] = DecToBCD(buffer[3-i]);

		TWI_Write_Buf(0xA0, 0x01, 4, (uint8_t*)bufferToSend);
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

