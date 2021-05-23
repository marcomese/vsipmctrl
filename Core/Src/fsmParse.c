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
                 outVoltagePointer,
                 vSection};

#define NOOP 0
#define IDNCMD 1
#define VOLTCMD 2
#define STATECMD 3
#define MAXCMD 4
#define HVCMD 5
#define DVCMD 6
#define PULSERATECMD 7

#define NODESECT "NODE:"
#define IDNSECT "*IDN?"
#define BIASSECT "BIAS"
#define KATODESECT "KATODE"
#define MONITORSECT "MONITOR"

#define NODELEN (sizeof(NODESECT)-1)
#define IDNLEN (sizeof(IDNSECT)-1)
#define BIASLEN (sizeof(BIASSECT)-1)
#define KATODELEN (sizeof(KATODESECT)-1)
#define MONITORLEN (sizeof(MONITORSECT)-1)

#define VOLTCMDSTR "VOLT"
#define MAXCMDSTR "MAX"
#define STATECMDSTR "STATE"

#define VOLTLEN sizeof(VOLTCMD)
#define MAXLEN sizeof(MAXCMD)
#define STATELEN sizeof(STATECMD)


// Funzioni di supporto
state_function_t selectCmd(const char* arg){
    char* argEnd = strpbrk(arg," ?");

    size_t argL = (argEnd > 0) ? argEnd-arg : sizeof(argEnd)-1;

    if(strncmp(arg,VOLTCMDSTR,argL) == 0){
        if(*argEnd == ' ')
            return parseVoltageCmd;
        else if(*argEnd == '?')
            return parseVoltageQuery;
        else
            return parseErrCmd;
    }else if(strncmp(arg,MAXCMDSTR,argL) == 0){
        if(*argEnd == ' ')
            return parseMaxCmd;
        else if(*argEnd == '?')
            return parseMaxQuery;
        else
            return parseErrCmd;
    }else if(strncmp(arg,STATECMDSTR,argL) == 0){
        if(*argEnd == ' ')
            return parseStateCmd;
        else if(*argEnd == '?')
            return parseStateQuery;
        else
            return parseErrCmd;
    }
    return parseErrCmd;
}


// Macchina a stati
void parseIDLE(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 0;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    if(FSM_IN(s,process,uint8_t))
        FSM_STATE(s) = parseNode;
    else
        FSM_STATE(s) = parseIDLE;
}

void parseNode(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    if(strncmp((const char*)pack,NODESECT,NODELEN) == 0){
        FSM_STATE(s) = parseAddr;
    }else if(strncmp((const char*)pack,IDNSECT,IDNLEN) == 0){
        FSM_STATE(s) = parseCmdIDN;
    }else{
        FSM_STATE(s) = parseErrNode;
    }
}

void parseAddr(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* addrStr = (const char*)pack+NODELEN;

    if(strncmp(addrStr,ADDR,ADDRLEN) == 0){
        FSM_STATE(s) = parseSection;
    }else{
        FSM_STATE(s) = parseSendToAddr;
    }
}

void parseSection(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* sectStr = (const char*)pack+NODELEN+ADDRLEN+1;

    char* endSect = strpbrk(sectStr,":");

    uint8_t secL = endSect-sectStr;

    if(secL > 0 && strncmp(sectStr,BIASSECT,secL) == 0){
        FSM_STATE(s) = parseCmdBIAS;
    }else if(secL > 0 && strncmp(sectStr,KATODESECT,secL) == 0){
        FSM_STATE(s) = parseCmdKAT;
    }else if(secL > 0 && strncmp(sectStr,MONITORSECT,secL) == 0){
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
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;
    FSM_OUT(s,vSection,uint8_t) = BIAS;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* biasArg = (const char*)pack+NODELEN+ADDRLEN+BIASLEN+2;

    FSM_STATE(s) = selectCmd(biasArg);

}

void parseCmdKAT(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 0;
    FSM_OUT(s,vSection,uint8_t) = KATODE;

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* katArg = (const char*)pack+NODELEN+ADDRLEN+KATODELEN+2;

    FSM_STATE(s) = selectCmd(katArg);

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

void parseVoltageCmd(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = VOLTCMD;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    uint8_t vSect = FSM_OUT(s,vSection,uint8_t);
    uint8_t sectLen = BIASLEN*(1-vSect)+KATODELEN*vSect;

    FSM_OUT(s,outVoltagePointer,float*) = (vSect == BIAS) ?
                                           FSM_INP(s,biasVoltage) : FSM_INP(s,katodeVoltage);

    char* argOut = FSM_OUTP(s,argument);
    memset(argOut,0,CMDARGSIZE);

    uint8_t* pack = FSM_IN(s,currPacket,uint8_t*);

    const char* voltArg = (const char*)pack+NODELEN+ADDRLEN+sectLen+VOLTLEN+3;

    char* argEnd = strpbrk(voltArg,":;\n\r");

    size_t argL = (argEnd > 0) ? argEnd-voltArg : sizeof(argEnd)-1;

    strncpy(argOut,voltArg,argL);

    FSM_STATE(s) = parseIDLE;

}

void parseMaxCmd(fsm_t* s){
    return;
}

void parseStateCmd(fsm_t* s){
    return;
}

void parseVoltageQuery(fsm_t* s){
    return;
}

void parseMaxQuery(fsm_t* s){
    return;
}

void parseStateQuery(fsm_t* s){
    return;
}

void parseExecute(fsm_t* s){
    return;
}

void parseErrNode(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseIDLE;
}

void parseErrSection(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseIDLE;
}

void parseErrCmd(fsm_t* s){
    FSM_OUT(s,command,uint8_t) = NOOP;
    FSM_OUT(s,busy,uint8_t) = 1;
    FSM_OUT(s,packetProcessed,uint8_t) = 1;

    FSM_STATE(s) = parseIDLE;
}
