#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t * q) {
        if (q == NULL) return 1;
	return (q->size == 0);
}

void enqueue(struct queue_t * q, struct pcb_t * proc) {
        /* TODO: put a new process to queue [q] */
        if(q->size < MAX_QUEUE_SIZE){
                q->proc[q->size] = proc;
                q->size++;
        }else{
                return;
        }      
}

struct pcb_t * dequeue(struct queue_t * q) {
        /* TODO: return a pcb whose priority is the highest
         * in the queue [q] and remember to remove it from q
         */

        if (q->size == 0) {
            return NULL;
        }
    
        int index = 0;
    
        #ifdef MLQ_SCHED

        struct pcb_t *retval = q->proc[0];
        index = 0;
    
        #else

        uint32_t best_prio = q->proc[0]->priority;
        for (int i = 1; i < q->size; i++) {
            if (q->proc[i]->priority < best_prio) {
                best_prio = q->proc[i]->priority;
                index = i;
            }
        }
        struct pcb_t *retval = q->proc[index];

        #endif
    
        for (int i = index; i < q->size - 1; i++) {
            q->proc[i] = q->proc[i + 1];
        }
    
        q->size--;
        return retval;
        
    }

    