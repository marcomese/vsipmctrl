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

extern uint8_t* endPacketPointer;
extern uint8_t uartResp[UARTRESSIZE];
extern char argument[CMDARGSIZE];
extern uint8_t command;

extern float biasVoltage;
extern float biasMaxVoltage;

extern float katodeVoltage;
extern float katodeMaxVoltage;

extern float biasReadVal;
extern float katodeReadVal;

extern float* outVoltagePointer;

extern uint8_t vSection;
extern uint32_t vDACChannels[2];

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
        *outVoltagePointer = outVal;

        float voltDACVal = 4096*outVal/2.9;

        HAL_DAC_SetValue(&hdac, vDACChannels[vSection], DAC_ALIGN_12B_R, voltDACVal);

        memset(uartResp,0,UARTRESSIZE);

        snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",outVal);

        HAL_UART_Transmit_IT(&huart1,uartResp,UARTRESSIZE);
    }else{
        HAL_UART_Transmit_IT(&huart1, (uint8_t*)"ERR: NaN     \r\n", UARTRESSIZE);
    }
}

const commands_t commandExecute[] = {(commands_t)noop,
                                     (commands_t)idnCMD,
                                     (commands_t)voltageCMD};


void execute(void){
    if(command){
        *(endPacketPointer-1) = 0; // Azzero il carattere terminatore perchè altrimenti si ripete la lettura del pacchetto
        (*(commandExecute[command]))();
    }
}
