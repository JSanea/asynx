#ifndef COMMON_H
#define COMMON_H

#include "typedefs.h"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>

typedef s32 SOCKET;
#endif

// #include "typedefs.h"

extern s32 ax_set_nonblocking(SOCKET sock); // Function to set a socket to non-blocking mode

#endif // COMMON_H