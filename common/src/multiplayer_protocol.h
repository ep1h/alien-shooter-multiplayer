#ifndef MULTIPLAYER_PROTOCOL
#define MULTIPLAYER_PROTOCOL

#include <stdint.h>

#define MP_MAX_NAME_LEN              15
#define MP_MAX_MAP_NAME_LEN          24
#define MP_RECV_TIMEOUT_MS           5000
#define MP_SEND_TIMEOUT_MS           5000
#define MP_USER_SYNC_UPDATE_RATE_MS  10000
#define MP_ACTOR_SYNC_UPDATE_RATE_MS 30

typedef struct MpServerConfiguration
{
    uint16_t user_sync_update_rate_ms;
    uint16_t actor_sync_update_rate_ms;
    char map_name[MP_MAX_MAP_NAME_LEN];
} MpServerConfiguration;

typedef struct MpActor
{
    float x;
    float y;
    float z;
    float velocity;
    uint8_t direction_legs;
    uint8_t direction_torso;
    uint8_t armed_weapon;
    uint8_t health;
} MpActor;

typedef struct MpUser
{
    char name[MP_MAX_NAME_LEN + 1];
} MpUser;

typedef struct MpPlayer
{
    MpUser mp_user;
    MpActor mp_actor;
} MpPlayer;

typedef enum MpPacketType
{
    MPT_EMPTY = 0,
    MPT_C_CONNECTION_REQUEST,
    MPT_S_CONENCTION_RESPONSE,
    MPT_C_USER_SYNC,
    MPT_S_USERS_SYNC,
    MPT_C_ACTOR_SYNC,
    MPT_S_ACTORS_SYNC,
    MPT_C_SHOOT,
    MPT_S_SHOOT,
} MpPacketType;

typedef struct MpPacketHead
{
    MpPacketType type;
} MpPacketHead;

typedef struct MpCPacketConnectionRequest
{
    MpPacketHead head;
    uint8_t name_len;
    char name[];
} MpCPacketConnectionRequest;

typedef struct MpSPacketConnectionResponse
{
    MpPacketHead head;
    MpServerConfiguration server_configuration;
} MpSPacketConnectionResponse;

typedef struct MpCPacketUserSync
{
    MpPacketHead head;
    MpUser mp_user;
} MpCPacketUserSync;

typedef struct MpSPacketUsersSyncItem
{
    uint8_t id;
    MpUser mp_user;
} MpSPacketUsersSyncItem;

typedef struct MpSPacketUsersSync
{
    MpPacketHead head;
    uint8_t num_items;
    MpSPacketUsersSyncItem items[];
} MpSPacketUsersSync;

typedef struct MpCPacketActorSync
{
    MpPacketHead head;
    MpActor mp_actor;
} MpCPacketActorSync;

typedef struct MpSPacketActorSyncItem
{
    uint8_t id;
    MpActor mp_actor;
} MpSPacketActorSyncItem;

typedef struct MpSPacketActorsSync
{
    MpPacketHead head;
    uint8_t num_items;
    MpSPacketActorSyncItem items[];
} MpSPacketActorsSync;

typedef struct MpCPacketShoot
{
    MpPacketHead head;
    // TODO: Add weapon id for additional syncing?
    float x;
    float y;
} MpCPacketShoot;

typedef struct MpSPacketShoot
{
    MpPacketHead head;
    uint8_t player_id;
    // TODO: Add weapon id for additional syncing?
    float x;
    float y;
} MpSPacketShoot;

#endif /* MULTIPLAYER_PROTOCOL */
