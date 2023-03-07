#ifndef MULTIPLAYER_PROTOCOL
#define MULTIPLAYER_PROTOCOL

#include <stdint.h>

#define MP_MAX_NAME_LEN 15

typedef struct MpClientInfo
{
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
    MPT_DISCONNECT
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

#endif /* MULTIPLAYER_PROTOCOL */
