/*
 * commands.h
 *
 *  Created on: May 2, 2021
 *      Author: mames
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

#include "configurations.h"
#include "stm32f3xx_hal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void execute(void);

void noop(void);

void idnCMD(void);

void voltageCMD(void);

void maxCMD(void);

typedef void (*commands_t)(void);

extern const commands_t commandExecute[];

#endif /* INC_COMMANDS_H_ */
