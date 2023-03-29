#include <string.h>
#include <stdlib.h>
#include "multiplayer.h"
#include "utils/mem/mem.h"
#include "utils/hook/hook.h"
#include "game/addresses.h"
#include "game/game.h"
#include "game/types/Army.h"
#include "game/types/entities/EntPlayer.h"
#include "utils/console/console.h"
#include "gameutils.h"
#include "game/types/entities/EntText.h"
#include "game/types/vids/Vid.h"
#include "client/client.h"

#define STATUSBAR_NDIR     0
#define NAME_TEXT_NDIR     2
#define ADDRESS_TEXT_NDIR  3
#define CONNECT_BUTTON_DIR 25 // NDIR = 1

typedef enum State
{
    STATE_NONE,
    STATE_PLAY_MENU,
    STATE_CONNECTED,
} State;

typedef enum StatePlayMenuSubstate
{
    SPMS_IDLE = 0,
    SPMS_CONNECT_PRESSED = 1,
    SPMS_INVALID_NAME = 2,
    SPMS_INVALID_ADDRESS = 3,
    SPMS_CONNECTING = 4,
    SPMS_CONNECTION_FAILED = 5,
    SPMS_CONNECTED = 6
} StatePlayMenuSubstate;

typedef enum StateConnectedSubstate
{
    SCS_IDLE = 0,
    SCS_WAIT_LOAD_MAP,
    SCS_WAIT_SPAWN,
    SCS_SPAWNED,
} StateConnectedSubstate;

typedef struct PlayMenu
{
    EntText* nickname;
    EntText* address;
    EntText* status;
    Entity* connect_button;
} PlayMenu;

typedef struct Client
{
    Entity* entity;
    Vid vid;
    MpPlayer* mp_player;
} Client;

typedef struct Multiplayer
{
    MpClient* client;
    State state;
    char ip[16];
    uint16_t port;
    load_menu_t load_menu_tramp;
    EntPlayer__set_armed_weapon_t EntPlayer__set_armed_weapon_tramp;
    StateConnectedSubstate connected_substate;
    Client* clients;
} Multiplayer;

long(__stdcall* EndScene_orig)(void*);
static Multiplayer* mp = 0;

static const char mainmenu_file_[] = "maps\\asmp_mainmenu.men";

static EntPlayer* get_local_layer_(void);
static bool read_play_menu_(PlayMenu* out_play_menu);
static bool parse_address_(const char* str, char* ip, uint16_t* port);
static int __stdcall _load_menu_hook(const char** menu_file);
static int __thiscall _Game__tick_hook(Game* this);
static long __stdcall EndScene_hook(void* id3d8dev);
static int __thiscall EntPlayer__set_armed_weapon_hook(EntPlayer* this,
                                                       int weapon_slot_id);
static void state_play_menu_handler_(void);
static void state_connected_handler_(void);
static bool set_hooks_(void);

static EntPlayer* get_local_layer_(void)
{
    Game* game = game_get_game();
    int army_idx = game_get_game()->mb_flagman_army;
    if (army_idx < 4)
    {
        Army* army = game->army[army_idx];
        if (army)
        {
            EntPlayer* player = army->player;
            if (player)
            {
                /* If player torso is inited */
                if (player->ent_unit.ent_object.entity.child)
                {
                    return player;
                }
            }
        }
    }
    return 0;
}

