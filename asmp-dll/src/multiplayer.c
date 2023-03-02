#include <string.h>
#include <stdlib.h>
#include "multiplayer.h"
#include "utils/hook/hook.h"
#include "game/addresses.h"
#include "game/game.h"
#include "utils/console/console.h"
#include "gameutils.h"
#include "game/types/entities/EntText.h"

typedef enum State
{
    STATE_NONE,
    STATE_PLAY_MENU,
    STATE_CONNECTION,
    STATE_CONNECTION_FAILED,
} State;

typedef enum StatusBarHealth
{
    SBH_NONE = 0,
    SBH_PRESSED_CONNECTION = 1,
    SBH_INVALID_NAME = 2,
    SBH_INVALID_ADDRESS = 3,
    SBH_CONNECTION = 4,
    SBH_CONNECTION_FAILED = 5,
} StatusBarHealth;

#define STATUSBAR_NDIR     0
#define NAME_TEXT_NDIR     2
#define ADDRESS_TEXT_NDIR  3
#define CONNECT_BUTTON_DIR 25 // NDIR = 1

static State state_ = STATE_NONE;
static char ip_[16] = {0};
static uint16_t port_ = 0;
static load_menu_t load_menu_tramp_ = 0;
static char mainmenu_file_[] = "maps\\asmp_mainmenu.men";

static bool parse_address_(const char* str, char* ip, uint16_t* port);
static int __stdcall _load_menu_hook(const char** menu_file);
static int __thiscall _Game__tick_hook(Game* this);
static bool set_hooks_(void);

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
    switch (state_)
    {
    case STATE_PLAY_MENU:
    {
        EntText* name = (EntText*)gameutils_get_menu_item(
            VID_004_MENU_FONT_SMALL, NAME_TEXT_NDIR);
        EntText* addr = (EntText*)gameutils_get_menu_item(
            VID_004_MENU_FONT_SMALL, ADDRESS_TEXT_NDIR);
        EntText* status_bar = (EntText*)gameutils_get_menu_item(
            VID_002_MENU_FONT, STATUSBAR_NDIR);

        if (!name || !addr || !status_bar)
        {
            // TODO: Something defenitely wrong. Throw an error.
            break;
        }

        StatusBarHealth sbh = (StatusBarHealth)status_bar->entity.entity.health;
        switch (sbh)
        {
        case SBH_PRESSED_CONNECTION:
        {
            /* 'Connect' button has just been pressed */

            /* Check entered name */
            size_t name_len = strlen(name->text_70);
            if (name_len == 0)
            {
                /* Invalid name len. Send error state to menu. */
                status_bar->entity.entity.health = SBH_INVALID_NAME;
                /* Reset state */
                state_ = STATE_PLAY_MENU;
                break;
            }
            /* Check entered address */
            else if (!parse_address_(addr->text_70, ip_, &port_))
            {
                /* Address is not valid. Send error state to menu. */
                status_bar->entity.entity.health = SBH_INVALID_ADDRESS;
                /* Reset state*/
                state_ = STATE_PLAY_MENU;
                break;
            }
            else
            {
                /* Address is valid */
                /* Send success state to menu (to display 'connecting...') */
                status_bar->entity.entity.health = SBH_CONNECTION;
                /* Switch to connection state */
                state_ = STATE_CONNECTION;
                break;
            }
            break;
        }
        default:
        {
            break;
        }
        }
        break;
    }
    case STATE_CONNECTION:
    {
        /* Disable 'CONNECT' button*/
        Entity* connect_button = gameutils_get_menu_item(
            VID_705_MENU_BUTTON_BACKGROUND, CONNECT_BUTTON_DIR);
        if (!connect_button)
        {
            // TODO: Something defenitely wrong. Throw an error.
            break;
        }
        Entity__set_anim(connect_button, 0, ANI_MENUDISABLEDOWN);
        static int i = 0;
        i++;
        if (i % 500 == 0) // TODO: Just for tests. Should be removed.
            state_ = STATE_CONNECTION_FAILED;
        break;
    }
    case STATE_CONNECTION_FAILED:
    {
        /* Send connection-failed state to menu (to display error) */
        EntText* status_bar = (EntText*)gameutils_get_menu_item(
            VID_002_MENU_FONT, STATUSBAR_NDIR);
        status_bar->entity.entity.health = SBH_CONNECTION_FAILED;

        /* Enable 'CONNECT' button back */
        Entity* connect_button = gameutils_get_menu_item(
            VID_705_MENU_BUTTON_BACKGROUND, CONNECT_BUTTON_DIR);
        if (!connect_button)
        {
            // TODO: Something defenitely wrong. Throw an error.
            break;
        }
        Entity__set_anim(connect_button, 0, ANI_STAND);

        /* Switch state back to PLAY_MENU */
        state_ = STATE_PLAY_MENU;
        break;
    }
    default:
    {
        break;
    }
    }
    return ((Game__tick_t)FUNC_GAME_TICK)(ECX, EDX);
}

static bool set_hooks_(void)
{
    load_menu_tramp_ = hook_set((void*)FUNC_LOAD_MENU, _load_menu_hook, 8);
    if (load_menu_tramp_)
    {
        /* Set VMT hook to game tick function */
        hook_set_vmt((void**)&game_get_game()->__vftable->tick,
                     _Game__tick_hook);
        return true;
    }
    return false;
}

bool multiplayer_init(void)
{
    if (set_hooks_())
    {
        return true;
    }
    return false;
}

void multiplayer_destroy(void)
{
}
