/*
 * main.c
 *
 *  Created on: 17 cze 2022
 *      Author: Grzegorzewski Szymon
 */


#include <avr/io.h>
#include <util/delay.h>



int main(void)
{

	DDRC |= (1<<PC2);


	while(1)
	{
		PORTC ^= (1<<PC2);
		_delay_ms(500);
	}

}
