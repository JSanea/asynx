/*
    * event.h
    *
    *  Created on: 2025-09-02
    *      Author: Josu Alexandru
    *  Event structure definition for cross-platform event handling.
    * 
*/

#ifndef EVENT_H
#define EVENT_H

#if defined(__linux__)
#include <sys/epoll.h>
#include <unistd.h>
#elif defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#include "typedefs.h"
#include "buffer.h"

typedef void (*callback_t)(void*);

enum {
    EVENT_TYPE_TCP   = 1,
    EVENT_TYPE_UDP   = 2,
    EVENT_TYPE_FILE  = 4,
    EVENT_TYPE_READ  = 8,
    EVENT_TYPE_WRITE = 16
}ax_event_type_e;


typedef struct event_ctx_s{
    ax_event_type_e type;  // event type
    callback_t cb;           // callback function
    void* arg;               // argument for callback function
    ax_buff_span_t buff;   // associated buffer
}ax_event_ctx_t;


typedef struct event_s {
    ax_event_ctx_t* ctx;

    union {
        #if defined(__linux__)
            struct epoll_event epoll_event; // Linux epoll
        #elif defined(__APPLE__) || defined(__FreeBSD__)
            struct kevent kevent_event;     // BSD kqueue
        #elif defined(_WIN32)
            struct {
                HANDLE handle;             // Windows handle
                OVERLAPPED overlapped;     // Overlapped I/O structure
            } win;
        #endif
    } ev_handle;
} ax_event_t;


static inline void ax_free_event(ax_event_t* ev){
    if(ev == NULL) return;

    if(ev->ctx){
        if (ev->ctx->buff.data){
            free(ev->ctx->buff.data);
            ev->ctx->buff.data = NULL;
        }
        free(ev->ctx);
        ev->ctx = NULL;
    };
    
    free(ev);
    ev = NULL;
}


#endif //EVENT_H