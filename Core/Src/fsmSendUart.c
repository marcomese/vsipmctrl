/*
 * fsmSendUart.c
 *
 *  Created on: 16 ago 2021
 *      Author: mames
 */

#include "fsmSendUart.h"
#include "configurations.h"

enum sendInputNames{
    uartDir,
    pckToSendNum,
    dataSentIn
};

enum sendOutputNames{
    pckToSendIndex,
    sendCommand,
    dataSentOut
};

// Macchina a stati
void fsmSendIDLE(fsm_t* s){
    FSM_OUT(s,sendCommand,uint8_t) = NOOP;

    if(FSM_IN(s,pckToSendNum,uint8_t) > 0)
        FSM_STATE(s) = fsmSendUART;
    else
        FSM_STATE(s) = fsmSendIDLE;
}

void fsmSendUART(fsm_t* s){
    FSM_OUT(s,sendCommand,uint8_t) = SENDTOUART;

    FSM_STATE(s) = fsmSendWait;
}

void fsmSendWait(fsm_t* s){
    FSM_OUT(s,sendCommand,uint8_t) = NOOP;

    if(FSM_IN(s,dataSentIn,uint8_t) == 1)
        FSM_STATE(s) = fsmSendNextPacket;
    else
        FSM_STATE(s) = fsmSendWait;
}

void fsmSendNextPacket(fsm_t* s){
    FSM_OUT(s,sendCommand,uint8_t) = NOOP;
    FSM_OUT(s,pckToSendIndex,uint8_t) = (++FSM_OUT(s,pckToSendIndex,uint8_t) < PACKETSINBUF) ?
                                          FSM_OUT(s,pckToSendIndex,uint8_t) : 0;
    FSM_OUT(s,dataSentOut,uint8_t) = 0;

    FSM_STATE(s) = fsmSendIDLE;
}
