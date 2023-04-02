/**
 * @file multiplayer.c
 * @brief Implementation of multiplayer logic.
 *
 */
#include <string.h>
#include <stdlib.h>
#include "multiplayer.h"
#include "client/client.h"
#include "utils/mem/mem.h"
#include "utils/hook/hook.h"
#include "game/game.h"
#include "game/addresses.h"
#include "game/types/entities/EntText.h"
#include "game/types/vids/Vid.h"
#include "gameutils.h"
#include "utils/console/console.h" // TODO: Remove later.


#define STATUSBAR_NDIR     0
#define NAME_TEXT_NDIR     2
#define ADDRESS_TEXT_NDIR  3
#define CONNECT_BUTTON_DIR 25 /* NDIR = 1 */


typedef enum MultiplayerState
{
    MULTIPLAYER_STATE_NONE,
    MULTIPLAYER_STATE_MAIN_MENU,
    MULTIPLAYER_STATE_MULTIPLAYER_MENU,
    MULTIPLAYER_STATE_CONNECTED,
} MultiplayerState;

typedef enum StateConnectedSubstate
{
    SCS_JUST_CONNECTED = 0,
    SCS_WAIT_MAP_LOAD,
    SCS_MAP_JUST_LOADED,
    SCS_PLAYER_JUST_SPAWNED,
    SCS_PLAY,
} StateConnectedSubstate;

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

typedef enum RemotePlayerState
{
    RPS_NOT_SPAWNED = 0,
    RPS_SPAWNING,
    RPS_JUST_SPAWNED,
    RPS_SPAWNED,
} RemotePlayerState;


typedef struct MultiplayerMenuInfo
{
    EntText* nickname;
    EntText* address;
    EntText* status;
    Entity* connect_button;
} MultiplayerMenuInfo;

typedef struct RemotePlayer
{
    RemotePlayerState state;
    MpPlayer mp_player;
    EntPlayer* entity;
    bool is_actor_info_changed;
    Vid vid;
} RemotePlayer;

typedef struct Multiplayer
{
    MultiplayerState state;
    StateConnectedSubstate state_connected_substate;
    MpClient* mp_client;
    RemotePlayer* remote_players;
    load_menu_t Game__load_menu_trampoline;
    EntPlayer__set_armed_weapon_t EntPlayer__set_armed_weapon_trampoline;
} Multiplayer;


static Multiplayer* mp_ = 0;


/**
 * @brief Hook of Game::wnd_proc function.
 *
 * @param this   This is a macro. The macro expands to 2 arguments:
 *               ECX - pointer to main AlienShooter game class object pointer.
 *               EDX - unused variable (actually this is EDX register value).
 * @param hwnd   Window handle.
 * @param msg    Message.
 * @param wparam Message parameter.
 * @param lparam Message parameter.
 *
 * @return int Result of call to original Game::wnd_proc.
 */
static int __fastcall Game__wnd_proc_hook_(Game* this, HWND hwnd, uint32_t msg,
                                           uint32_t wparam, uint32_t lparam);

/**
 * @brief Hook of Game::load_menu function.
 *
 * @param menu_file Pointer to pointer to name of menu file to load.
 *
 * @return int Result of call to original Game::load_menu.
 */
static int __stdcall Game__load_menu_hook_(const char** menu_file);

/**
 * @brief Hook of Game::tick function.
 *
 * @param this This is a macro. The macro expands to 2 arguments:
 *             ECX - pointer to main AlienShooter game class object pointer.
 *             EDX - unused variable (actually this is EDX register value).
 *
 * @return int Result of call original Game::tick.
 */
static int __thiscall Game__tick_hook_(Game* this);

/**
 * @brief Hook of EntPlayer::set_armed_weapon function.
 *
 * @param this           This is a macro. The macro expands to 2 arguments:
 *                       ECX - pointer to entity who is calling this function.
 *                       EDX - unused variable (actually this is EDX register
 *                       value).
 * @param weapon_slot_id Weapon slot id.
 *
 * @return int Result of call to original EntPlayer::set_armed_weapon:
 *             0 - weapon was not changed.
 *             1 - weapon was changed.
 */
static int __thiscall EntPlayer__set_armed_weapon_hook_(EntPlayer* this,
                                                        int weapon_slot_id);

