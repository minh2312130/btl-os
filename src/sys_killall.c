/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */


#include "common.h"
#include "syscall.h"
#include "stdio.h"
#include "libmem.h"
#include "queue.h"
#include <string.h>

int __sys_killall(struct pcb_t *caller, struct sc_regs *regs)
{
    char proc_name[100];
    uint32_t data;

    // hardcode for demo only
    uint32_t memrg = regs->a1;

    /* TODO: Get name of the target proc */
    // proc_name = libread..
    int i = 0;
    data = 0;
    while (data != (uint32_t)(-1))
    {
        libread(caller, memrg, i, &data);
        proc_name[i] = (char)data;
        if (data == -1)
            proc_name[i] = '\0';
        i++;
    }
    printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);

    /* Implementation of process termination by name */
    int terminated_count = 0;
    
    
    if (!caller->running_list) {
        printf("Error: Running process list not available\n");
        return -1;
    }
    
    struct queue_t *active_processes = caller->running_list;
    if (active_processes->size == 0) {
        printf("No active processes found in running list\n");
        return -1;
    }
    
    
    for (i = 0; i < active_processes->size; i++) {
        struct pcb_t *current = active_processes->proc[i];
        
        
        if (!current) continue;
        
       
        if (strcmp(current->path, proc_name) == 0) {
  
            #ifdef MM_PAGING
            if (current->mm) {
                free_pcb_memph(current);
            }
            #endif
            
            terminated_count++;
            current = NULL;
            
            
            for (int j = i; j < active_processes->size - 1; j++) {
                active_processes->proc[j] = active_processes->proc[j + 1];
            }
            active_processes->size--;
            i--; 
        }
    }
    
    #ifdef MLQ_SCHED
   
    if (!caller->mlq_ready_queue) {
        printf("MLQ scheduler enabled but queue not initialized\n");
        return terminated_count;
    }
    
    
    for (int priority = 0; priority < MAX_PRIO; priority++) {
        struct queue_t *priority_queue = &caller->mlq_ready_queue[priority];
        
       
        if (!priority_queue) continue;
        
       
        for (i = 0; i < priority_queue->size; i++) {
            struct pcb_t *current = priority_queue->proc[i];
            
            
            if (!current) continue;
             
            
            if (strcmp(current->path, proc_name) == 0) {
               
                #ifdef MM_PAGING
                if (current->mm) {
                    free_pcb_memph(current);
                }
                #endif
                
                current = NULL;
                terminated_count++;
                
                
                for (int j = i; j < priority_queue->size - 1; j++) {
                    priority_queue->proc[j] = priority_queue->proc[j + 1];
                }
                priority_queue->size--;
                i--; 
            }
        }
    }
    #else
    
    if (caller->ready_queue) {
        struct queue_t *ready = caller->ready_queue;
        
        for (i = 0; i < ready->size; i++) {
            struct pcb_t *current = ready->proc[i];
            
            
            if (!current) continue;
            
            
            if (strcmp(current->path, proc_name) == 0) {
               
                #ifdef MM_PAGING
                if (current->mm) {
                    free_pcb_memph(current);
                }
                #endif
                
                current = NULL;
                terminated_count++;
                
              
                for (int j = i; j < ready->size - 1; j++) {
                    ready->proc[j] = ready->proc[j + 1];
                }
                ready->size--;
                i--; 
            }
        }
    }
    #endif
    
    printf("Terminated %d process(es) with name \"%s\"\n", terminated_count, proc_name);
    return terminated_count;
}
