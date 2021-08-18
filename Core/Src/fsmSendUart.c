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
    pckToSendNum
};

enum sendOutputNames{
    pckToSendIndex,
    sendCommand
};

// Funzioni di supporto

// Macchina a stati
void fsmSendIDLE(fsm_t* s){
    FSM_OUT(s,sendCommand,uint8_t) = NOOP;

    if(FSM_IN(s,pckToSendNum,uint8_t) > 0){
        FSM_STATE(s) = fsmSendUART;
    }else{
        FSM_STATE(s) = fsmSendIDLE;
    }
}

void fsmSendUART(fsm_t* s){
    FSM_OUT(s,sendCommand,uint8_t) = SENDTOUART;

    FSM_STATE(s) = fsmSendNextPacket;
}

void fsmSendNextPacket(fsm_t* s){
    FSM_OUT(s,sendCommand,uint8_t) = NOOP;
    FSM_OUT(s,pckToSendIndex,uint8_t) = (++FSM_OUT(s,pckToSendIndex,uint8_t) < PACKETSINBUF) ?
                                          FSM_OUT(s,pckToSendIndex,uint8_t) : 0;

    FSM_STATE(s) = fsmSendIDLE;
}
