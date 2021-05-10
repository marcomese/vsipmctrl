/*
 * fsmPacketCtrl.h
 *
 *  Created on: May 1, 2021
 *      Author: mames
 */

#ifndef INC_FSMPACKETCTRL_H_
#define INC_FSMPACKETCTRL_H_

#include "fsm.h"

/*
 * Macchina a stati per gestire i pacchetti in arrivo dalla UART e da mandare a fsmParse
 */

void packetCtrlIDLE(fsm_t* s);
void packetCtrlSelect(fsm_t* s);
void packetCtrlWaitProcessed(fsm_t* s);
void packetCtrlNext(fsm_t* s);

#endif /* INC_FSMPACKETCTRL_H_ */
