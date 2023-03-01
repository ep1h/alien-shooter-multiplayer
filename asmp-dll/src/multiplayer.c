#include <string.h>
#include "multiplayer.h"
#include "utils/hook/hook.h"
#include "game/addresses.h"
#include "game/game.h"
#include "utils/console/console.h"

static load_menu_t load_menu_tramp_ = 0;
static char mainmenu_file_[] = "maps\\asmp_mainmenu.men";

static int __stdcall _load_menu_hook(const char** menu_file);
static bool set_hooks_(void);

static int __stdcall _load_menu_hook(const char** menu_file)
{
    console_log("load_menu: %s\n", *menu_file);
    if (strcmp(*menu_file, "maps\\mainmenu.men") == 0)
    {
        char* p = mainmenu_file_;
        return load_menu_tramp_((const char**)&p);
    }
    return load_menu_tramp_(menu_file);
}

static bool set_hooks_(void)
{
    load_menu_tramp_ = hook_set((void*)FUNC_LOAD_MENU, _load_menu_hook, 8);
    if (load_menu_tramp_)
    {
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
