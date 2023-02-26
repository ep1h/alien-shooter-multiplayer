/**
 * @file socket.c
 * @brief Implementation of socket component.
 * 
*/
#include "socket.h"
#include <winsock2.h>
#include "utils/mem/mem.h"

static WSADATA ws_ = {0};

/**
 * @brief Socket object. Required to interact using this socket API.
*/
typedef struct NetSocket
{
    SOCKET socket;
} NetSocket;

bool net_socket_init(void)
{
    /* Init WS */
    if (WSAStartup(MAKEWORD(2, 2), &ws_) == SOCKET_ERROR)
    {
        // printf("WSAStartup() failed: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

void net_socket_destroy(void)
{
    if (WSACleanup() == SOCKET_ERROR)
    {
        // printf("WSACleanup() failed: %d\n", WSAGetLastError());
        return;
    }
    memset(&ws_, 0, sizeof(ws_));
}

NetSocket* net_socket_create_socket(void)
{
    NetSocket* ns = mem_alloc(sizeof(NetSocket));
    if (!ns)
    {
        // printf("mem_alloc() failed\n");
        return 0;
    }
    ns->socket = socket(AF_INET, SOCK_DGRAM, 0 /*IPPROTO_UDP*/);
    if (ns->socket == INVALID_SOCKET)
    {
        // printf("socket() failed: %d\n", WSAGetLastError());
        mem_free(ns);
        return 0;
    }
    return ns;
}

void net_socket_destroy_socket(NetSocket* ns)
{
    if (!ns)
    {
        return;
    }
    if (ns->socket)
    {
        closesocket(ns->socket);
    }
    mem_free(ns);
}

bool net_socket_bind_socket(NetSocket* ns, const char ip[], unsigned short port)
{
    if (!ns)
    {
        return false;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.S_un.S_addr = ip ? inet_addr(ip) : INADDR_ANY;
    if (bind(ns->socket, (struct sockaddr*)&address, sizeof(address)) ==
        SOCKET_ERROR)
    {
        // printf("bind() failed: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

int net_socket_sendto(NetSocket* ns, const struct sockaddr_in* dst,
                      const void* buf, int size)
{
    int result = sendto(ns->socket, buf, size, 0, (const struct sockaddr*)dst,
                        sizeof(struct sockaddr));
    if (result == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        if (error == WSAETIMEDOUT)
        {
            return NET_SOCKET_ERROR_TIMEDOUT;
        }
        // printf("sendto() failed: %d\n", error);
        return -1;
    }
    return result;
}

int net_socket_recvfrom(NetSocket* ns, struct sockaddr_in* src, void* buf)
{
    int fromlen = sizeof(struct sockaddr_in);
    int result = recvfrom(ns->socket, buf, NET_SOCKET_MAX_MSG_SIZE, 0,
                          (struct sockaddr*)src, &fromlen);
    if (result == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        if (error == WSAETIMEDOUT)
        {
            return NET_SOCKET_ERROR_TIMEDOUT;
        }
        // printf("recvfrom() failed: %d\n", error);
        return -1;
    }
    return result;
}

bool net_socket_set_sendto_timeout(NetSocket* ns, int timeout_ms)
{
    if (!ns)
    {
        return false;
    }
    if (setsockopt(ns->socket, SOL_SOCKET, SO_SNDTIMEO,
                   (const char*)&timeout_ms,
                   sizeof(timeout_ms)) == SOCKET_ERROR)
    {
        // printf("setsockopt() failed: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

bool net_socket_set_recvfrom_timeout(NetSocket* ns, int timeout_ms)
{
    if (!ns)
    {
        return false;
    }
    if (setsockopt(ns->socket, SOL_SOCKET, SO_RCVTIMEO,
                   (const char*)&timeout_ms,
                   sizeof(timeout_ms)) == SOCKET_ERROR)
    {
        // printf("setsockopt() failed: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

bool net_socket_set_nonblocking(NetSocket* ns, bool status)
{
    if (!ns)
    {
        return false;
    }
    u_long mode = (u_long)status;
    if (ioctlsocket(ns->socket, FIONBIO, &mode))
    {
        // printf("ioctlsocket failed: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}
