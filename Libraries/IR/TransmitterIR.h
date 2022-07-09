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
#endif /* LIBRARIES_IR_TRANSMITTERIR_H_ */


//buttons with corresponding commands for 3975# pilot

//Red    43
//Green  44
//Blue   46

//Segment_1  1
//Segment_2  2
//Segment_3  3
//Segment_4  4
//Segment_5  5
//Segment_6  6

//Light+   16
//Light-   17


//Select all segments    34
//Diselect all segments  15

//Lights out  12


