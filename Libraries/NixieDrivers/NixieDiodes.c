/*
 * NixieDiodes.c
 *
 *  Created on: 29 cze 2022
 *      Author: Grzegorzewski Szymon
 */
#include <avr/io.h>
#include "NixieDiodes.h"

void NixieDiodesInit()
{
	NIXIEDIODE_0_INIT;
	NIXIEDIODE_1_INIT;
}
