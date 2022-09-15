/*
 * utils.h
 *
 *  Created on: 17 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_UTILS_H_
#define LIBRARIES_UTILS_H_


//LEDs
#define LED_INIT DDRD |=  (1<<PD5)
#define LED_ON   PORTD &=~ (1<<PD5)
#define LED_OFF  PORTD |=  (1<<PD5)
#define LED_TOG  PORTD ^=  (1<<PD5)

void Led_Init();
void BlinkLed();


//Interrupts
void Timer0_Init();//start counting millis
extern uint64_t millis;

//Buttons
#define SW_0_IS_PUSHED !(PIND & (1<<PD7))
#define SW_1_IS_PUSHED !(PIND & (1<<PD6))

#define SW_0_PULL_UP PORTD |= (1<<PD7)
#define SW_1_PULL_UP PORTD |= (1<<PD6)


#define DEBOUNCE_TIME 20 //ms;
#define LONG_PRESS_TIME 2000 //ms;  time to press turns to long press
#define LONG_PRESS_TIME_CYCLE 60 //ms;

typedef enum
{
	IDLE = 0,
	DEBOUNCE,
	PRESSED,
	LONG_PRESS,
	RELEASED

}SWITCH_STATE;

typedef struct
{
	SWITCH_STATE switchState;
	uint8_t switchNumber;

	uint8_t counterData;

	uint32_t timerDebounce;
	uint32_t timerPress;
	uint32_t timerLongPress;

}SWITCH;

extern SWITCH Switch_0;
extern SWITCH Switch_1;
void SwitchInit(SWITCH *switch_0, SWITCH *switch_1);
void StateMachine(SWITCH *switch_);


#endif /* LIBRARIES_UTILS_H_ */
