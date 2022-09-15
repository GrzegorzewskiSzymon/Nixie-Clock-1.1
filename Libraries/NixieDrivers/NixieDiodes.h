/*
 * NixieDiodes.h
 *
 *  Created on: 29 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_NIXIEDRIVERS_NIXIEDIODES_H_
#define LIBRARIES_NIXIEDRIVERS_NIXIEDIODES_H_

#define DIODES_BLINK_TIME 3000 //ms

#define NIXIEDIODES_INIT DDRD |= (1<<PD0)

#define NIXIEDIODES_ON  PORTD |=  (1<<PD0)
#define NIXIEDIODES_OFF PORTD &=~ (1<<PD0)
#define NIXIEDIODES_TOG PORTD ^=  (1<<PD0)

void NixieDiodesInit();

#endif /* LIBRARIES_NIXIEDRIVERS_NIXIEDIODES_H_ */
