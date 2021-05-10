/*
 * fsm.c
 *
 *  Created on: Apr 30, 2021
 *      Author: mames
 */

#include "fsm.h"

/*
 * initFSM inizializza la macchina a stati partendo dallo stato iniziale, variabili di input e di output
 *
 * Per passare le variabili è necessario creare un array di puntatori void.
 *
 * Esempio:
 *         main(){
 *             uint8_t a,b,c;
 *             uint8_t out1,out2,out3;
 *
 *             fsm_t myFSM;
 *
 *             void* inputs[] = {&a,&b,&c};
 *             void* outputs[] = {&out1,&out2,&out3};
 *
 *             myFSM = initFSM(stateINIT, inputs, outputs);
 *                ...
 *        }
 *
 */

fsm_t initFSM(state_function_t initialState, void** inputs, void** outputs){

    fsm_t retFSM;

    retFSM.state = initialState;
    retFSM.input_var = inputs;
    retFSM.output_var = outputs;

    return retFSM;

}
