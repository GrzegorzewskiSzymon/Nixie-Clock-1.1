/*
 * AnodesMultiplexer.h
 *
 *  Created on: 18 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_NIXIEDRIVERS_ANODESMULTIPLEXER_H_
#define LIBRARIES_NIXIEDRIVERS_ANODESMULTIPLEXER_H_


#define ANODES_INIT DDRD |= 0b00000011; DDRB |= 0b10000001

#define NIXIE_3_ON   PORTB |=  (1<<PB0)
#define NIXIE_3_OFF  PORTB &=~ (1<<PB0)

#define NIXIE_2_ON   PORTD |=  (1<<PD7)
#define NIXIE_2_OFF  PORTD &=~ (1<<PD7)

#define NIXIE_1_ON   PORTB |=  (1<<PB7)
#define NIXIE_1_OFF  PORTB &=~ (1<<PB7)

#define NIXIE_0_ON   PORTD |=  (1<<PD6)
#define NIXIE_0_OFF  PORTD &=~ (1<<PD6)

void Anodes_Init();


#endif /* LIBRARIES_NIXIEDRIVERS_ANODESMULTIPLEXER_H_ */
