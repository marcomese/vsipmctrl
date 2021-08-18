/*
 * fsmSendUart.h
 *
 *  Created on: 16 ago 2021
 *      Author: mames
 */

#ifndef INC_FSMSENDUART_H_
#define INC_FSMSENDUART_H_

#include "fsm.h"
#include "stm32f3xx_hal.h"

/*
 * Macchina a stati per gestire i pacchetti in uscita verso le UART
 */

void fsmSendIDLE(fsm_t* s);
void fsmSendUART(fsm_t* s);
void fsmSendNextPacket(fsm_t* s);
void fsmSendWait(fsm_t* s);

#endif /* INC_FSMSENDUART_H_ */
