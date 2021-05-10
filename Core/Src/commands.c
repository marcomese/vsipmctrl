/*
 * commands.c
 *
 *  Created on: May 2, 2021
 *      Author: mames
 */

#include "commands.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint8_t* endPacketPointer;
extern uint8_t uartResp[UARTRESSIZE];
extern char argument[CMDARGSIZE];
extern uint8_t command;

extern float biasVoltage;
extern float biasMaxVoltage;

extern float katodeVoltage;
extern float katodeMaxVoltage;

extern float* outVoltagePointer;

void execute(void){
    if(command){
        *(endPacketPointer-1) = 0; // Azzero il carattere terminatore perchè altrimenti si ripete la lettura del pacchetto
        (*(commandExecute[command]))();
    }
}

void noop(void){}

void idnCMD(void){
    snprintf((char*)uartResp,UARTRESSIZE,"%s\r\n",ADDR);
    HAL_UART_Transmit_IT(&huart2,uartResp,UARTRESSIZE);
}

/*
 * REALIZZARE UN BUFFER PER IL TX!!!!
 */

void voltageCMD(void){
    char* err;
    float outVal = strtof(argument,&err);

    if(err != argument){
        *outVoltagePointer = outVal;

        memset(uartResp,0,UARTRESSIZE);

        snprintf((char*)uartResp,UARTRESSIZE,"%.2f\r\n",outVal);

        HAL_UART_Transmit_IT(&huart2,uartResp,UARTRESSIZE);
    }else{
        HAL_UART_Transmit_IT(&huart2, (uint8_t*)"ERR: NaN\r\n", UARTRESSIZE);
    }
}

const commands_t commandExecute[] = {(commands_t)noop,
                                     (commands_t)idnCMD,
                                     (commands_t)voltageCMD};
