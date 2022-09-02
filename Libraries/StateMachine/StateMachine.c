/*
 * StateMachine.c
 *
 *  Created on: 9 lip 2022
 *      Author: Grzegorzewski Szymon
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "StateMachine.h"
#include "../Utils/utils.h"
#include "../RTC/pcf8583.h"
#include "../IR/TransmitterIR.h"
#include "../NixieDrivers/SN74141.h"
#include "../NixieDrivers/NixieDiodes.h"
#include "../NixieDrivers/AnodesMultiplexer.h"

static void Clock_Idle();
uint8_t designatedTime[3];

static void Clock_Sustain();
uint64_t sustainMillis;

static void Clock_Sustain();

static void Clock_Alarmed();
uint8_t stageOfWakingUp = 0;
uint8_t alarmed_Flag = 0;
uint16_t stepOfStage_To_Do = 0;
uint16_t stepOfStage_Done = 0;
uint64_t millis_Alarmed;

CLOCK_STATE NixieClock = IDLE_CLOCK;
void StateMachine_Clock()
{
	switch(NixieClock)
	{
		case IDLE_CLOCK:
			Clock_Idle();
			break;

		case SUSTAIN_CLOCK:
			Clock_Sustain();
			break;

		case ALARMED_CLOCK:
			Clock_Alarmed();
			break;
	}
}

static uint8_t IrSignalCheck();

static void Clock_Idle()
{
	RtcReadData(&seconds, &minutes, &hours);
	DisplayTime(hours, minutes, 0);

	//check state of button
	StateMachine(&Switch_2);

	if(Switch_2.counterData)//signal to go to sustain mode
	{
		//Send few Rc5 signals to check if it can be received
		if(!IrSignalCheck())
			return;


		int8_t buffer[3] = {hours,minutes,0}; //hh mm ss
		Switch_0.counterData = minutes;
		uint64_t millis_Switch = millis;
		while(millis - millis_Switch <10000)
		{
			LED_ON;
			Switch_2.counterData = 0;

			//check state of buttons
			StateMachine(&Switch_0);
			StateMachine(&Switch_1);

			if(buffer[1] != Switch_0.counterData - Switch_1.counterData) //if any of buttons were pressed
					millis_Switch = millis; //reset millis to give time to change time

			buffer[1] = Switch_0.counterData - Switch_1.counterData;

			if(buffer[1] >= 60)//60 min turns to 1 hour
			{
				Switch_0.counterData = 0;
				Switch_1.counterData = 0;
				buffer[0]++;
			}

			if(buffer[0] > 23) buffer[0] = 0;

			if(buffer[1] <= -1) //-1 in minutes need to be compensated by decreasing hour and setting minutes to 59
			{
				Switch_0.counterData = 59; //set minutes to 59
				Switch_1.counterData = 0;

				if(buffer[0])
					buffer[0]--;
				else
					buffer[0] = 23;//decreasing from 00:00 goes to 23:59
		 	}

			DisplayTime(buffer[0], buffer[1], 0);
		}
		LED_OFF;
		if(hours != buffer[0] || minutes != buffer[1] ) //if user tried to set designated time
		{
			designatedTime[0] = buffer[0];
			designatedTime[1] = buffer[1];
			LED_OFF;
			NixieClock = SUSTAIN_CLOCK;
			sustainMillis = millis;
			Switch_2.counterData  = 0;
		}

	}
}


static uint8_t IrSignalCheck()
{

	if(millis - millis_Alarmed > 120)
	{
		stepOfStage_To_Do++;
		millis_Alarmed = millis;
	}
	 switch (stepOfStage_To_Do) {
		case 0:
			SendRc5(0, 0, DISSELECT_ALL_SEGMENTS);
			stepOfStage_To_Do++;
			break;

		case 2:
			SendRc5(0, 0, SEGMENT_1);
			stepOfStage_To_Do++;
			break;

		case 4:
			SendRc5(0, 0, BLUE);
			stepOfStage_To_Do++;
			break;

		case 6:
		case 8:
		case 10:
		case 12:
		case 14:
		case 16:
			SendRc5(0, 0, LIGHT_PLUS);
			stepOfStage_To_Do++;
			break;
		case 18:
			SendRc5(0, 0, LIGHTS_OUT);
			stepOfStage_To_Do = 0;
			return 1;
	}
	return 0;
}



static void Clock_Sustain()
{
	RtcReadData(&seconds, &minutes, &hours);
	DisplayTime(hours, minutes, (millis-sustainMillis)/300);

	if(designatedTime[0] > hours || (designatedTime[0] == hours && designatedTime[1] >= minutes) ) //if designated hour is still before us
	{
		//check state of button
		StateMachine(&Switch_2);

		if(Switch_2.counterData)//if pressed
		{
			sustainMillis = millis;
			Switch_2.counterData = 0;
		}

		if(millis-sustainMillis >= MAX_TIME_TO_CHECH_IN - 30000)
			LED_ON;
		else
			LED_OFF;


		if(millis-sustainMillis >= MAX_TIME_TO_CHECH_IN)
		{
			NixieClock = ALARMED_CLOCK;
			millis_Alarmed = millis;
			Switch_2.counterData = 0;
		}
	}
	else
	{
		NixieClock = IDLE_CLOCK;
		LED_OFF;
	}
}

static void Alarmed_Acknowledgment();
static void Alarmed_Smooth_One_Segment_Light_UP();
static void Alarmed_Drastic_One_Segment_Light_UP();
static void Alarmed_Drastic_Light_UP();
static void Alarmed_Turn_OFF();

static void Clock_Alarmed()
{
	//check state of button
	StateMachine(&Switch_2);

	if(Switch_2.counterData) //if pressed
	{
		millis_Alarmed = millis;
		stepOfStage_Done = 0;
		stepOfStage_To_Do = 0;
		stageOfWakingUp = ALARMED_TURN_OFF;
	}


	switch (stageOfWakingUp) {
		case ALARMED_ACKNOWLEDGMENT:
			Alarmed_Acknowledgment();
			break;

		case ALARMED_SMOOTH_ONE_SEGMET_LIGHT_UP:
			Alarmed_Smooth_One_Segment_Light_UP();
			break;

		case ALARMED_DRASTIC_ONE_SEGMET_LIGHT_VARIATIONS:
			LED_ON;
			Alarmed_Drastic_One_Segment_Light_UP();
			break;

		case ALARMED_DRASTIC_LIGHT_VARIATIONS:
			Alarmed_Drastic_Light_UP();
			break;

		case ALARMED_TURN_OFF:
			Alarmed_Turn_OFF();
			break;
	}
}

static void Alarmed_Acknowledgment()
{
	NIXIE_0_OFF;//Turn off clock
	NIXIE_1_OFF;
	NIXIE_2_OFF;
	NIXIE_3_OFF;

	if(millis- millis_Alarmed > 30000)//after 30 sec
	{
		stageOfWakingUp = ALARMED_SMOOTH_ONE_SEGMET_LIGHT_UP;
		millis_Alarmed = millis;
	}
}

static void Alarmed_Smooth_One_Segment_Light_UP()
{

//wait till we can send next rc5 signal
	if(millis - millis_Alarmed > 120)
	{
		stepOfStage_To_Do++;
		millis_Alarmed = millis;


	if(stepOfStage_To_Do != stepOfStage_Done)
		switch (stepOfStage_To_Do)
		{
			case 1:
				SendRc5(0, 0, SELECT_ALL_SEGMENTS);
				stepOfStage_Done = 1;
				return;
			case 2:
				SendRc5(0, 0, SELECT_ALL_SEGMENTS);
				stepOfStage_Done = 2;
				return;
			case 3:
				SendRc5(0, 0, LIGHTS_OUT);
				stepOfStage_Done = 3;
				return;
			case 4:
				SendRc5(0, 0, LIGHTS_OUT);
				stepOfStage_Done = 4;
				return;
			case 5:
				SendRc5(0, 0, DISSELECT_ALL_SEGMENTS);
				stepOfStage_Done = 5;
				return;
			case 6:
				SendRc5(0, 0, DISSELECT_ALL_SEGMENTS);
				stepOfStage_Done = 6;
				return;
			case 7:
				SendRc5(0, 0, SEGMENT_6);
				stepOfStage_Done = 7;
				return;
			case 8:
				SendRc5(0, 0, SEGMENT_6);
				stepOfStage_Done = 8;
				return;
		}

	if(stepOfStage_To_Do < 487)
	{
		if(stepOfStage_Done == 0) SendRc5(0, 0, RED);
		if(stepOfStage_Done == 1) SendRc5(0, 0, LIGHT_PLUS);

		if(stepOfStage_Done == 2) SendRc5(0, 0, GREEN);
		if(stepOfStage_Done == 3) SendRc5(0, 0, LIGHT_PLUS);

		if(stepOfStage_Done == 4) SendRc5(0, 0, BLUE);
		if(stepOfStage_Done == 5) SendRc5(0, 0, LIGHT_PLUS);
	}
	else
	{
		stageOfWakingUp = ALARMED_DRASTIC_ONE_SEGMET_LIGHT_VARIATIONS;
		millis_Alarmed = millis;
		stepOfStage_Done = 0;
		stepOfStage_To_Do = 0;
	}

	stepOfStage_Done = stepOfStage_To_Do%6;
	}
}

static void Alarmed_Drastic_One_Segment_Light_UP()
{
	//wait till we can send next rc5 signal
	if(millis - millis_Alarmed > 200)
	{
		stepOfStage_To_Do++;
		millis_Alarmed = millis;
	}

	if(stepOfStage_To_Do - stepOfStage_Done != 0 && stepOfStage_To_Do - stepOfStage_Done != 200 && stepOfStage_To_Do - stepOfStage_Done != 400)
	switch (stepOfStage_To_Do){
		case 1:
		case 201:
		case 401:
			SendRc5(0, 0, LIGHTS_OUT);
			stepOfStage_Done = 1;
			return;
		case 2:
		case 202:
		case 402:
			SendRc5(0, 0, DISSELECT_ALL_SEGMENTS);
			stepOfStage_Done = 2;
			return;
		case 3:
		case 203:
		case 403:
			SendRc5(0, 0, GREEN);
			stepOfStage_Done = 3;
			return;
		case 4:
		case 204:
		case 404:
			SendRc5(0, 0, LIGHT_MINUS);
			stepOfStage_Done = 4;
			return;
		case 5:
		case 205:
		case 405:
			SendRc5(0, 0, LIGHT_MINUS);
			stepOfStage_Done = 5;
			return;
		case 6:
		case 206:
		case 406:
			SendRc5(0, 0, RED);
			stepOfStage_Done = 6;
			return;
		case 7:
		case 207:
		case 407:
			SendRc5(0, 0, LIGHT_MINUS);
			stepOfStage_Done = 7;
			return;
		case 8:
		case 208:
		case 408:
			SendRc5(0, 0, BLUE);
			stepOfStage_Done = 8;
			return;
		case 9:
		case 209:
		case 409:
			SendRc5(0, 0, LIGHT_MINUS);
			stepOfStage_Done = 9;
			return;
		case 10:
		case 210:
		case 410:
			SendRc5(0, 0, SEGMENT_6); //drastic light up
			stepOfStage_Done = 10;
			return;
		case 200:
		case 400:
		case 600:
			SendRc5(0, 0, LIGHTS_OUT);
			stepOfStage_Done = 200;
			if(stepOfStage_To_Do == 600)
			{
				stageOfWakingUp = ALARMED_DRASTIC_LIGHT_VARIATIONS;
				millis_Alarmed = millis;
				stepOfStage_Done = 0;
				stepOfStage_To_Do = 0;
			}
			return;
	}
}

static void Alarmed_Drastic_Light_UP()
{
	//wait till we can send next rc5 signal
	if(millis - millis_Alarmed > 350)
	{
		stepOfStage_To_Do++;
		millis_Alarmed = millis;
	}

	if(stepOfStage_To_Do - stepOfStage_Done >0 && stepOfStage_To_Do - stepOfStage_Done != 0 && stepOfStage_To_Do - stepOfStage_Done != 200 && stepOfStage_To_Do - stepOfStage_Done != 400)
	switch (stepOfStage_To_Do){
		case 1:
		case 201:
		case 401:
			SendRc5(0, 0, LIGHTS_OUT);
			stepOfStage_Done = 1;
			return;
		case 2:
		case 202:
		case 402:
			SendRc5(0, 0, DISSELECT_ALL_SEGMENTS);
			stepOfStage_Done = 2;
			return;
		case 3:
		case 203:
		case 403:
			SendRc5(0, 0, RED);
			stepOfStage_Done = 3;
			return;
		case 4:
		case 204:
		case 404:
			SendRc5(0, 0, LIGHT_MINUS);
			stepOfStage_Done = 4;
			return;
		case 5:
		case 205:
		case 405:
			SendRc5(0, 0, LIGHT_MINUS);
			stepOfStage_Done = 5;
			return;
		case 6:
		case 206:
		case 406:
			SendRc5(0, 0, GREEN);
			stepOfStage_Done = 6;
			return;
		case 7:
		case 207:
		case 407:
			SendRc5(0, 0, LIGHT_MINUS);
			stepOfStage_Done = 7;
			return;
		case 8:
		case 208:
		case 408:
			SendRc5(0, 0, BLUE);
			stepOfStage_Done = 8;
			return;
		case 9:
		case 209:
		case 409:
			SendRc5(0, 0, LIGHT_MINUS);
			stepOfStage_Done = 9;
			return;
		case 10:
		case 210:
		case 410:
			SendRc5(0, 0, SELECT_ALL_SEGMENTS); //drastic light up
			stepOfStage_Done = 10;
			return;
		case 150:
		case 350:
		case 550:
			SendRc5(0, 0, LIGHTS_OUT);
			stepOfStage_Done = 200;
			if(stepOfStage_To_Do == 550)
			{
				stageOfWakingUp = ALARMED_TURN_OFF;
				millis_Alarmed = millis;
				stepOfStage_Done = 0;
				stepOfStage_To_Do = 0;
			}
			return;
	}
}
static void Alarmed_Turn_OFF()
{

	//wait till we can send next rc5 signal
	if(millis - millis_Alarmed > 120)
	{
		stepOfStage_To_Do++;
		millis_Alarmed = millis;


	if(stepOfStage_To_Do != stepOfStage_Done)
		switch (stepOfStage_To_Do)
		{
			case 1:
				SendRc5(0, 0, SELECT_ALL_SEGMENTS);
				stepOfStage_Done = 1;
				return;
			case 2:
				SendRc5(0, 0, SELECT_ALL_SEGMENTS);
				stepOfStage_Done = 2;
				return;
			case 3:
				SendRc5(0, 0, LIGHTS_OUT);
				stepOfStage_Done = 3;
				return;
			case 4:
				SendRc5(0, 0, LIGHTS_OUT);
				stepOfStage_Done = 4;
				return;
			case 5:
				SendRc5(0, 0, DISSELECT_ALL_SEGMENTS);
				stepOfStage_Done = 5;
				return;
			case 6:
				SendRc5(0, 0, DISSELECT_ALL_SEGMENTS);
				stepOfStage_Done = 6;
				return;
			case 7:
				NixieClock = IDLE_CLOCK;
				stageOfWakingUp = ALARMED_ACKNOWLEDGMENT;
				stepOfStage_Done = 0;
				stepOfStage_To_Do = 0;
				return;
		}
	}
}
