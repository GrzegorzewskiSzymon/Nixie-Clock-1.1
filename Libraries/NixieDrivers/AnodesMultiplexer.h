/*
 * AnodesMultiplexer.h
 *
 *  Created on: 18 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_NIXIEDRIVERS_ANODESMULTIPLEXER_H_
#define LIBRARIES_NIXIEDRIVERS_ANODESMULTIPLEXER_H_


#define ANODES_INIT DDRD |= 0b00000110;

//tens of hours/minutes/seconds
#define NIXIE_TENS_ON   PORTD |=  (1<<PD1)
#define NIXIE_TENS_OFF  PORTD &=~ (1<<PD1)

//units of hours/minutes/seconds
#define NIXIE_UNITS_ON   PORTD |=  (1<<PD2)
#define NIXIE_UNITS_OFF  PORTD &=~ (1<<PD2)

void Anodes_Init();


#endif /* LIBRARIES_NIXIEDRIVERS_ANODESMULTIPLEXER_H_ */
