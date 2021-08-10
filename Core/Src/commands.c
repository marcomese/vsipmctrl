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

extern uint8_t uartResp[UARTRESSIZE];
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

extern float vref;

extern uint16_t* vrefCal;

void noop(void){}

void idnCMD(void){
    snprintf((char*)uartResp,UARTRESSIZE,"%s\r\n",ADDR);
    HAL_UART_Transmit_IT(&huart1,uartResp,UARTRESSIZE);
}

/*
 * REALIZZARE UN BUFFER PER IL TX!!!!
 */

void voltageCMD(void){
    char* err;
    float outVal = strtof(argument,&err);

    if(err != argument){
        if(outVal <= *maxVoltagePointer){
            *outVoltagePointer = outVal;

            float voltDACVal = 4095*outVal/2.9;

            HAL_DAC_SetValue(&hdac, vDACChannels[vSection], DAC_ALIGN_12B_R, voltDACVal);

            memset(uartResp,0,UARTRESSIZE);

            snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",outVal);

            HAL_UART_Transmit_IT(&huart1,uartResp,UARTRESSIZE);
        }else{
            memset(uartResp,0,UARTRESSIZE);

            snprintf((char*)uartResp,UARTRESSIZE,"E:Vmax=%.2f\r\n",*maxVoltagePointer);

            HAL_UART_Transmit_IT(&huart1, uartResp, UARTRESSIZE);
        }
    }else if(argument[0] == '?'){
        memset(uartResp,0,UARTRESSIZE);

        snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",*readVoltagePointer);

        HAL_UART_Transmit_IT(&huart1,uartResp,UARTRESSIZE);
    }else{
        HAL_UART_Transmit_IT(&huart1, (uint8_t*)"E:NaN        \r\n", UARTRESSIZE);
    }
}

void maxCMD(void){
    char* err;
    float maxVal = strtof(argument,&err);

    if(err != argument){
        *maxVoltagePointer = maxVal;

        memset(uartResp,0,UARTRESSIZE);

        snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",maxVal);

        HAL_UART_Transmit_IT(&huart1,uartResp,UARTRESSIZE);

    }else if(argument[0] == '?'){
        memset(uartResp,0,UARTRESSIZE);

        snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",*maxVoltagePointer);

        HAL_UART_Transmit_IT(&huart1,uartResp,UARTRESSIZE);
    }else{
        HAL_UART_Transmit_IT(&huart1, (uint8_t*)"ERR: NaN     \r\n", UARTRESSIZE);
    }
}

/*
 * AGGIUNGERE BUFFER TX!!!!
 */

void sendAddrCMD(void){//DA SOSTITUIRE, QUESTA FUNZIONE DOVRA' AGGIUNGERE IL PACCHETTO NEL BUFFER
    HAL_UART_Transmit_IT(&huart2,currPacket,endPacketPointer-currPacket); //UNA MACCHINA A STATI PENSERA' AD INVIARLO
}

const commands_t commandExecute[] = {(commands_t)noop,
                                     (commands_t)idnCMD,
                                     (commands_t)voltageCMD,
                                     (commands_t)maxCMD,
                                     (commands_t)sendAddrCMD};


void execute(uint8_t cmd){
    if(cmd){
        //*(endPacketPointer-1) = 0; // Azzero il carattere terminatore perchè altrimenti si ripete la lettura del pacchetto
        (*(commandExecute[cmd]))();
        *(endPacketPointer-1) = 0; // Azzero il carattere terminatore perchè altrimenti si ripete la lettura del pacchetto
    }
}
