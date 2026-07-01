
#include "common.h"

s32  ax_set_nonblocking(SOCKET sock) {
    #ifdef _WIN32
        u_long mode = 1;
        return ioctlsocket(sock, FIONBIO, &mode);
    #else
        int flags = fcntl(sock, F_GETFL, 0);
        if (flags == -1) return -1;
        return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    #endif
}


