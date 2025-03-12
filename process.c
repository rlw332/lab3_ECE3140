/*
 * process.c
 *
 *  Created on: Mar 6, 2025
 *      Author: timothyli
 *      Reviser and Tester: rlw332
 */


#include "3140_concur.h"
#include <stdlib.h>
#include <MKL46Z4.h>
#include "process.h"


int process_create(void (*f)(void), int n) {
	process_t *temp = (process_t *)malloc(sizeof(process_t));
	if(temp == NULL){
		return -1;
	}
	temp -> sp = process_stack_init(f, n);
	if(temp -> sp == NULL) {
		free(temp);
		return -1;
	}

	temp -> next = NULL;
	enqueue(temp, &process_queue);
	return 0;
}

void process_start (void) {
    set15MHz();

    //same as part 3
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT->MCR = 0x00;
    PIT->CHANNEL[0].LDVAL = 150000;

    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; // enable interrupts
    NVIC_EnableIRQ(PIT_IRQn); // enable PIT interrupt

    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; //start timer

    process_begin();
}

unsigned int * process_select(unsigned int * cursp) {
	if(current_process_p != NULL) {
		if(cursp == NULL) {
			free(current_process_p);
		} else {
			current_process_p -> sp = cursp;
			enqueue(current_process_p, &process_queue);
		}
	}

	if(is_empty(&process_queue)) {
		return NULL;
	}
	current_process_p = dequeue(&process_queue);
	return current_process_p -> sp;
}
