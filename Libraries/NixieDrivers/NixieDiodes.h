/*
 * NixieDiodes.h
 *
 *  Created on: 29 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_NIXIEDRIVERS_NIXIEDIODES_H_
#define LIBRARIES_NIXIEDRIVERS_NIXIEDIODES_H_

#define NIXIEDIODE_0_INIT DDRB |= (1<<PB6)
#define NIXIEDIODE_1_INIT DDRD |= (1<<PD4)

#define NIXIEDIODE_0_ON PORTB |= (1<<PB6)
#define NIXIEDIODE_1_ON PORTD |= (1<<PD4)

#define NIXIEDIODE_0_OFF PORTB &=~ (1<<PB6)
#define NIXIEDIODE_1_OFF PORTD &=~ (1<<PD4)

void NixieDiodesInit();

#endif /* LIBRARIES_NIXIEDRIVERS_NIXIEDIODES_H_ */
