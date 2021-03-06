/*
 * fsmParse.c
 *
 *  Created on: Apr 30, 2021
 *      Author: mames
 */

#include "fsmParse.h"
#include "configurations.h"
#include <string.h>

enum inputNames{currPacket,
                process,
                biasVoltage,
                biasMaxVoltage,
                katodeVoltage,
                katodeMaxVoltage,
                biasReadVal,
                katodeReadVal};

enum outputNames{command,
                 argument,
                 packetsNum,
                 packetProcessed,
                 uartDir,
                 outVoltagePointer,
                 readVoltagePointer,
                 maxVoltagePointer,
                 vSection};

// Costanti di supporto
const char uartDirVals[2] = {UART1,UART2};

// Funzioni di supporto
state_function_t selectCmd(const char* arg){
    char* argEnd = strpbrk(arg," ?");

    size_t argL = (argEnd > 0) ? argEnd-arg : sizeof(argEnd)-1;

    if(strncmp(arg,VOLTCMDSTR,argL) == 0)
        return parseVoltageCmd;
    else if(strncmp(arg,MAXCMDSTR,argL) == 0)
        return parseMaxCmd;
    else if(strncmp(arg,HVCMDSTR,argL) == 0)
        return parseBiasHVCmd;

    return parseErrCmd;
}

// Macchina a stati
void parseIDLE(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    if(FSM_IN(s,process,uint8_t))
        FSM_STATE(s) = parseNode;
    else
        FSM_STATE(s) = parseIDLE;
}

void parseNode(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    if(strncmp((const char*)pack,NODESECT,NODELEN) == 0)
        FSM_STATE(s) = parseAddr;
    else if(strncmp((const char*)pack,IDNSECT,IDNLEN) == 0)
        FSM_STATE(s) = parseCmdIDN;
    else if(strncmp((const char*)pack,RESP,RESPLEN) == 0)
        FSM_STATE(s) = parseSendToAddr;
    else if(strncmp((const char*)pack,ERR,ERRLEN) == 0)
        FSM_STATE(s) = parseSendToAddr;
    else
        FSM_STATE(s) = parseErrNode;
}

void parseAddr(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* addrStr = (const char*)pack+NODELEN;

    if(strncmp(addrStr,ADDR,ADDRLEN) == 0)
        FSM_STATE(s) = parseSection;
    else
        FSM_STATE(s) = parseSendToAddr;
}

void parseSection(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* sectStr = (const char*)pack+NODELEN+ADDRLEN+1;

    char* endSect = strpbrk(sectStr,":");

    uint8_t secL = endSect-sectStr;

    if(secL > 0 && strncmp(sectStr,BIASSECT,secL) == 0)
        FSM_STATE(s) = parseCmdBIAS;
    else if(secL > 0 && strncmp(sectStr,KATODESECT,secL) == 0)
        FSM_STATE(s) = parseCmdKAT;
    else
        FSM_STATE(s) = parseErrSection;
}

void parseCmdIDN(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = IDNCMD;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    char* idnEnd = strpbrk((const char*)pack,uartDirVals);

    FSM_OUT(s,uartDir,uint8_t) = *idnEnd;

    FSM_STATE(s) = parseNextPacket;
}

void parseCmdBIAS(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;
    FSM_OUT(s,vSection,uint8_t) = BIAS;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* biasArg = (const char*)pack+NODELEN+ADDRLEN+BIASLEN+2;

    FSM_STATE(s) = selectCmd(biasArg);
}

void parseCmdKAT(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;
    FSM_OUT(s,vSection,uint8_t) = KATODE;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* katArg = (const char*)pack+NODELEN+ADDRLEN+KATODELEN+2;

    FSM_STATE(s) = selectCmd(katArg);
}

void parseSendToAddr(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = SENDTOADDRCMD;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    char* pckEnd = strpbrk((const char*)pack,uartDirVals);

    FSM_OUT(s,uartDir,uint8_t) = (*pckEnd == UART1) ?
                                  UART2 : UART1;

    FSM_STATE(s) = parseNextPacket;
}

void parseVoltageCmd(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = VOLTCMD;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    uint8_t vSect = FSM_OUT(s,vSection,uint8_t);
    uint8_t sectLen = BIASLEN*(1-vSect)+KATODELEN*vSect;

    FSM_OUT(s,outVoltagePointer,float*) = (vSect == BIAS) ?
                                           FSM_INP(s,biasVoltage) : FSM_INP(s,katodeVoltage);

    FSM_OUT(s,readVoltagePointer,float*) = (vSect == BIAS) ?
                                           FSM_INP(s,biasReadVal) : FSM_INP(s,katodeReadVal);

    FSM_OUT(s,maxVoltagePointer,float*) = (vSect == BIAS) ?
                                           FSM_INP(s,biasMaxVoltage) : FSM_INP(s,katodeMaxVoltage);


    char* argOut = FSM_OUTP(s,argument);
    memset(argOut,0,CMDARGSIZE);

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* voltArg = (const char*)pack+NODELEN+ADDRLEN+sectLen+VOLTLEN+2;

    char* argEnd = strpbrk(voltArg,uartDirVals);

    FSM_OUT(s,uartDir,uint8_t) = *argEnd;

    size_t argL = (argEnd > 0) ? argEnd-voltArg : sizeof(argEnd)-1;

    strncpy(argOut,voltArg,argL);

    FSM_STATE(s) = parseNextPacket;
}

void parseMaxCmd(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = MAXCMD;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    uint8_t vSect = FSM_OUT(s,vSection,uint8_t);
    uint8_t sectLen = BIASLEN*(1-vSect)+KATODELEN*vSect;

    FSM_OUT(s,maxVoltagePointer,float*) = (vSect == BIAS) ?
                                           FSM_INP(s,biasMaxVoltage) : FSM_INP(s,katodeMaxVoltage);

    char* argOut = FSM_OUTP(s,argument);
    memset(argOut,0,CMDARGSIZE);

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* voltArg = (const char*)pack+NODELEN+ADDRLEN+sectLen+MAXLEN+2;

    char* argEnd = strpbrk(voltArg,uartDirVals);

    FSM_OUT(s,uartDir,uint8_t) = *argEnd;

    size_t argL = (argEnd > 0) ? argEnd-voltArg : sizeof(argEnd)-1;

    strncpy(argOut,voltArg,argL);

    FSM_STATE(s) = parseNextPacket;
}

void parseBiasHVCmd(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = HVCMD;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    uint8_t vSect = FSM_OUT(s,vSection,uint8_t);
    uint8_t sectLen = BIASLEN*(1-vSect)+KATODELEN*vSect;

    char* argOut = FSM_OUTP(s,argument);
    memset(argOut,0,CMDARGSIZE);

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* hvArg = (const char*)pack+NODELEN+ADDRLEN+sectLen+HVLEN+2;

    char* argEnd = strpbrk(hvArg,uartDirVals);

    FSM_OUT(s,uartDir,uint8_t) = *argEnd;

    size_t argL = (argEnd > 0) ? argEnd-hvArg : sizeof(argEnd)-1;

    strncpy(argOut,hvArg,argL);

    FSM_STATE(s) = parseNextPacket;
}

void parseErrNode(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseNextPacket;
}

void parseErrSection(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseNextPacket;
}

void parseErrCmd(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseNextPacket;
}

void parseNextPacket(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;
    FSM_OUT(s,packetsNum,uint8_t) = FSM_OUT(s,packetsNum,uint8_t)-1;

    FSM_STATE(s) = parseIDLE;
}

