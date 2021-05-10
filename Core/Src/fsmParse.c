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
                katodeMaxVoltage};

enum outputNames{command,
                 busy,
                 argument,
                 packetProcessed,
                 outVoltagePointer};

#define NOOP 0
#define IDNCMD 1
#define VOLTCMD 2
#define STATECMD 3
#define MAXCMD 4
#define HVCMD 5
#define DVCMD 6
#define PULSERATECMD 7

#define NODESECT (const char*) "NODE:"
#define IDNSECT (const char*) "*IDN?"
#define BIASSECT (const char*) "BIAS"
#define KATODESECT (const char*) "KATODE"
#define MONITORSECT (const char*) "MONITOR"

#define IDNLEN 5
#define NODELEN 5
#define BIASLEN sizeof(BIASSECT)
#define KATODELEN sizeof(KATODESECT)
#define MONITORLEN sizeof(MONITORSECT)

void parseIDLE(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = 0;
    FSM_OUT(s,busy,uint8_t) = 0;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    if(FSM_IN(s,process,uint8_t))
        FSM_STATE(s) = parseNode;
    else
        FSM_STATE(s) = parseIDLE;
}

void parseNode(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = 0;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    if(!strncmp((const char*)pack,NODESECT,NODELEN)){
        FSM_STATE(s) = parseAddr;
    }else if(!strncmp((const char*)pack,IDNSECT,IDNLEN)){
        FSM_STATE(s) = parseCmdIDN;
    }else{
        FSM_STATE(s) = parseErrNode;
    }
}

void parseAddr(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = 0;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* addrStr = (const char*)pack+NODELEN;

    if(!strncmp(addrStr,ADDR,ADDRLEN-1)){
        FSM_STATE(s) = parseSection;
    }else{
        FSM_STATE(s) = parseSendToAddr;
    }
}

void parseSection(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = 0;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* sectStr = (const char*)pack+NODELEN+ADDRLEN;

    char* endSect = strpbrk(sectStr,":");

    uint8_t secL = endSect-sectStr;

    if(!strncmp(sectStr,BIASSECT,secL)){
        FSM_STATE(s) = parseCmdBIAS;
    }else if(!strncmp(sectStr,KATODESECT,secL)){
        FSM_STATE(s) = parseCmdKAT;
    }else if(!strncmp(sectStr,MONITORSECT,secL)){
        FSM_STATE(s) = parseCmdMON;
    }else{
        FSM_STATE(s) = parseErrSection;
    }
}

void parseCmdIDN(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = IDNCMD;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseIDLE;
}

void parseCmdBIAS(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = VOLTCMD;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;
////////// DA SPOSTARE!!!
    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* biasArg = (const char*)pack+NODELEN+ADDRLEN+BIASLEN+1;

    char* argEnd = strpbrk(biasArg,":;");

    size_t argL = (argEnd > 0) ? argEnd-biasArg : sizeof(argEnd)-1;
/*
 * Se il pacchetto non termina con ; o : vuol dire che è l'ultimo (dato che i caratteri terminatori vengono rimossi per evitare loop)
 * quindi in questo caso argL è semplicemente sizeof(argEnd)
 */

    FSM_OUT(s,outVoltagePointer,float*) = FSM_INP(s,biasVoltage);

    char* argOut = FSM_OUTP(s,argument);
    memset(argOut,0,CMDARGSIZE);

    strncpy(argOut,biasArg,argL);

    FSM_STATE(s) = parseIDLE;

}

void parseErrNode(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseIDLE;
}

void parseCmdKAT(fsm_t* s){
    return;
}

void parseCmdMON(fsm_t* s){
    return;
}

void parseSendToAddr(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;//SENDTOADDR;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseIDLE;
}

void parseExecute(fsm_t* s){
    return;
}

void parseErrSection(fsm_t* s){
    return;
}

void parseErrCmd(fsm_t* s){
    return;
}
