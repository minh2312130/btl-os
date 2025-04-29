#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t * q) {
        if (q == NULL) return 1;
	return (q->size == 0);
}

void enqueue(struct queue_t * q, struct pcb_t * proc) {
        /* TODO: put a new process to queue [q] */
    if (q == NULL) {
        fprintf(stderr, "Error: Queue does not exist.\n");
        return;
    }

    if (proc == NULL) {
            fprintf(stderr, "Error: Process does not exist.\n");
            return;
        }
    
    if (q->size < MAX_QUEUE_SIZE){
        q->proc[q->size] = proc ;
        q->size=q->size + 1;
    }
}

struct pcb_t * dequeue(struct queue_t * q) {
        /* TODO: return a pcb whose prioprity is the highest*/

        if (q->size == 0) return NULL; // Handle empty queue case

        int max_idx = 0; // Assume first element has the highest priority
        for (int i = 1; i < q->size; i++) {
            if (q->proc[i]->prio < q->proc[max_idx]->prio) {
                max_idx = i; // Update max_idx for higher priority process
            }
        }
    
        struct pcb_t *highest_priority_proc = q->proc[max_idx]; // Save process pointer
    
        // Shift elements to fill the gap left by the removed process
        for (int i = max_idx; i < q->size - 1; i++) {
            q->proc[i] = q->proc[i + 1];
        }
    
        q->size=q->size - 1; // Decrease size to reflect removal
    
        return highest_priority_proc; // Return the removed process 
        //highest_priority_proc -> minimum 
}

