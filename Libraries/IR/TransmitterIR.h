/*
 * TransmitterIR.h
 *
 *  Created on: 4 lip 2022
 *      Author: Grzegorzewski Szymon
 */

#ifndef LIBRARIES_IR_TRANSMITTERIR_H_
#define LIBRARIES_IR_TRANSMITTERIR_H_

#define INIT_IR_DIODE DDRC |= (1<<PC0)

#define IR_DIODE_TOG PORTC ^=  (1<<PC0)

void InitTransmitterIr();
void SendRc5(uint8_t adr, uint8_t tog, uint8_t cmd);

//buttons with corresponding commands for 3975# pilot
#define RED   43
#define GREEN  44
#define BLUE   46

#define SEGMENT_1  1
#define SEGMENT_2  2
#define SEGMENT_3  3
#define SEGMENT_4  4
#define SEGMENT_5  5
#define SEGMENT_6  6

#define LIGHT_PLUS   16
#define LIGHT_MINUS   17


#define SELECT_ALL_SEGMENTS    34
#define DISSELECT_ALL_SEGMENTS  15

#define LIGHTS_OUT  12


#endif /* LIBRARIES_IR_TRANSMITTERIR_H_ */
