#include <string.h>
#include <stdlib.h>
#include "multiplayer.h"
#include "utils/hook/hook.h"
#include "game/addresses.h"
#include "game/game.h"
#include "utils/console/console.h"
#include "gameutils.h"
#include "game/types/entities/EntText.h"
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

typedef struct PlayMenu
{
    EntText* nickname;
    EntText* address;
    EntText* status;
    Entity* connect_button;
} PlayMenu;

static State state_ = STATE_NONE;
static char ip_[16] = {0};
static uint16_t port_ = 0;
static load_menu_t load_menu_tramp_ = 0;
static char mainmenu_file_[] = "maps\\asmp_mainmenu.men";
static MpClient* client_;

static bool read_play_menu_(PlayMenu* out_play_menu);
static bool parse_address_(const char* str, char* ip, uint16_t* port);
static int __stdcall _load_menu_hook(const char** menu_file);
static int __thiscall _Game__tick_hook(Game* this);
static void state_play_menu_handler_(void);
static void state_connected_handler_(void);
static bool set_hooks_(void);

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
        mp_client_disconnect(client_);
        state_ = STATE_NONE;
        char* p = mainmenu_file_;
        return load_menu_tramp_((const char**)&p);
    }
    else if (strcmp(*menu_file, "maps\\asmp_play.men") == 0)
    {
        state_ = STATE_PLAY_MENU;
    }
    return load_menu_tramp_(menu_file);
}

static int __thiscall _Game__tick_hook(Game* this)
{
    mp_client_tick(client_);
    switch (state_)
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
        else if (!parse_address_(play_menu.address->text_70, ip_, &port_))
        {
            /* Invalid address. */
            substate = SPMS_INVALID_ADDRESS;
        }
        else
        {
            mp_client_connection_request(client_, ip_, port_,
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
        MpClientState mcs = mp_client_get_state(client_);
        if (mcs == MPS_CONNECTED)
        {
            substate = SPMS_CONNECTED;
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
    case SPMS_CONNECTED:
    {
        state_ = STATE_CONNECTED;
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
}

static bool set_hooks_(void)
{
    load_menu_tramp_ = hook_set((void*)FUNC_LOAD_MENU, _load_menu_hook, 8);
    if (load_menu_tramp_)
    {
        if (hook_set_vmt((void**)&game_get_game()->__vftable->tick,
                         _Game__tick_hook))
        {
            if (hook_set_vmt((void**)&game_get_game()->__vftable->wnd_proc,
                             Game__wnd_proc_hook))
            {
                return true;
            }
        }
    }
    return false;
}

bool multiplayer_init(void)
{
    if (set_hooks_())
    {
        client_ = mp_client_create();
        if (client_)
        {
            return true;
        }
    }
    return false;
}

void multiplayer_destroy(void)
{
    mp_client_destroy(client_);
}
