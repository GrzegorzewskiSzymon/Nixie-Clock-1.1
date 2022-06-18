/*
 * utils.c
 *
 *  Created on: 17 cze 2022
 *      Author: Grzegorzewski Szymon
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "utils.h"

void Led_Init()
{
	LED_INIT;
}

void BlinkLed()
{
	LED_ON;
	_delay_ms(250);
	LED_OFF;
}

/********************Interrupt********************/
uint64_t millis;
void Timer0_Init()
{
	sei();// allow interrupts

	TCCR0A |= (1<<WGM01); //CTC mode
	TIMSK0 |= (1<<OCIE0A); //Compare match
	TCCR0B |= (1<<CS00) | (1<<CS01); //Prescaler = 64     8000000\64 = 125000
	OCR0A = 124;                     //                   125000\(124+1) = 1000
}

ISR(TIMER0_COMPA_vect)
{
	millis++;
}
/*end*of***************Interrupt********************/


/********************Buttons********************/
SWITCH Switch_0;
SWITCH Switch_1;
SWITCH Switch_2;

void SwitchInit(SWITCH *switch_0, SWITCH *switch_1, SWITCH *switch_2)
{
	switch_0->switchNumber = 0;
	switch_1->switchNumber = 1;
	switch_2->switchNumber = 2;

	SW_0_PULL_UP;
	SW_1_PULL_UP;
	SW_2_PULL_UP;

}


static void Idle(SWITCH *switch_)
{
	if(switch_->switchNumber == 0 && SW_0_IS_PUSHED)//possibility of button0 being pressed
	{
		switch_->timerDebounce = millis;
		switch_->switchState = DEBOUNCE;
	}

	else if(switch_->switchNumber == 1 && SW_1_IS_PUSHED)//possibility of button1 being pressed
	{
		switch_->timerDebounce = millis;
		switch_->switchState = DEBOUNCE;
	}

	else if(switch_->switchNumber == 2 && SW_2_IS_PUSHED)//possibility of button2 being pressed
	{
		switch_->timerDebounce = millis;
		switch_->switchState = DEBOUNCE;
	}

}

static void Debounce(SWITCH *switch_)
{
	if(millis-switch_->timerDebounce > DEBOUNCE_TIME)
	{
		if(switch_->switchNumber == 0 && SW_0_IS_PUSHED)//if pressed
		{
			switch_->counterData++;
			switch_->timerPress = millis;
			switch_->switchState = PRESSED;
		}
		else if(switch_->switchNumber == 1 && SW_1_IS_PUSHED)//if pressed
		{
			switch_->counterData++;
			switch_->timerPress = millis;
			switch_->switchState = PRESSED;
		}
		else if(switch_->switchNumber == 2 && SW_2_IS_PUSHED)//if pressed
		{
			switch_->counterData++;
			switch_->timerPress = millis;
			switch_->switchState = PRESSED;
		}
	}
}


static void Pressed(SWITCH *switch_)
{
	if(switch_->switchNumber == 0)
	{
		if(SW_0_IS_PUSHED)//if still pressed
		{
			if(millis-switch_->timerPress > LONG_PRESS_TIME)
			{
				switch_->timerLongPress = millis;
				switch_->switchState = LONG_PRESS;
			}
		}
		else
		{
			switch_->switchState = RELEASED;
		}
	}
	else if (switch_->switchNumber == 1)
	{
		if(SW_1_IS_PUSHED)//if still pressed
		{
			if(millis-switch_->timerPress > LONG_PRESS_TIME)
			{
				switch_->timerLongPress = millis;
				switch_->switchState = LONG_PRESS;
			}
		}
		else
		{
			switch_->switchState = RELEASED;
		}
	}
	else if (switch_->switchNumber == 2)
	{
		if(SW_2_IS_PUSHED)//if still pressed
		{
			if(millis-switch_->timerPress > LONG_PRESS_TIME)
			{
				switch_->timerLongPress = millis;
				switch_->switchState = LONG_PRESS;
			}
		}
		else
		{
			switch_->switchState = RELEASED;
		}
	}
}

static void LongPress(SWITCH *switch_)
{
	if(millis-switch_->timerLongPress > LONG_PRESS_TIME_CYCLE)
	{
		if(switch_->switchNumber == 0)
		{
			if(SW_0_IS_PUSHED)//if pressed
			{
				switch_->counterData++;
				switch_->timerLongPress = millis;
			}
			else
			{
				switch_->switchState = RELEASED;
			}
		}
		else if (switch_->switchNumber == 1)
		{
			if(SW_1_IS_PUSHED)//if pressed
			{
				switch_->counterData++;
				switch_->timerLongPress = millis;
			}
			else
			{
				switch_->switchState = RELEASED;
			}
		}
		else if (switch_->switchNumber == 2)
		{
			if(SW_2_IS_PUSHED)//if pressed
			{
				switch_->counterData++;
				switch_->timerLongPress = millis;
			}
			else
			{
				switch_->switchState = RELEASED;
			}
		}
	}
}

void StateMachine(SWITCH *switch_)
{
	switch(switch_->switchState)
	{
		case IDLE:
			Idle(switch_);
			break;

		case DEBOUNCE:
			Debounce(switch_);
			break;

		case PRESSED:
			Pressed(switch_);
			break;

		case LONG_PRESS:
			LongPress(switch_);
			break;

		case RELEASED:
			switch_->switchState = IDLE;
			break;
	}
}
/*end*of*************Buttons********************/













