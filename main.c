/*
 * main.c
 *
 *  Created on: 17 cze 2022
 *      Author: Grzegorzewski Szymon
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Libraries/Utils/utils.h"
#include "Libraries/NixieDrivers/AnodesMultiplexer.h"
#include "Libraries/NixieDrivers/SN74141.h"
#include "Libraries/RTC/i2c.h"
#include "Libraries/RTC/pcf8583.h"

int main(void)
{
	Timer0_Init();//Starts counting millis

	Led_Init();
	BlinkLed(); //visual confirmation of reset

	SwitchInit(&Switch_0, &Switch_1, &Switch_2);//Numerate and pull up switches

	Anodes_Init();//Set pins for transoptor as output
	BCD_Init();

	//Put actual hour to rtc
	RtcInit();

	uint8_t seconds, minutes, hours;
	while(1)
	{
		RtcReadData(&seconds, &minutes, &hours);
		DisplayTime(hours, minutes);

	}

}
