/*
    * @file threadpool.c
    *
    *  Created on: 2025-08-28
    *      Author: Josu Alexandru
*/

#include "asynx.h"
#include "threadpool.h"

static ax_threadpool_t pool;


static void* ax_worker(void* arg){
    (void)arg; 

    while(true){
        ax_mutex_lock(&pool.lock);

        while(is_empty(pool.task_queue) && !pool.shut_down){
            ax_cond_wait(&pool.notify, &pool.lock);
        }

        if(is_empty(pool.task_queue) && pool.shut_down){
            ax_mutex_unlock(&pool.lock);
            break;
        }

        ax_task_t* task = (ax_task_t*)dequeue(pool.task_queue);

        ax_mutex_unlock(&pool.lock);

        if(task){
            task->func(task->arg);
            free(task);
            task = NULL;
        }
    }

    return NULL;
}

s32 ax_threadpool_init(){
    pool.thread_count = AX_MAX_THREADS;
    pool.shut_down = false;

    pool.task_queue = (queue_t*)malloc(sizeof(queue_t));
    queue_init(pool.task_queue);

    if(ax_mutex_init(&pool.lock) != 0) exit(EXIT_FAILURE);

    if(ax_cond_init(&pool.notify) != 0) exit(EXIT_FAILURE);

    pool.threads = (ax_thread_t*)malloc(sizeof(ax_thread_t) * pool.thread_count);

    for(u32 i = 0; i < pool.thread_count; i++){
        if(ax_thread_create(&pool.threads[i], ax_worker, NULL) != 0){
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

s32 ax_threadpool_submit(void (*func)(void*), void* arg){
    if(func == NULL) return -1;

    ax_task_t* task = (ax_task_t*)malloc(sizeof(ax_task_t));
    if(task == NULL) return -1;

    task->func = func;
    task->arg = arg;

    ax_mutex_lock(&pool.lock);

    enqueue(pool.task_queue, task);

    ax_cond_signal(&pool.notify);
    ax_mutex_unlock(&pool.lock);

    return 0;
}

s32 ax_threadpool_shutdown(){
    ax_mutex_lock(&pool.lock);
    pool.shut_down = true;
    ax_cond_broadcast(&pool.notify);
    ax_mutex_unlock(&pool.lock);

    for(u32 i = 0; i < pool.thread_count; i++){
        ax_thread_join(&pool.threads[i]);
    }

    free(pool.threads);
    queue_destroy(pool.task_queue);
    free(pool.task_queue);

    pool.threads = NULL;
    pool.task_queue = NULL;

    ax_mutex_destroy(&pool.lock);
    ax_cond_destroy(&pool.notify);

    return 0;
}



