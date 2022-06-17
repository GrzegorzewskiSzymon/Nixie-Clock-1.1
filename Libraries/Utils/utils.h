/*
 * utils.h
 *
 *  Created on: 17 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_UTILS_H_
#define LIBRARIES_UTILS_H_


//LEDs
#define LED_INIT DDRC |=  (1<<PC2)
#define LED_ON   PORTC &=~ (1<<PC2)
#define LED_OFF  PORTC |=  (1<<PC2)
#define LED_TOG  PORTC ^=  (1<<PC2)

void Led_Init();
void BlinkLed();


//Interrupts
void Timer0_Init();//start counting millis
extern uint64_t millis;


#endif /* LIBRARIES_UTILS_H_ */
