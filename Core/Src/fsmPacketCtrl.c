/*
 * fsmPacketCtrl.c
 *
 *  Created on: May 1, 2021
 *      Author: mames
 */

#include "fsmPacketCtrl.h"
#include "configurations.h"

enum pcktInputNames{
    packetsNum,
    packetProcessed
};

enum pcktOutputNames{
    process,
    packetReadIndex
};

void packetCtrlIDLE(fsm_t* s){
    FSM_OUT(s,process,uint8_t) = 0;

    if(FSM_IN(s,packetsNum,uint8_t) > 0)
        FSM_STATE(s) = packetCtrlSelect;
    else
        FSM_STATE(s) = packetCtrlIDLE;
}

void packetCtrlSelect(fsm_t* s){
    FSM_OUT(s,process,uint8_t) = 1;

    FSM_STATE(s) = packetCtrlWaitProcessed;
}

void packetCtrlWaitProcessed(fsm_t* s){
    FSM_OUT(s,process,uint8_t) = 0;

    if(FSM_IN(s,packetProcessed,uint8_t) == 1)
        FSM_STATE(s) = packetCtrlNext;
    else
        FSM_STATE(s) = packetCtrlWaitProcessed;
}

void packetCtrlNext(fsm_t* s){
    FSM_OUT(s,process,uint8_t) = 0;
    FSM_OUT(s,packetReadIndex,uint8_t) = (++FSM_OUT(s,packetReadIndex,uint8_t) < PACKETSINBUF) ?
                                          FSM_OUT(s,packetReadIndex,uint8_t) : 0;

    if(FSM_IN(s,packetsNum,uint8_t) > 0)
        FSM_STATE(s) = packetCtrlSelect;
    else
        FSM_STATE(s) = packetCtrlIDLE;
}