/**
 * @brief Hook of EntPlayer::action function.
 *
 * @param this   This is a macro. The macro expands to 2 arguments:
 *               ECX - pointer to entity who is calling this function.
 *               EDX - unused variable (actually this is EDX register value).
 * @param action Action to perform.
 * @param a3     Action param 1.
 * @param a4     Action param 2.
 * @param a5     Action param 3.
 *
 * @return int Result of call to original EntPlayer::action.
 */
static int __thiscall EntPlayer__action_hook_(Entity* this, enEntAction action,
                                              void* a3, void* a4, void* a5);

// TODO: Should be removed after refactoring.
static void on_actor_info_updated(MpClient* client, int id, MpActor* actor);

/**
 * @brief Sets all necessary hooks.
 *
 * @return true If all hooks were set successfully.
 * @return false If at least one hook was not set successfully.
 */
static bool set_hooks_(void);

/**
 * @brief Reads states of multiplayer menu entities to interact with
 *        asmp_play.lgc script.
 *
 * @param[in,out] multiplayer_menu_info MultiplayerMenuInfo structure pointer.
 *
 * @return true if all required entities were found and stored to argument.
 */
static bool read_mp_menu_info_(MultiplayerMenuInfo* multiplayer_menu_info);

/**
 * @brief Parses ip and port from string.
 *
 * @param[in]  str String to parse.
 * @param[out] ip  Pointer to buffer to store ip address.
 * @param[out] port Pointer to variable to store port.
 *
 * @return true if \p ip and \p port are not NULL and \p str matches ip:port.
 */
static bool parse_ip_port_(const char* str, char* ip, uint16_t* port);

/**
 * @brief Calls each game tick when multiplayer menu is active.
 */
static void handle_multiplayer_state_multiplayer_menu_(void);

/**
 * @brief Calls each game tick when client is connected to server.
 */
static void handle_multiplayer_state_connected_(void);

/**
 * @brief Reflects changes of remote players to game world.
 */
static void handle_remote_players_(void);

static int __fastcall Game__wnd_proc_hook_(Game* this, HWND hwnd, uint32_t msg,
                                           uint32_t wparam, uint32_t lparam)
{
    if (msg == 0x001C) /* WM_ACTIVATEAPP */
    {
        /* Prevent freezing game loop when game window is not active */
        wparam = 1;
    }
    return ((Game__wnd_proc_t)FUNC_GAME_WNDPROC)(ECX, EDX, hwnd, msg, wparam,
                                                 lparam);
}

static int __stdcall Game__load_menu_hook_(const char** menu_file)
{
    // console_log("Game::load_menu: %s\n", *menu_file);
    /* If main menu, load the custom one instead */
    if (strcmp(*menu_file, "maps\\mainmenu.men") == 0)
    {
        mp_client_disconnect(mp_->mp_client);
        const char custom_mainmenu_file_name[] = "maps\\asmp_mainmenu.men";
        const char* ptr = custom_mainmenu_file_name;
        return mp_->Game__load_menu_trampoline((const char**)&ptr);
    }
    else if (strcmp(*menu_file, "maps\\asmp_play.men") == 0)
    {
        mp_->state = MULTIPLAYER_STATE_MULTIPLAYER_MENU;
    }
    return mp_->Game__load_menu_trampoline(menu_file);
}

static int __thiscall Game__tick_hook_(Game* this)
{
    int result = ((Game__tick_t)FUNC_GAME_TICK)(ECX, EDX);
    multiplayer_tick();
    return result;
}

static int __thiscall EntPlayer__set_armed_weapon_hook_(EntPlayer* this,
                                                        int weapon_slot_id)
{
    int result =
        mp_->EntPlayer__set_armed_weapon_trampoline(ECX, EDX, weapon_slot_id);
    if (result && ECX == gameutils_get_player())
    {
        MpPlayer* mp_player = mp_client_get_local_player(mp_->mp_client);
        if (mp_player)
        {
            /* Update local player's armed weapon in client structure */
            mp_player->mp_actor.armed_weapon = weapon_slot_id;
        }
    }
    return result;
}

static int __thiscall EntPlayer__action_hook_(Entity* this, enEntAction action,
                                              void* a3, void* a4, void* a5)
{
    return ((Entity__action_t)FUNC_ENT_PLAYER_ACTION)(ECX, EDX, action, a3, a4,
                                                      a5);
}

