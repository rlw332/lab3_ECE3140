/*
 * process.h
 *
 * Nils Napp
 * Cornell University
 * All right reserved
 *
 * Jan 2024
 * Ithaca NY
 *
 * This file is part of the ECE3140/CS3420 offering for Spring 2024. If you are not part
 * of this class you should not have access to this file. Do not post, share, or otherwise
 * distribute this file. We will consider it an AI violation if you do.
 */

#ifndef PROCESS_H_
#define PROCESS_H_

struct process_state{
	//
	unsigned int *sp;
	unsigned int * start;
	int n;
	//
	struct process_state * next; // keep this definition for en/de-queue
};

typedef struct process_state process_t;


#endif /* PROCESS_H_ */
