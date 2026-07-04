/*
    * @file threadpool.h
    *
    *  Created on: 2025-08-27
    *      Author: Josu Alexandru
    * This file defines a thread pool structure and functions to manage a pool of worker threads.
    * 
*/

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdbool.h>
#include "thread.h"
#include "queue.h"
#include "config.h"
#include "typedefs.h"

typedef struct {
    void (*func)(void*);
    void* arg;
} ax_task_t;

typedef struct {
    ax_thread_t* threads;  // Array of worker threads
    u32 thread_count;        // Number of threads in the pool
    queue_t* task_queue;      // Queue to hold tasks
    ax_mutex_t lock;       // Mutex to protect access to the task queue
    ax_cond_t notify;      // Condition variable to signal worker threads
    bool shut_down;          // Flag to indicate if the pool is shutting down
} ax_threadpool_t;

extern s32 ax_threadpool_init();
extern s32 ax_threadpool_submit(void (*function)(void*), void* arg);
extern s32 ax_threadpool_shutdown();


#endif //THREAD_POOL_H