static void on_actor_info_updated(MpClient* client, int id, MpActor* actor)
{
    (void)client;
    mp_->remote_players[id].mp_player.mp_actor = *actor;
    mp_->remote_players[id].is_actor_info_changed = true;
}

static bool set_hooks_(void)
{
    /* Game::wndproc hook */
    if (hook_set_vmt((void**)&game_get_game()->__vftable->wnd_proc,
                     Game__wnd_proc_hook_))
    {
        /* Game::load_menu hook */
        mp_->Game__load_menu_trampoline =
            hook_set((void*)FUNC_LOAD_MENU, Game__load_menu_hook_, 8);
        if (mp_->Game__load_menu_trampoline)
        {
            /* Game::tick hook */
            if (hook_set_vmt((void**)&game_get_game()->__vftable->tick,
                             Game__tick_hook_))
            {
                /* EntPlayer::set_armed_weapon hook */
                mp_->EntPlayer__set_armed_weapon_trampoline =
                    hook_set((void*)FUNC_ENT_PLAYER_SET_ARMED_WEAPON,
                             EntPlayer__set_armed_weapon_hook_, 5);
                if (mp_->EntPlayer__set_armed_weapon_trampoline)
                {
                    /* EntPlayer::action hook */
                    if (hook_set_vmt(
                            (void**)&((Entity_vtbl*)ENT_PLAYER_VTBL)->action,
                            &EntPlayer__action_hook_))
                    {

                        return true;
                    }
                }
            }
        }
    }
    return false;
}

