#ifndef BUFFER_H
#define BUFFER_H

#include "typedefs.h"

typedef struct buffer_s {
    void* data;       // pointer to buffer data
    u32 size;       // size of the buffer
    u32 offset;     // current offset in the buffer
} ax_buff_span_t;

#endif //BUFFER_H