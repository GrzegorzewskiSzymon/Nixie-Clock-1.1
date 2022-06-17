/*
 * main.c
 *
 *  Created on: 17 cze 2022
 *      Author: Grzegorzewski Szymon
 */
#include <avr/io.h>
#include <util/delay.h>
#include "Libraries/Utils/utils.h"


int main(void)
{
	Timer0_Init();//Starts counting millis

	Led_Init();
	BlinkLed(); //visual confirmation of reset


	while(1)
	{

	}

}