static bool read_mp_menu_info_(MultiplayerMenuInfo* multiplayer_menu_info)
{
    if (multiplayer_menu_info)
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
                        multiplayer_menu_info->nickname = name;
                        multiplayer_menu_info->address = addr;
                        multiplayer_menu_info->status = status_bar;
                        multiplayer_menu_info->connect_button = connect_button;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

static bool parse_ip_port_(const char* str, char* ip, uint16_t* port)
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

static void handle_multiplayer_state_multiplayer_menu_(void)
{
    MultiplayerMenuInfo mp_menu;
    if (!read_mp_menu_info_(&mp_menu))
    {
        // TODO: Something definitely went wrong, handle it.
        return;
    }
    StatePlayMenuSubstate substate = mp_menu.status->entity.entity.health;
    switch (substate)
    {
    case SPMS_IDLE:
    {
        break;
    }
    case SPMS_CONNECT_PRESSED:
    {
        console_log("SPMS_CONNECT_PRESSED\n");
        char ip[16] = {0};
        uint16_t port = 0;
        /* Check entered name */
        size_t name_len = strlen(mp_menu.nickname->text_70);
        if (name_len == 0) /* Invalid name len */
        {
            substate = SPMS_INVALID_NAME;
        }
        /* Check entered address */
        else if (!parse_ip_port_(mp_menu.address->text_70, ip, &port))
        {
            /* Invalid address. */
            substate = SPMS_INVALID_ADDRESS;
        }
        else
        {
            console_log("mp_client_connection_request(%s:%d | %s)...\n", ip,
                        port, mp_menu.nickname->text_70);
            if (mp_client_connection_request(mp_->mp_client, ip, port,
                                             mp_menu.nickname->text_70))
            {
                console_log("ok!\n");
                /* Disable 'CONNECT' button */
                Entity__set_anim(mp_menu.connect_button, 0,
                                 ANI_MENUDISABLEDOWN);
                // TODO: Disable edit boxes, player model buttons.
                /* Switch substate to connecting */
                substate = SPMS_CONNECTING;
            }
            else
            {
                substate = SPMS_CONNECTION_FAILED;
            }
        }
        break;
    }
    case SPMS_INVALID_NAME:
    case SPMS_INVALID_ADDRESS:
    {
        console_log("SPMS_INVALID_NAME or SPMS_INVALID_ADDRESS\n");
        substate = SPMS_IDLE;
        break;
    }
    case SPMS_CONNECTING:
    {
        console_log("SPMS_CONNECTING\n");
        /* Monitor mp_client state */
        MpClientState mcs = mp_client_get_state(mp_->mp_client);
        console_log("client state: %d\n", mcs);
        switch (mcs)
        {
        case MP_CLIENT_STATE_CONNECTING:
        {
            /* Just wait for connecting */
            break;
        }
        case MP_CLIENT_STATE_CONNECTED:
        {
            // TODO: Move functionality from this case to SPMS_CONNECTED.
            mp_->remote_players =
                mem_alloc(sizeof(*mp_->remote_players) *
                          mp_client_get_max_players_number(mp_->mp_client));
            if (mp_->remote_players)
            {
                mem_set(mp_->remote_players, 0,
                        sizeof(*mp_->remote_players) *
                            mp_client_get_max_players_number(mp_->mp_client));
                substate = SPMS_CONNECTED;
                mp_->state_connected_substate = SCS_JUST_CONNECTED;
                mp_->state = MULTIPLAYER_STATE_CONNECTED;
            }
            else
            {
                // TODO: Not exactly CONNECTION failed, but allocation failed.
                substate = SPMS_CONNECTION_FAILED;
            }
            break;
        }
        default:
        {
            substate = SPMS_CONNECTION_FAILED;
            break;
        }
        }
        break;
    }
    case SPMS_CONNECTION_FAILED:
    {
        console_log("SPMS_CONNECTION_FAILED\n");
        /* Enable 'CONNECT' button */
        Entity__set_anim(mp_menu.connect_button, 0, ANI_STAND);
        substate = SPMS_IDLE;
        break;
    }
    case SPMS_CONNECTED:
    {
        console_log("SPMS_CONNECTED\n");
        break;
    }
    default:
    {
        console_log("default (%d)\n", substate);
        break;
    }
    }
    /* Send new SPMS back to asmp_play.lgc */
    mp_menu.status->entity.entity.health = substate;
}

static void handle_multiplayer_state_connected_(void)
{
    switch (mp_->state_connected_substate)
    {
    case SCS_JUST_CONNECTED:
    {
        console_log("SCS_JUST_CONNECTED\n");
        const char* map_name =
            mp_client_get_server_configuration(mp_->mp_client)->map_name;
        if (map_name && strlen(map_name))
        {
            Game__load_map(game_get_game(), 0, map_name);
            mp_->state_connected_substate = SCS_WAIT_MAP_LOAD;
        }
        else
        {
            // TODO: Throw an error: map name is empty.
        }
        break;
    }
    case SCS_WAIT_MAP_LOAD:
    {
        console_log("SCS_WAIT_MAP_LOAD\n");
        /* This intermediate state lasts one tick and is required for the
         * game to load a map. */
        mp_->state_connected_substate = SCS_MAP_JUST_LOADED;
        break;
    }
    case SCS_MAP_JUST_LOADED:
    {
        console_log("SCS_MAP_JUST_LOADED\n");
        /* Wait for player to be spawned */
        EntPlayer* local_player = gameutils_get_player();
        if (local_player && ((Entity*)local_player)->child)
        {
            mp_->state_connected_substate = SCS_PLAYER_JUST_SPAWNED;
        }
        break;
    }
    case SCS_PLAYER_JUST_SPAWNED:
    {
        console_log("SCS_PLAYER_JUST_SPAWNED\n");
        mp_->state_connected_substate = SCS_PLAY;
        break;
    }
    case SCS_PLAY:
    {
        /* Update local player's data in client structure */
        MpPlayer* mp_local_player = mp_client_get_local_player(mp_->mp_client);
        if (mp_local_player)
        {
            EntPlayer* local_palyer = gameutils_get_player();
            if (local_palyer && local_palyer->ent_unit.ent_object.entity.child)
            {
                /* Update local player's data in client structure */
                mp_local_player->mp_actor.x = ((Entity*)local_palyer)->x;
                mp_local_player->mp_actor.y = ((Entity*)local_palyer)->y;
                mp_local_player->mp_actor.z = ((Entity*)local_palyer)->z;
                mp_local_player->mp_actor.velocity =
                    ((Entity*)local_palyer)->velocity;
                mp_local_player->mp_actor.direction_legs =
                    ((Entity*)local_palyer)->direction;
                mp_local_player->mp_actor.direction_torso =
                    ((Entity*)local_palyer)->child->direction;
            }
        }
        /* Reflect remote players changes in game */
        handle_remote_players_();
        break;
    }
    }
}

static void handle_remote_players_(void)
{
    for (int i = 0; i < mp_client_get_max_players_number(mp_->mp_client); i++)
    {
        // TOOD: Add if-connected check.
        RemotePlayer* rp = &mp_->remote_players[i];
        if (!rp->is_actor_info_changed)
        {
            continue;
        }
        switch (rp->state)
        {
        case RPS_NOT_SPAWNED:
        {
            console_log("RPS_NOT_SPAWNED for %d\n", i);

            /* Create remote player entity */
            /* Copy default player vid */
            rp->vid = *game_get_game()->vids[VID_009_UNIT_PLAYER_MALE_LEGS];
            /* Create entity */
            rp->entity = (EntPlayer*)gameutils_create_entity(
                &rp->vid, // game_get_game()->vids[VID_009_UNIT_PLAYER_MALE_LEGS],
                rp->mp_player.mp_actor.x, rp->mp_player.mp_actor.y,
                rp->mp_player.mp_actor.z);
            rp->state = RPS_SPAWNING;
            break;
        }
        case RPS_SPAWNING:
        {
            console_log("RPS_SPAWNING for %d\n", i);
            /* Just wait for entity (its legs and torso) to be spawned */
            if (rp->entity && rp->entity->ent_unit.ent_object.entity.child)
            {
                rp->state = RPS_JUST_SPAWNED;
            }
            break;
        }
        case RPS_JUST_SPAWNED:
        {
            /* Load (prepare) all weapons for this player */
            console_log("add weapons: ");
            for (int j = 2; j <= 9; j++)
            {
                console_log("%d ", j);

                if (!((Entity*)rp->entity)
                         ->__vftable->action(
                             (Entity*)rp->entity, 0, ACT_HAVE_ITEM,
                             (void*)(VID_260_OBJECT_UNK_MB_ALIEN_GUN + j), 0,
                             0))
                {
                    ((Entity*)rp->entity)
                        ->__vftable->action(
                            (Entity*)rp->entity, 0, ACT_ADD_ITEM,
                            (void*)(VID_260_OBJECT_UNK_MB_ALIEN_GUN + j), 0, 0);
                }
            }
            console_log("\n");

            rp->state = RPS_SPAWNED;
            break;
        }
        case RPS_SPAWNED:
        {
            const MpActor* actor = &rp->mp_player.mp_actor;
            Entity__move((Entity*)rp->entity, 0, actor->x, actor->y, actor->z);
            Entity__rotate((Entity*)rp->entity, 0, actor->direction_legs);
            if (((Entity*)rp->entity)->child)
            {
                Entity__rotate(((Entity*)rp->entity)->child, 0,
                               actor->direction_torso);
            }
            ((Entity*)rp->entity)->velocity = actor->velocity;
            EntPlayer__set_armed_weapon(rp->entity, 0, actor->armed_weapon);
            rp->is_actor_info_changed = false;
            break;
        }
        }
    }
}

bool multiplayer_init(void)
{
    if (!mp_)
    {
        mp_ = mem_alloc(sizeof(Multiplayer));
        if (mp_)
        {
            mp_->mp_client = mp_client_create();
            if (mp_->mp_client)
            {
                /* Set multiplayer client callbacks */
                mp_client_set_actor_sync_callback(mp_->mp_client,
                                                  &on_actor_info_updated);
                if (set_hooks_())
                {
                    return true;
                }
                mp_client_destroy(mp_->mp_client);
            }
            mem_free(mp_);
            mp_ = 0;
        }
    }
    return mp_ != 0;
}

void multiplayer_destroy(void)
{
    if (mp_)
    {
        mem_free(mp_);
        mp_ = 0;
    }
}

void multiplayer_tick(void)
{
    if (!mp_)
    {
        // TODO: Looks like this check can be removed.
        return;
    }
    mp_client_tick(mp_->mp_client); // TODO: Tick only if connected/connecting.

    switch (mp_->state)
    {
    case MULTIPLAYER_STATE_NONE:
    {
        break;
    }
    case MULTIPLAYER_STATE_MAIN_MENU:
    { /* Just wait for multiplayer menu to be loaded to change state to
        MULTIPLAYER_STATE_MULTIPLAYER_MENU. State switches in
        Game::load_menu hook */
        break;
    }
    case MULTIPLAYER_STATE_MULTIPLAYER_MENU:
    {
        handle_multiplayer_state_multiplayer_menu_();
        break;
    }
    case MULTIPLAYER_STATE_CONNECTED:
    {
        handle_multiplayer_state_connected_();
        break;
    }
    }
}

Multiplayer* multiplayer_instance(void)
{
    return mp_;
}
