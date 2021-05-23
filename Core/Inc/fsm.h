/*
 * fsm.h
 *
 *  Created on: Apr 30, 2021
 *      Author: mames
 */

#ifndef FSM_H_
#define FSM_H_

#include <stdint.h>

struct fsm;
typedef void (*state_function_t)(struct fsm*);
typedef struct fsm {
    state_function_t    state;
    void**              input_var;
    void**              output_var;
} fsm_t;

/*
 * Macro utili:
 *     FSM_OUT seleziona una delle variabili di uscita della FSM
 *     FSM_IN seleziona una delle variabili di ingresso della FSM
 *     FSM_STATE seleziona il puntatore a funzione che descrive lo stato della FSM
 */
#define FSM_OUT(FSM_T,VARNAME,TYPE) *(TYPE*)FSM_T->output_var[VARNAME]
#define FSM_IN(FSM_T,VARNAME,TYPE) *(TYPE*)FSM_T->input_var[VARNAME]
#define FSM_STATE(FSM_T) ((FSM_T)->state)

#define FSM_INP(FSM_T,VARNAME) FSM_T->input_var[VARNAME]
#define FSM_OUTP(FSM_T,VARNAME) FSM_T->output_var[VARNAME]

/*
 * Macro per far girare la FSM
 */
#define runFSM(FSM_T) ((FSM_T)->state(FSM_T))

fsm_t initFSM(state_function_t initialState, void** inputs, void** outputs);

#endif /* FSM_H_ */
