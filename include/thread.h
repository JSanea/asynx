/*
    * @file thread.h
    *
    *  Created on: 2025-08-27
    *      Author: Josu Alexandru
    *  This file provides a cross-platform abstraction for threading primitives such as mutexes,
    *  condition variables, and reader-writer locks. It supports both Windows and POSIX systems.
    *  The implementation uses Windows API on Windows and pthreads on POSIX systems.
    *  It includes functions to initialize, lock, unlock, and destroy these primitives.
    * 
*/

#ifndef THREAD_H
#define THREAD_H

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "typedefs.h"

// Thread structure
typedef struct {
    #if defined(_WIN32)
        HANDLE handle; // Windows thread handle
        DWORD thread_id; // Windows thread ID
    #else
        pthread_t thread; // POSIX thread
    #endif
}ax_thread_t;

// Mutex structure
typedef struct{
    #if defined(_WIN32)
        CRITICAL_SECTION cs; // Windows critical section
    #else
        pthread_mutex_t mutex; // POSIX mutex
    #endif
}ax_mutex_t;

// Condition variable structure
typedef struct{
    #if defined(_WIN32)
        CONDITION_VARIABLE cond; // Windows condition variable          
    #else
        pthread_cond_t cond;   // POSIX condition variable
    #endif
}ax_cond_t;

// Reader-writer lock structure
typedef struct {
    #if defined(_WIN32)
        SRWLOCK rw_lock; // Windows SRWLOCK
    #else
        pthread_rwlock_t rw_lock; // POSIX pthread_rwlock_t
    #endif
} ax_rwlock_t;

/*
* Thread functions
*/
extern s32 ax_thread_create(ax_thread_t* thread, void* (*start_routine)(void*), void* arg);
extern s32 ax_thread_join(ax_thread_t* thread);
extern s32 ax_thread_detach(ax_thread_t* thread);
extern void ax_thread_exit(void* retval);     

/*
* Mutex functions
*/
extern s32 ax_mutex_init(ax_mutex_t* mutex);
extern s32 ax_mutex_lock(ax_mutex_t* mutex);        
extern s32 ax_mutex_unlock(ax_mutex_t* mutex);
extern s32 ax_mutex_destroy(ax_mutex_t* mutex);

/*
* Condition variable functions
*/
extern s32 ax_cond_init(ax_cond_t* cond);
extern s32 ax_cond_wait(ax_cond_t* cond, ax_mutex_t* mutex);
extern s32 ax_cond_signal(ax_cond_t* cond);     
extern s32 ax_cond_broadcast(ax_cond_t* cond);
extern s32 ax_cond_destroy(ax_cond_t* cond);

/*
* Reader-writer lock functions
*/      
extern s32 ax_rwlock_init(ax_rwlock_t* lock);
extern s32 ax_rwlock_rdlock(ax_rwlock_t* lock);
extern s32 ax_rwlock_wrlock(ax_rwlock_t* lock);
extern s32 ax_rwlock_unlock(ax_rwlock_t* lock);
extern s32 ax_rwlock_destroy(ax_rwlock_t* lock);


#endif //THREAD_h
