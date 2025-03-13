/*
 * process.c
 *
 *  Created on: Mar 6, 2025
 *      Author: timothyli, rawsenweideman
 *    
 */


#include "3140_concur.h"
#include <stdlib.h>
#include <MKL46Z4.h>
#include "process.h"


int process_create(void (*f)(void), int n) {
	process_t *temp = (process_t *)malloc(sizeof(process_t)); // create temp process that will take on input parameter values and be enqueued
	if(temp == NULL){ // case where things fail
		return -1;
	}
	__disable_irq(); // process_stack_init only works when disable interrupts, this line is meaningless when interrupts are not set up
	temp -> sp = process_stack_init(f, n); // initialize process stack, set our temp stack pointer as this initial value
	__enable_irq();
	temp -> n = n; // save n for later when need to free process_stack
	if(temp -> sp == NULL) { // other case when things fail
		free(temp);
		return -1;
	}

	temp -> next = NULL;
	enqueue(temp, &process_queue); // put this process onto the queue
	return 0;
}

void process_start (void) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT->MCR = 0x00;
    PIT->CHANNEL[0].LDVAL = 150000; // 100 Hz

    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; // enable interrupts
    NVIC_EnableIRQ(PIT_IRQn); // enable PIT interrupt

    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; //start timer

    process_begin();
}

unsigned int * process_select(unsigned int * cursp) {
	if(current_process_p != NULL) {
		if(cursp == NULL) { // means current process is done
			process_stack_free(current_process_p -> sp, current_process_p -> n);
			free(current_process_p);
		} else {
			current_process_p -> sp = cursp; // update stack pointer with cursp
			enqueue(current_process_p, &process_queue); // put current process at end of queue
		}
	}

	if(is_empty(&process_queue)) {
	    if(cursp != NULL) { // If the current process is still running, keep running it 
		    //(this catches cases when there is only one process remaining, so our previous enqueue does nothing)
	        return current_process_p->sp;
	    } else {
	        return NULL;  // Only exit if there's no process running at all.
	    }
	}
	current_process_p = dequeue(&process_queue); // get next process to run
	return current_process_p -> sp; //return the stack pointer of this next process
}
