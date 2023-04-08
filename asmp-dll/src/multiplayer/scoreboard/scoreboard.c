#include "scoreboard.h"
#include "utils/mem/mem.h"
#include "game/api.h"

typedef struct ScoreboardEntry
{
    int id;
    char name[32];
    int score;
    int latency;
} ScoreboardEntry;

typedef struct Scoreboard
{
    ScoreboardEntry* entries;
    int entries_number;
    int x;
    int y;
    int w;
    int h;
    EntText* header_nickname;
    EntText* header_score;
    EntText* header_latency;
} Scoreboard;

static char header_nickname_text[] = "Nickname";
#include "console.h"
Scoreboard* scoreboard_create(int entries_number)
{
    Scoreboard* scoreboard = mem_alloc(sizeof(*scoreboard));
    if (scoreboard)
    {
        mem_set(scoreboard, 0, sizeof(*scoreboard));
        scoreboard->entries =
            mem_alloc(sizeof(*scoreboard->entries) * entries_number);
        if (scoreboard->entries)
        {
            mem_set(scoreboard->entries, 0,
                    sizeof(*scoreboard->entries) * entries_number);
            scoreboard->entries_number = entries_number;
            scoreboard->w = 382;
            scoreboard->h = 300;
            scoreboard->x =
                (game_globals_get_render()->width - scoreboard->w) * 0.5f;
            scoreboard->y =
                (game_globals_get_render()->height - scoreboard->h) * 0.5f;

            scoreboard->header_nickname =
                (EntText*)game_globals_get_game()->__vftable->create_entity(
                    game_globals_get_game(), 0,
                    game_globals_get_game()->vids[VID_004_MENU_FONT_SMALL], 100,
                    100, 1.0f, 0, 0);
            // scoreboard->x + 24, scoreboard->y + 2, 1.0f, 0, 0);

            if (scoreboard->header_nickname)
            {
                scoreboard->header_nickname->entity.entity.x = 100.0f;
                scoreboard->header_nickname->entity.entity.y = 100.0f;
                scoreboard->header_nickname->entity.entity.z = 0.0f;

                scoreboard->header_nickname->text_70 = header_nickname_text;
                // List__remove_item_by_ptr((List*)&game_globals_get_game()->list_menu,
                // 0,
                //                          (Entity*)scoreboard->header_nickname);
                console_log("created: %p\n", scoreboard->header_nickname);
                return scoreboard;
            }
        }
    }
    return 0;
}

void scoreboard_destroy(Scoreboard* scoreboard)
{
    if (scoreboard)
    {
        if (scoreboard->entries)
        {
            mem_free(scoreboard->entries);
        }
        mem_free(scoreboard);
    }
}

void scoreboard_draw(Scoreboard* scoreboard)
{
    if (!scoreboard)
    {
        return;
    }
    return; // TOOD: Uncomment later.

    /* Draw background */
    Render__draw_colored_rect(game_globals_get_render(), scoreboard->x,
                              scoreboard->y, scoreboard->x + scoreboard->w,
                              scoreboard->y + scoreboard->h, 0x99000000);
    /* Draw header line */
    Render__draw_colored_rect(
        game_globals_get_render(), scoreboard->x + 5, scoreboard->y + 20,
        scoreboard->x + scoreboard->w - 4 - 4, scoreboard->y + 21, 0xFFFFA106);
    /* Draw border */
    Render__draw_colored_rect(game_globals_get_render(), scoreboard->x,
                              scoreboard->y, scoreboard->x + scoreboard->w,
                              scoreboard->y + 1, 0xFFA89DAE);
    Render__draw_colored_rect(game_globals_get_render(), scoreboard->x,
                              scoreboard->y + scoreboard->h - 1,
                              scoreboard->x + scoreboard->w,
                              scoreboard->y + scoreboard->h, 0xFFA89DAE);
    Render__draw_colored_rect(game_globals_get_render(), scoreboard->x,
                              scoreboard->y, scoreboard->x + 1,
                              scoreboard->y + scoreboard->h, 0xFFA89DAE);
    Render__draw_colored_rect(game_globals_get_render(),
                              scoreboard->x + scoreboard->w - 1, scoreboard->y,
                              scoreboard->x + scoreboard->w,
                              scoreboard->y + scoreboard->h, 0xFFA89DAE);
}

ScoreboardEntry* scoreboard_get_entry(Scoreboard* scoreboard)
{
    (void)scoreboard;
    return 0;
}
