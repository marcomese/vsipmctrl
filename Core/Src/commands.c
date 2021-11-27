/*
 * commands.c
 *
 *  Created on: May 2, 2021
 *      Author: mames
 */

#include "commands.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern DAC_HandleTypeDef hdac;

extern uint8_t* currPacket;

extern uint8_t uartDir;

extern uint8_t sendBuffer[BUFSIZE];
extern uint8_t* packetToSend[PACKETSINBUF];
extern uint8_t* sendPointer;
extern uint8_t pckToSendNum;
extern uint8_t sendBufIndex;
extern uint8_t pckToSendIndex;

extern char argument[CMDARGSIZE];

extern float* outVoltagePointer;
extern float* readVoltagePointer;
extern float* maxVoltagePointer;
extern float biasHVReadVal;

extern const char uartDirVals[2];

void putInSendBuf(uint8_t* dstBuf, uint8_t** dstBufPtr,
                  uint8_t* srcPtr, uint8_t srcLen,
                  uint8_t** sendPckArray, uint8_t* sendPckIndex, uint8_t* sendPckNum,
                  uint8_t uDir){
    if(*dstBufPtr+srcLen > &dstBuf[BUFSIZE-MAXPACKETSIZE])
        *dstBufPtr = dstBuf;

    uint8_t* startPtr = *dstBufPtr;

    sendPckArray[*sendPckIndex] = startPtr;

    *sendPckIndex = (++(*sendPckIndex) < PACKETSINBUF) ?
                     *sendPckIndex : 0;

    **dstBufPtr = uDir;
    (*dstBufPtr)++;
    **dstBufPtr = srcLen;
    (*dstBufPtr)++;

    while(*dstBufPtr < startPtr+srcLen+2){
        **dstBufPtr = *srcPtr++;
        (*dstBufPtr)++;
    }

    (*sendPckNum)++;
}

void noop(void){}

void idnCMD(void){
    uint8_t uartResp[UARTRESSIZE];
    uint8_t respLen = 0;

    snprintf((char*)uartResp,UARTRESSIZE,"%s%s\n",RESP,ADDR);
    respLen = strlen((const char*)uartResp);

    putInSendBuf(sendBuffer, &sendPointer,
                 (uint8_t*)uartResp, respLen,
                 packetToSend, &sendBufIndex, &pckToSendNum,
                 uartDir);
}

void voltageCMD(void){
    char* err;
    float outVal = strtof(argument,&err);
    uint8_t uartResp[UARTRESSIZE];
    uint8_t respLen = 0;

    memset(uartResp,0,UARTRESSIZE);

    if(err != argument)
        if(outVal <= *maxVoltagePointer){
            *outVoltagePointer = outVal;
            snprintf((char*)uartResp,UARTRESSIZE,"%s%.2f\n",RESP,outVal);
        }else
            snprintf((char*)uartResp,UARTRESSIZE,"%sVmax=%.2f\n",ERR,*maxVoltagePointer);
    else if(argument[0] == '?')
        snprintf((char*)uartResp,UARTRESSIZE,"%s%.2f\n",RESP,*readVoltagePointer);
    else
        snprintf((char*)uartResp,UARTRESSIZE,"%sNaN\n",ERR);

    respLen = strlen((const char*)uartResp);
    putInSendBuf(sendBuffer, &sendPointer,
                 (uint8_t*)uartResp, respLen,
                 packetToSend, &sendBufIndex, &pckToSendNum,
                 uartDir);
}

void maxCMD(void){
    char* err;
    float maxVal = strtof(argument,&err);
    uint8_t uartResp[UARTRESSIZE];
    uint8_t respLen = 0;

    memset(uartResp,0,UARTRESSIZE);

    if(err != argument){
        *maxVoltagePointer = maxVal;
        snprintf((char*)uartResp,UARTRESSIZE,"%s%.2f\n",RESP,maxVal);
    }else if(argument[0] == '?')
        snprintf((char*)uartResp,UARTRESSIZE,"%s%.2f\n",RESP,*maxVoltagePointer);
    else
        snprintf((char*)uartResp,UARTRESSIZE,"%sNaN\n",ERR);

    respLen = strlen((const char*)uartResp);
    putInSendBuf(sendBuffer, &sendPointer,
                 (uint8_t*)uartResp, respLen,
                 packetToSend, &sendBufIndex, &pckToSendNum,
                 uartDir);
}

void hvCMD(void){
    uint8_t uartResp[UARTRESSIZE];
    uint8_t respLen = 0;

    memset(uartResp,0,UARTRESSIZE);

    if(argument[0] == '?')
        snprintf((char*)uartResp,UARTRESSIZE,"%s%.2f\n",RESP,biasHVReadVal);
    else
        snprintf((char*)uartResp,UARTRESSIZE,"%sNaN\n",ERR);

    respLen = strlen((const char*)uartResp);
    putInSendBuf(sendBuffer, &sendPointer,
                 (uint8_t*)uartResp, respLen,
                 packetToSend, &sendBufIndex, &pckToSendNum,
                 uartDir);
}

void sendToAddrCMD(void){
    char* pckEnd = strpbrk((const char*)currPacket,uartDirVals);
    putInSendBuf(sendBuffer, &sendPointer,
                 currPacket, (uint8_t*)pckEnd-currPacket,
                 packetToSend, &sendBufIndex, &pckToSendNum,
                 uartDir);
}

void sendToUartCMD(void){
    uint8_t uDir = packetToSend[pckToSendIndex][0];
    uint8_t pckLen = packetToSend[pckToSendIndex][1];
    uint8_t* pck = packetToSend[pckToSendIndex]+2;

    if(uDir == UART1)
        HAL_UART_Transmit_IT(&huart1,pck,pckLen);
    else if(uDir == UART2)
        HAL_UART_Transmit_IT(&huart2,pck,pckLen);

    pckToSendNum--;
}

const commands_t commandExecute[] = {(commands_t)noop,
                                     (commands_t)idnCMD,
                                     (commands_t)voltageCMD,
                                     (commands_t)maxCMD,
                                     (commands_t)sendToAddrCMD,
                                     (commands_t)sendToUartCMD,
                                     (commands_t)hvCMD};


void execute(uint8_t cmd){
    (*(commandExecute[cmd]))();
}
