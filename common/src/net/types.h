#ifndef NET_TYPES_H
#define NET_TYPES_H

typedef unsigned char NetClientId;
typedef unsigned char NetPriority;
typedef unsigned long NetTime;
typedef unsigned short NetPort;
typedef unsigned char NetByte;
typedef int NetSize;

#define NET_CLIENT_ID_INVALID      ((NetClientId)0b11111110)
#define NET_CLIENT_ID_ALL          ((NetClientId)0b11111111)
#define NET_CLIENT_ID_ALL_BUT_FLAG ((NetClientId)0b10000000)
#define NET_CLIENT_ID_ALL_BUT(except_id)                                       \
    ((NetClientId)(except_id | NET_CLIENT_ID_ALL_BUT_FLAG))

typedef enum NetPacketType
{
    NPT_EMPTY = 0,
    NPT_C_CONNECTION_REQUEST,
    NPT_S_CONNECTION_RESPONSE,
    NPT_C_SYNC,
    NPT_C_DISCONNECT,
    NPT_S_DATA,  /* Data from server to client */
    NPT_C_DATA,  /* Data from client to server */
    NPT_VIRTUAL, /* Events from server to server */
} NetPacketType;

typedef struct NetServerInfo
{
    NetClientId max_clients;
    NetTime recv_timeout_ms;
    NetTime send_timeout_ms;
} NetServerInfo;

typedef struct NetPacketHead
{
    NetPacketType type;
} NetPacketHead;

typedef struct NetSPacketHead
{
    NetPacketHead net_head;
} NetSPacketHead;

typedef struct NetCPacketHead
{
    NetPacketHead net_head;
    NetClientId sender;
} NetCPacketHead;

typedef struct NetSPacket
{
    NetSPacketHead shead;
    NetByte payload[];
} NetSPacket;

typedef struct NetCPacket
{
    NetCPacketHead chead;
    NetByte payload[];
} NetCPacket;

typedef struct NetCPacketConnectionRequest
{
    NetCPacketHead head;
} NetCPacketConnectionRequest;

typedef struct NetSPacketConnectionResponse
{
    NetSPacketHead head;
    NetServerInfo server_info;
    NetClientId assigned_id;
} NetSPacketConnectionResponse;

typedef struct NetCPacketDisconnect
{
    NetCPacketHead head;
} NetCPacketDisconnect;

typedef struct NetCPacketSync
{
    NetCPacketHead head;
} NetCPacketSync;

#endif /* MULTIPLAYER_NET_TYPES_H */
