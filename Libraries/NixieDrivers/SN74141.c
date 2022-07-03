/*
 * SN74141.c
 *
 *  Created on: 23 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#include <avr/io.h>
#include <util/delay.h>
#include "SN74141.h"
#include "AnodesMultiplexer.h"
#include "../Utils/utils.h"

void BCD_Init()
{
	BCD_INIT;
}

uint64_t millis_DisplayTime = 0;
void DisplayTime(int8_t hours, int8_t minutes)
{

	if(millis-millis_DisplayTime>20)//Its time to display units of minutes
	{
		NIXIE_3_OFF;NIXIE_2_OFF;NIXIE_1_OFF;NIXIE_0_OFF;
		if(millis-millis_DisplayTime>21)//one ms later
		{
			PORTD &=~ 0b00001111;
			PORTD |= minutes%10;

			NIXIE_3_OFF;
			NIXIE_2_OFF;
			NIXIE_1_OFF;
			NIXIE_0_ON;

			millis_DisplayTime = millis;//set actual time after full cicle
		}
		return;
	}

	if(millis-millis_DisplayTime>15)//Its time to display tens of minutes
	{
		NIXIE_3_OFF;NIXIE_2_OFF;NIXIE_1_OFF;NIXIE_0_OFF;
		if(millis-millis_DisplayTime>16)//one ms later
		{
			PORTD &=~ 0b00001111;
			PORTD |= minutes/10;

			NIXIE_3_OFF;
			NIXIE_2_OFF;
			NIXIE_1_ON;
			NIXIE_0_OFF;
		}
		return;
	}

	if(millis-millis_DisplayTime>10)//Its time to display units of hours
	{
		NIXIE_3_OFF;NIXIE_2_OFF;NIXIE_1_OFF;NIXIE_0_OFF;
		if(millis-millis_DisplayTime>11)//one ms later
		{
			PORTD &=~ 0b00001111;
			PORTD |= hours%10;

			NIXIE_3_OFF;
			NIXIE_2_ON;
			NIXIE_1_OFF;
			NIXIE_0_OFF;
		}
		return;
	}

	if(millis-millis_DisplayTime>5 && hours>9)//Its time to display tens of hours
	{
		NIXIE_3_OFF;NIXIE_2_OFF;NIXIE_1_OFF;NIXIE_0_OFF;
		if(millis-millis_DisplayTime>6)//one ms later
		{
			PORTD &=~ 0b00001111;
			PORTD |= hours/10;

			NIXIE_3_ON;
			NIXIE_2_OFF;
			NIXIE_1_OFF;
			NIXIE_0_OFF;
		}
	}

}

