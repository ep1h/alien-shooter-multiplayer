#ifndef MULTIPLAYER_PROTOCOL
#define MULTIPLAYER_PROTOCOL

#include <stdint.h>

#define MP_MAX_NAME_LEN 15
#define MP_ACTOR_INFO_UPDATE_RATE_MS 60

typedef struct MpServerConfiguration
{
    uint32_t actor_info_update_rate_ms;
} MpServerConfiguration;

typedef struct MpServerInfo
{
    unsigned char map_name_len;
    char map_name[];
} MpServerInfo;

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
    uint8_t armed_weapon;
} MpActorInfo;

typedef struct MpPlayer
{
    MpClientInfo client_info;
    MpActorInfo actor_info;
} MpPlayer;

typedef struct MpPlayerInfo
{
    unsigned short id;
    MpPlayer mp_player;
} MpPlayerInfo;

typedef enum MpPacketType
{
    MPT_EMPTY = 0,
    MPT_C_CONNECTION_REQUEST,
    MPT_S_CONNECTION_RESPONSE,
    MPT_C_PLAYERS_INFO_REQUEST,
    MPT_S_PLAYERS_INFO_RESPONSE,
    MPT_C_ACTOR_INFO,
    MPT_S_ACTORS_INFO,
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

typedef struct MpCPacketConnectionRequest
{
    MpPacketHead head;
    unsigned char name_len;
    char name[];
} MpCPacketConnectionRequest;

typedef struct MpSPacketConnectionResponse
{
    MpPacketHead head;
    MpServerConfiguration server_configuration;
    MpServerInfo server_info;
} MpSPacketConnectionResponse;

typedef struct MpCPacketPlayersInfoRequest
{
    MpPacketHead head;
} MpCPacketPlayersInfoRequest;

typedef struct MpSPacketPlayersInfo
{
    MpPacketHead head;
    unsigned char players_number;
    MpPlayerInfo palyers[];
} MpSPacketPlayersInfo;

typedef struct MpCPacketActorInfo
{
    MpPacketHead head;
    uint32_t timestamp_ms;
    MpActorInfo mp_actor_info;
} MpCPacketActorInfo;

typedef struct MpInfoWrapper
{
    unsigned char id;
    uint32_t timestamp_ms;
    unsigned char payload[];
} MpInfoWrapper;

typedef struct MpPacketActorsInfo
{
    MpPacketHead head;
    unsigned char actors_number;
    MpInfoWrapper info_wrapper[];
} MpPacketActorsInfo;

#endif /* MULTIPLAYER_PROTOCOL */
