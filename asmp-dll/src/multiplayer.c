#include <string.h>
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
} State;

#define STATUSBAR_NDIR    0
#define NAME_TEXT_NDIR    2
#define ADDRESS_TEXT_NDIR 3

static State state_ = STATE_NONE;

static load_menu_t load_menu_tramp_ = 0;
static char mainmenu_file_[] = "maps\\asmp_mainmenu.men";

static int __stdcall _load_menu_hook(const char** menu_file);
static int __thiscall _Game__tick_hook(Game* this);
static bool set_hooks_(void);

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
