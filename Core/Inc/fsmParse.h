/*
 * fsmParse.h
 *
 *  Created on: Apr 30, 2021
 *      Author: mames
 */

#ifndef INC_FSMPARSE_H_
#define INC_FSMPARSE_H_

#include "fsm.h"

/*
 * Macchina a stati per fare il parsing della stringa proveniente dalla seriale
 */


void parseIDLE(fsm_t* s);
void parseNode(fsm_t* s);
void parseAddr(fsm_t* s);
void parseSection(fsm_t* s);
void parseCmdIDN(fsm_t* s);
void parseCmdBIAS(fsm_t* s);
void parseCmdKAT(fsm_t* s);

void parseVoltageCmd(fsm_t* s);
void parseMaxCmd(fsm_t* s);

void parseExecute(fsm_t* s);
void parseSendToAddr(fsm_t* s);
void parseErrNode(fsm_t* s);
void parseErrSection(fsm_t* s);
void parseErrCmd(fsm_t* s);

void parseNextPacket(fsm_t* s);

#endif /* INC_FSMPARSE_H_ */
