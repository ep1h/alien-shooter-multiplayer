#ifndef MULTIPLAYER_PROTOCOL
#define MULTIPLAYER_PROTOCOL

#include <stdint.h>

#define MP_MAX_NAME_LEN 15

typedef struct MpServerInfo
{
    unsigned char map_name_len;
    char map_name[];
}MpServerInfo;

typedef struct MpClientInfo
{
    unsigned char id;
    char name[MP_MAX_NAME_LEN + 1];
} MpClientInfo;

typedef struct MpActorInfo
{
    float x;
    float y;
    float z;
    float velocity;
    uint8_t direction_legs;
    uint8_t direction_torso;
    uint8_t health;
} MpActorInfo;

typedef struct MpPlayer
{
    MpClientInfo client_info;
    MpActorInfo actor_info;
} MpPlayer;

typedef enum MpPacketType
{
    MPT_EMPTY = 0,
    MPT_CONNECT,
    MPT_DISCONNECT,
    MPT_CONNECTION_REQUEST,
    MPT_CONNECTION_RESPONSE,
    MPT_PLAYERS_INFO,
} MpPacketType;

typedef struct MpPacketHead
{
    MpPacketType type;
} MpPacketHead;

typedef struct MpPacket
{
    MpPacketHead head;
    unsigned char payload[];
} MpPacket;

typedef struct MpPacketConnect
{
    MpPacketHead head;
    MpClientInfo client_info;
} MpPacketConnect;

typedef struct MpPacketConnectionRequest
{
    MpPacketHead head;
    unsigned char name_len;
    char name[];
} MpPacketConnectionRequest;

typedef struct MpPacketConnectionResponse
{
    MpPacketHead head;
    MpServerInfo server_info;
} MpPacketConnectionResponse;

#endif /* MULTIPLAYER_PROTOCOL */
