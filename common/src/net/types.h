#ifndef NET_TYPES_H
#define NET_TYPES_H

typedef unsigned char NetClientId;
typedef unsigned char NetPriority;
typedef unsigned long NetTime;
typedef unsigned short NetPort;

#define NET_CLIENT_ID_INVALID      ((NetClientId)0b11111110)
#define NET_CLIENT_ID_ALL          ((NetClientId)0b11111111)
#define NET_CLIENT_ID_ALL_BUT_FLAG ((NetClientId)0b10000000)
#define NET_CLIENT_ID_ALL_BUT(except_id)                                       \
    ((NetClientId)(except_id | NET_CLIENT_ID_ALL_BUT_FLAG))

typedef enum NetPacketType
{
    NPT_EMPTY = 0,
    NPT_CONNECTION_REQUEST,
    NPT_CONNECTION_RESPONSE,
    NPT_SYNC,
    NPT_DISCONNECT,
    NPT_DATA,
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
    NetClientId sender;
} NetPacketHead;

typedef struct NetPacket
{
    NetPacketHead head;
    unsigned char payload[];
} NetPacket;

typedef struct NetPacketConnectionRequest
{
    NetPacketHead head;
} NetPacketConnectionRequest;

typedef struct NetPacketConnectionResponse
{
    NetPacketHead head;
    NetServerInfo server_info;
    NetClientId assigned_id;
} NetPacketConnectionResponse;

typedef struct NetPacketDisconnect
{
    NetPacketHead head;
} NetPacketDisconnect;

typedef struct NetPacketSync
{
    NetPacketHead head;
} NetPacketSync;

#endif /* MULTIPLAYER_NET_TYPES_H */
