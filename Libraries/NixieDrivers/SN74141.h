/*
 * SN74141.h
 *
 *  Created on: 18 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_NIXIEDRIVERS_SN74141_H_
#define LIBRARIES_NIXIEDRIVERS_SN74141_H_

#define BCD_INIT DDRD |= 0b00001111

#define DISPLAY_0 PORTD &=~ 0b00001111
#define DISPLAY_1 PORTD &=~ 0b00001111; PORTD |= 0b00000001
#define DISPLAY_2 PORTD &=~ 0b00001111; PORTD |= 0b00000010
#define DISPLAY_3 PORTD &=~ 0b00001111; PORTD |= 0b00000011
#define DISPLAY_4 PORTD &=~ 0b00001111; PORTD |= 0b00000100
#define DISPLAY_5 PORTD &=~ 0b00001111; PORTD |= 0b00000101
#define DISPLAY_6 PORTD &=~ 0b00001111; PORTD |= 0b00000110
#define DISPLAY_7 PORTD &=~ 0b00001111; PORTD |= 0b00000111
#define DISPLAY_8 PORTD &=~ 0b00001111; PORTD |= 0b00001000
#define DISPLAY_9 PORTD &=~ 0b00001111; PORTD |= 0b00001001

void BCD_Init();
void DisplayTime(int8_t hours, int8_t minutes);//This function overwrite whole PORTD which could lead to problems in future

#endif /* LIBRARIES_NIXIEDRIVERS_SN74141_H_ */
