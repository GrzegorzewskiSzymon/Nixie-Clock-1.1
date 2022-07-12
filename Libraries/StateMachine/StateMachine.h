/*
 * StateMachine.h
 *
 *  Created on: 9 lip 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_STATEMACHINE_STATEMACHINE_H_
#define LIBRARIES_STATEMACHINE_STATEMACHINE_H_

#define MAX_TIME_TO_CHECH_IN 900000

typedef enum
{
	IDLE_CLOCK = 0, //only displaying hour and waiting to change state
	SUSTAIN_CLOCK, //displaying hour and repetitively waiting to checking in by user till designated hour
	ALARMED_CLOCK //displaying hour and trying to wake me up using lights
}CLOCK_STATE;


typedef enum
{
	ALARMED_ACKNOWLEDGMENT = 0,
	ALARMED_SMOOTH_ONE_SEGMET_LIGHT_UP,
	ALARMED_DRASTIC_ONE_SEGMET_LIGHT_VARIATIONS,
	ALARMED_DRASTIC_LIGHT_VARIATIONS,
	ALARMED_TURN_OFF
}STAGES_OF_WAKING_UP;

void StateMachine_Clock();

extern uint8_t seconds, minutes, hours;

#endif /* LIBRARIES_STATEMACHINE_STATEMACHINE_H_ */