static bool read_play_menu_(PlayMenu* out_play_menu)
{
    if (out_play_menu)
    {
        EntText* name = (EntText*)gameutils_get_menu_item(
            VID_004_MENU_FONT_SMALL, NAME_TEXT_NDIR);
        if (name)
        {
            EntText* addr = (EntText*)gameutils_get_menu_item(
                VID_004_MENU_FONT_SMALL, ADDRESS_TEXT_NDIR);
            if (addr)
            {
                EntText* status_bar = (EntText*)gameutils_get_menu_item(
                    VID_002_MENU_FONT, STATUSBAR_NDIR);
                if (status_bar)
                {
                    Entity* connect_button = gameutils_get_menu_item(
                        VID_705_MENU_BUTTON_BACKGROUND, CONNECT_BUTTON_DIR);
                    if (connect_button)
                    {
                        out_play_menu->nickname = name;
                        out_play_menu->address = addr;
                        out_play_menu->status = status_bar;
                        out_play_menu->connect_button = connect_button;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

static bool parse_address_(const char* str, char* ip, uint16_t* port)
{
    char* colon_pos = strchr(str, ':');
    if (!colon_pos)
    {
        return false;
    }
    uint32_t ip_len = colon_pos - str;
    if (ip_len == 0 || ip_len >= 16)
    {
        return false;
    }
    uint32_t port_len = strlen(str) - ip_len - 1;
    if (port_len == 0)
    {
        return false;
    }
    strncpy(ip, str, ip_len);
    ip[ip_len] = '\0';
    *port = atoi(colon_pos + 1);
    return true;
}

static int __stdcall _load_menu_hook(const char** menu_file)
{
    console_log("load_menu: %s\n", *menu_file);
    if (strcmp(*menu_file, "maps\\mainmenu.men") == 0)
    {
        mp_client_disconnect(mp->client);
        mp->state = STATE_NONE;
        const char* p = mainmenu_file_;
        return mp->load_menu_tramp((const char**)&p);
    }
    else if (strcmp(*menu_file, "maps\\asmp_play.men") == 0)
    {
        mp->state = STATE_PLAY_MENU;
    }
    return mp->load_menu_tramp(menu_file);
}

static int __thiscall _Game__tick_hook(Game* this)
{
    mp_client_tick(mp->client);
    switch (mp->state)
    {
    case STATE_PLAY_MENU:
    {
        state_play_menu_handler_();
        break;
    }
    case STATE_CONNECTED:
    {
        state_connected_handler_();
        break;
    }
    default:
    {
        break;
    }
    }
    return ((Game__tick_t)FUNC_GAME_TICK)(ECX, EDX);
}

int __fastcall Game__wnd_proc_hook(Game* this, HWND hwnd, uint32_t msg,
                                   uint32_t wparam, uint32_t lparam)
{
    if (msg == 0x001C) /* WM_ACTIVATEAPP */
    {
        /* Prevent game loop freething when game window loses focus */
        wparam = 1;
    }
    return ((Game__wnd_proc_t)FUNC_GAME_WNDPROC)(ECX, EDX, hwnd, msg, wparam,
                                                 lparam);
}

long __stdcall EndScene_hook(void* id3d8dev)
{
    return EndScene_orig(id3d8dev);
}

static int __thiscall EntPlayer__set_armed_weapon_hook(EntPlayer* this,
                                                       int weapon_slot_id)
{
    int result =
        mp->EntPlayer__set_armed_weapon_tramp(ECX, EDX, weapon_slot_id);
    if (result && ECX == get_local_layer_())
    {
        MpPlayer* mp_player = mp_client_get_local_player(mp->client);
        if (mp_player)
        {
            mp_player->actor_info.armed_weapon = weapon_slot_id;
        }
    }
    return result;
}

static void state_play_menu_handler_(void)
{
    PlayMenu play_menu = {0};
    if (!read_play_menu_(&play_menu))
    {
        // TODO: Something defenitely wrong. Throw an error.
        return;
    }
    StatePlayMenuSubstate substate = play_menu.status->entity.entity.health;
    switch (substate)
    {
    case SPMS_CONNECT_PRESSED:
    {
        /* Check entered name */
        size_t name_len = strlen(play_menu.nickname->text_70);
        if (name_len == 0)
        {
            /* Invalid name len */
            substate = SPMS_INVALID_NAME;
        }
        /* Check entered address */
        else if (!parse_address_(play_menu.address->text_70, mp->ip, &mp->port))
        {
            /* Invalid address. */
            substate = SPMS_INVALID_ADDRESS;
        }
        else
        {
            mp_client_connection_request(mp->client, mp->ip, mp->port,
                                         play_menu.nickname->text_70);
            /* Disable button */
            Entity__set_anim(play_menu.connect_button, 0, ANI_MENUDISABLEDOWN);
            substate = SPMS_CONNECTING;
        }
        break;
    }
    case SPMS_INVALID_NAME:
    {
        break;
    }
    case SPMS_INVALID_ADDRESS:
    {
        break;
    }
    case SPMS_CONNECTING:
    {
        MpClientState mcs = mp_client_get_state(mp->client);
        if (mcs == MPS_CONNECTED)
        {
            play_menu.status->entity.entity.health = substate;
            mp->clients = mem_alloc(sizeof(*mp->clients) *
                                    mp_client_get_max_players(mp->client));
            if (mp->clients)
            {
                mem_set(mp->clients, 0,
                        sizeof(*mp->clients) *
                            mp_client_get_max_players(mp->client));
                substate = SPMS_CONNECTED;
                mp->state = STATE_CONNECTED;
            }
        }
        else if (mcs == MPS_CONNECTION_FAILED)
        {
            substate = SPMS_CONNECTION_FAILED;
        }
        break;
    }
    case SPMS_CONNECTION_FAILED:
    {
        /* Enable 'CONNECT' button */
        Entity__set_anim(play_menu.connect_button, 0, ANI_STAND);
        break;
    }
    case SPMS_CONNECTED: // TODO: Remove this substate.
    {
        mp->state = STATE_CONNECTED;
        break;
    }
    case SPMS_IDLE:
    default:
    {
        break;
    }
    }
    play_menu.status->entity.entity.health = substate;
}

static void state_connected_handler_(void)
{
    switch (mp->connected_substate)
    {
    case SCS_IDLE:
    {
        const char* map_name = mp_client_get_map_name(mp->client);
        if (map_name && strlen(map_name))
        {
            Game__load_map(game_get_game(), 0, map_name);
            mp->connected_substate = SCS_WAIT_LOAD_MAP;
        }
        break;
    }
    case SCS_WAIT_LOAD_MAP:
    {
        /* This intermediate state lasts one tick and is required for the game
         * to load a map. */
        mp->connected_substate = SCS_WAIT_SPAWN;
        break;
    }
    case SCS_WAIT_SPAWN:
    {
        EntPlayer* local_player = get_local_layer_();
        if (local_player && local_player->ent_unit.ent_object.entity.child)
        {
            mp->connected_substate = SCS_SPAWNED;
        }
        break;
    }
    case SCS_SPAWNED:
    {
        EntPlayer* local_player = get_local_layer_();
        if (!local_player || !local_player->ent_unit.ent_object.entity.child)
        {
            mp->connected_substate = SCS_WAIT_SPAWN;
            break;
        }
        /* Update local actor info in client structures */
        Entity* local_player_entity = &local_player->ent_unit.ent_object.entity;
        MpPlayer* mp_player_local = mp_client_get_local_player(mp->client);
        mp_player_local->actor_info.x = local_player_entity->x;
        mp_player_local->actor_info.y = local_player_entity->y;
        mp_player_local->actor_info.z = local_player_entity->z;
        mp_player_local->actor_info.velocity = local_player_entity->velocity;
        mp_player_local->actor_info.direction_legs =
            local_player_entity->direction;
        mp_player_local->actor_info.direction_torso =
            local_player_entity->child->direction;
        mp_player_local->actor_info.health = local_player_entity->health;

        /* Update remote actors info */
        for (int i = 0; i < mp_client_get_max_players(mp->client); i++)
        {
            if (i == mp_client_get_local_player_id(mp->client))
            {
                continue;
            }
            const MpPlayer* p = mp_client_get_player(mp->client, i);
            if (p)
            {
                if (!mp->clients[i].entity)
                {
                    mem_copy(
                        &mp->clients[i].vid,
                        game_get_game()->vids[VID_009_UNIT_PLAYER_MALE_LEGS],
                        sizeof(Vid));
                    mp->clients[i].entity =
                        game_get_game()->__vftable->create_entity(
                            game_get_game(), 0, &mp->clients[i].vid,
                            p->actor_info.x, p->actor_info.y, p->actor_info.z,
                            0, 0);
                    if (mp->clients[i].entity)
                    {
                        /* Add weapons */
                        for (int j = 2; j <= 9; j++)
                        {
                            mp->clients[i].entity->__vftable->action(
                                mp->clients[i].entity, 0, ACT_ADD_ITEM,
                                (void*)(260 + j), 0, 0);
                        }
                    }
                }
                if (!mp->clients[i].entity || !mp->clients[i].entity->child)
                {
                    continue;
                }
                Entity__move(mp->clients[i].entity, 0, p->actor_info.x,
                             p->actor_info.y, p->actor_info.z);
                mp->clients[i].entity->velocity = p->actor_info.velocity;
                Entity__rotate(mp->clients[i].entity, 0,
                               p->actor_info.direction_legs);
                Entity__rotate(mp->clients[i].entity->child, 0,
                               p->actor_info.direction_torso);
                mp->clients[i].entity->velocity = p->actor_info.velocity;
                mp->clients[i].entity->health = p->actor_info.health;
                EntPlayer__set_armed_weapon((EntPlayer*)mp->clients[i].entity,
                                            0, p->actor_info.armed_weapon);
            }
        }
        break;
    }
    }
}

static bool set_hooks_(void)
{
    mp->load_menu_tramp = hook_set((void*)FUNC_LOAD_MENU, _load_menu_hook, 8);
    if (mp->load_menu_tramp)
    {
        if (hook_set_vmt((void**)&game_get_game()->__vftable->tick,
                         _Game__tick_hook))
        {
            if (hook_set_vmt((void**)&game_get_game()->__vftable->wnd_proc,
                             Game__wnd_proc_hook))
            {
                EndScene_orig = hook_set_vmt(
                    *(void***)(game_get_render()->IDirect3DDevice8) + 35,
                    EndScene_hook);
                if (EndScene_orig)
                {
                    mp->EntPlayer__set_armed_weapon_tramp =
                        hook_set((void*)FUNC_ENT_PLAYER_SET_ARMED_WEAPON,
                                 EntPlayer__set_armed_weapon_hook, 5);
                    if (mp->EntPlayer__set_armed_weapon_tramp)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Multiplayer* multiplayer_create(void)
{
    if (mp)
    {
        return mp;
    }
    mp = mem_alloc(sizeof(Multiplayer));
    if (mp)
    {
        mem_set(mp, 0, sizeof(*mp));
        if (set_hooks_())
        {
            mp->client = mp_client_create();
            if (mp->client)
            {
                return mp;
            }
        }
        mem_free(mp);
    }
    return 0;
}

void multiplayer_destroy(Multiplayer* mp)
{
    if (mp)
    {
        mp_client_destroy(mp->client);
        mp = 0;
    }
}
