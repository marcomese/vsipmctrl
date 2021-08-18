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
extern uint8_t* endPacketPointer;

extern uint8_t uartDir;

extern uint8_t sendBuffer[BUFSIZE];
extern uint8_t* packetToSend[PACKETSINBUF];
extern uint8_t* sendPointer;
extern uint8_t pckToSendNum;
extern uint8_t sendBufIndex;
extern uint8_t pckToSendIndex;

extern char argument[CMDARGSIZE];

extern float biasVoltage;
extern float biasMaxVoltage;

extern float katodeVoltage;
extern float katodeMaxVoltage;

extern float biasReadVal;
extern float katodeReadVal;

extern float* outVoltagePointer;
extern float* readVoltagePointer;
extern float* maxVoltagePointer;

extern uint8_t vSection;
extern uint32_t vDACChannels[2];

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
    char idnStr[ADDRLEN+2];
    snprintf(idnStr,ADDRLEN+3,"%s\r\n",ADDR);

    putInSendBuf(sendBuffer, &sendPointer,
                 (uint8_t*)idnStr, ADDRLEN+2,
                 packetToSend, &sendBufIndex, &pckToSendNum,
                 uartDir);
}

void voltageCMD(void){
    char* err;
    float outVal = strtof(argument,&err);
    uint8_t uartResp[UARTRESSIZE];
    uint8_t respLen = 0;

    if(err != argument){
        if(outVal <= *maxVoltagePointer){
            *outVoltagePointer = outVal;

            float voltDACVal = 4095*outVal/2.9;

            HAL_DAC_SetValue(&hdac, vDACChannels[vSection], DAC_ALIGN_12B_R, voltDACVal);

            memset(uartResp,0,UARTRESSIZE);

            snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",outVal);
        }else{
            memset(uartResp,0,UARTRESSIZE);

            snprintf((char*)uartResp,UARTRESSIZE,"E:Vmax=%.2f\r\n",*maxVoltagePointer);
        }
    }else if(argument[0] == '?'){
        memset(uartResp,0,UARTRESSIZE);

        snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",*readVoltagePointer);
    }else{
          snprintf((char*)uartResp,UARTRESSIZE,"E:NaN\r\n");
    }

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

    if(err != argument){
        *maxVoltagePointer = maxVal;

        memset(uartResp,0,UARTRESSIZE);

        snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",maxVal);
    }else if(argument[0] == '?'){
        memset(uartResp,0,UARTRESSIZE);

        snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",*maxVoltagePointer);
    }else{
        snprintf((char*)uartResp,UARTRESSIZE,"E:NaN\r\n");
    }

    respLen = strlen((const char*)uartResp);
    putInSendBuf(sendBuffer, &sendPointer,
                 (uint8_t*)uartResp, respLen,
                 packetToSend, &sendBufIndex, &pckToSendNum,
                 uartDir);

}

void sendAddrCMD(void){//DA SOSTITUIRE, QUESTA FUNZIONE DOVRA' AGGIUNGERE IL PACCHETTO NEL BUFFER
//    HAL_UART_Transmit_IT(&huart2,currPacket,endPacketPointer-currPacket); //UNA MACCHINA A STATI PENSERA' AD INVIARLO
}

void sendToUartCMD(void){
    uint8_t uDir = packetToSend[pckToSendIndex][0];
    uint8_t pckLen = packetToSend[pckToSendIndex][1];
    uint8_t* pck = packetToSend[pckToSendIndex]+2;

    if(uDir == UART1){
        HAL_UART_Transmit_IT(&huart1,pck,pckLen);
    }else if(uDir == UART2){
        HAL_UART_Transmit_IT(&huart2,pck,pckLen);
    }

    pckToSendNum--;
}

const commands_t commandExecute[] = {(commands_t)noop,
                                     (commands_t)idnCMD,
                                     (commands_t)voltageCMD,
                                     (commands_t)maxCMD,
                                     (commands_t)sendAddrCMD,
                                     (commands_t)sendToUartCMD};


void execute(uint8_t cmd){
    (*(commandExecute[cmd]))();
}
