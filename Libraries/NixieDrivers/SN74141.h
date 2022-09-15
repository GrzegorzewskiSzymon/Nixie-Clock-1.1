/*
 * SN74141.h
 *
 *  Created on: 18 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_NIXIEDRIVERS_SN74141_H_
#define LIBRARIES_NIXIEDRIVERS_SN74141_H_

#define BCD_INIT DDRB |= 0b11111111; DDRC |= 0b00001111


void BCD_Init();
void DisplayTime(int8_t hours, int8_t minutes, int8_t seconds);
#endif /* LIBRARIES_NIXIEDRIVERS_SN74141_H_ */
