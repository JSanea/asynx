/*
    * @file thread.c
    * 
    *  Created on: 2025-08-28
    *      Author: Josu Alexandru
    *
*/

#include "thread.h"


s32 ax_thread_create(ax_thread_t* t, void* (*start_routine)(void*), void* arg){
    if(!t || !start_routine) return -1;
    #if defined(_WIN32)
        t->handle = CreateThread(
            NULL,              
            0,                  
            (LPTHREAD_START_ROUTINE)start_routine, 
            arg,               
            0,                  
            &t->thread_id); 

        if (t->handle == NULL) {
            perror("Failed to create thread");
            return -1;
        }
    #else
        if(pthread_create(&t->thread, NULL, start_routine, arg) != 0){
            perror("Failed to create thread");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_thread_join(ax_thread_t* t){
    if(!t) return -1;
    #if defined(_WIN32)
        WaitForSingleObject(t->handle, INFINITE);
        CloseHandle(t->handle);
        t->handle = NULL;
    #else
        if(pthread_join(t->thread, NULL) != 0){
            perror("Failed to join thread");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_thread_detach(ax_thread_t* t){
    if(!t) return -1;
    #if defined(_WIN32)
        CloseHandle(t->handle);
        t->handle = NULL;
    #else
        if(pthread_detach(t->thread) != 0){
            perror("Failed to detach thread");
            return -1;
        }
    #endif
    return 0;
}

void ax_thread_exit(void* retval){
    #if defined(_WIN32)
        ExitThread((DWORD)(uintptr_t)retval);
    #else
        pthread_exit(retval);
    #endif
}

s32 ax_mutex_init(ax_mutex_t* m){
    if(!m) return -1;
    #if defined(_WIN32)
        InitializeCriticalSection(&m->cs);
    #else
        if(pthread_mutex_init(&m->mutex, NULL) != 0){
            perror("Failed to initialize mutex");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_mutex_lock(ax_mutex_t* m){
    if(!m) return -1;
    #if defined(_WIN32)
        EnterCriticalSection(&m->cs);
    #else
        if(pthread_mutex_lock(&m->mutex) != 0){
            perror("Failed to lock mutex");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_mutex_unlock(ax_mutex_t* m){
    if(!m) return -1;
    #if defined(_WIN32)
        LeaveCriticalSection(&m->cs);
    #else
        if(pthread_mutex_unlock(&m->mutex) != 0){
            perror("Failed to unlock mutex");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_mutex_destroy(ax_mutex_t* m){
    if(!m) return -1;
    #if defined(_WIN32)
        DeleteCriticalSection(&m->cs);
    #else
        if(pthread_mutex_destroy(&m->mutex) != 0){
            perror("Failed to destroy mutex");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_cond_init(ax_cond_t* c){
    if(!c) return -1;
    #if defined(_WIN32)
        InitializeConditionVariable(&c->cond);
        c->waiters_count = 0;
    #else
        if(pthread_cond_init(&c->cond, NULL) != 0){
            perror("Failed to initialize condition variable");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_cond_wait(ax_cond_t* c, ax_mutex_t* m){
    if(!c || !m) return -1;
    #if defined(_WIN32)
        SleepConditionVariableCS(&c->cond, &m->cs, INFINITE);
    #else
        if(pthread_cond_wait(&c->cond, &m->mutex) != 0){
            perror("Failed to wait on condition variable");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_cond_signal(ax_cond_t* c){
    if(!c) return -1;
    #if defined(_WIN32)
        WakeConditionVariable(&c->cond);
    #else
        if(pthread_cond_signal(&c->cond) != 0){
            perror("Failed to signal condition variable");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_cond_broadcast(ax_cond_t* c){
    if(!c) return -1;
    #if defined(_WIN32)
        WakeAllConditionVariable(&c->cond);
    #else
        if(pthread_cond_broadcast(&c->cond) != 0){
            perror("Failed to broadcast condition variable");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_cond_destroy(ax_cond_t* c){
    if(!c) return -1;
    #if !defined(_WIN32)
        if(pthread_cond_destroy(&c->cond) != 0){
            perror("Failed to destroy condition variable");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_rwlock_init(ax_rwlock_t* rwlock){
    if(!rwlock) return -1;
    #if defined(_WIN32)
        InitializeSRWLock(&rwlock->rw_lock);
    #else
        if(pthread_rwlock_init(&rwlock->rw_lock, NULL) != 0){
            perror("Failed to initialize reader-writer lock");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_rwlock_rdlock(ax_rwlock_t* rwlock){
    if(!rwlock) return -1;
    #if defined(_WIN32)
        AcquireSRWLockShared(&rwlock->rw_lock);
    #else
        if(pthread_rwlock_rdlock(&rwlock->rw_lock) != 0){
            perror("Failed to acquire read lock");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_rwlock_wrlock(ax_rwlock_t* rwlock){
    if(!rwlock) return -1;
    #if defined(_WIN32)
        AcquireSRWLockExclusive(&rwlock->rw_lock);
    #else
        if(pthread_rwlock_wrlock(&rwlock->rw_lock) != 0){
            perror("Failed to acquire write lock");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_rwlock_unlock(ax_rwlock_t* rwlock){
    if(!rwlock) return -1;
    #if defined(_WIN32)
        ReleaseSRWLockShared(&rwlock->rw_lock);
    #else
        if(pthread_rwlock_unlock(&rwlock->rw_lock) != 0){
            perror("Failed to release reader-writer lock");
            return -1;
        }
    #endif
    return 0;
}

s32 ax_rwlock_destroy(ax_rwlock_t* rwlock){
    if(!rwlock) return -1;
    #if !defined(_WIN32)
        if(pthread_rwlock_destroy(&rwlock->rw_lock) != 0){
            perror("Failed to destroy rwlock");
            return -1;
        }
    #endif
    return 0;
}   

