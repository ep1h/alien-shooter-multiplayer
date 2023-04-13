/**
 * @file Game.c
 * @brief Implementation of the API for interacting with the main game class.
 *
 */
#include "Game.h"
#include "game/addresses.h"

Game* Game__constructor(Game* ECX, HINSTANCE instance_handle,
                        HINSTANCE prev_instance_handle, const char** argv,
                        int show_cmd, Settings* settings)
{
    return ((Game__init_t)(FUNC_GAME_INIT))(ECX, 0, instance_handle,
                                            prev_instance_handle, argv,
                                            show_cmd, settings);
}

Game* Game__destroy_405110(Game* ECX, char free)
{
    return ((Game__destroy_t)(FUNC_GAME_DESTROY_405110))(ECX, 0, free);
}

int Game__tick(Game* ECX)
{
    return ((Game__tick_t)(FUNC_GAME_TICK))(ECX, 0);
}

Vid* Game__get_vid_by_idx(Game* ECX, enVid vid_idx)
{
    return ((Game__get_vid_by_idx_t)(FUNC_GAME_GET_VID_BY_IDX))(ECX, 0,
                                                                vid_idx);
}

Entity* Game__create_entity(Game* ECX, Vid* vid, float x, float y, float z,
                            int a6, void* parent)
{
    return ((Game__create_entity_t)(FUNC_GAME_CREATE_ENTITY))(ECX, 0, vid, x, y,
                                                              z, a6, parent);
}

Game* Game__init(Game* ECX, HINSTANCE instance_handle,
                 HINSTANCE prev_instance_handle, const char** argv,
                 int show_cmd, Settings* settings)
{
    return ((Game__init_t)(FUNC_GAME_INIT))(ECX, 0, instance_handle,
                                            prev_instance_handle, argv,
                                            show_cmd, settings);
}

Game* Game__destroy_40B140(Game* ECX, char free)
{
    return ((Game__destroy_t)(FUNC_GAME_DESTROY_40B140))(ECX, 0, free);
}

int Game__wnd_proc(Game* ECX, HWND hWnd, uint32_t uMsg, uint32_t wParam,
                   uint32_t lParam)
{
    return ((Game__wnd_proc_t)(FUNC_GAME_WNDPROC))(ECX, 0, hWnd, uMsg, wParam,
                                                  lParam);
}

void Game__load_map(Game* ECX, const char* map_file_path)
{
    ((Game__load_map_t)(FUNC_GAME_LOAD_MAP))(ECX, 0, map_file_path);
}

EntPlayer* Game__get_army_player(Game* ECX, int army_idx)
{
    return ((Game__get_army_player_t)(FUNC_GAME_GET_ARMY_PLAYER))(ECX, 0,
                                                                  army_idx);
}

Entity* Game__get_entity_at_screen_pos(Game* ECX, enGetSpriteType a_type,
                                       int scr_x, int scr_y)
{
    return (
        (Game__get_entity_at_screen_pos_t)(FUNC_GAME_GET_ENTITY_AT_SCREEN_POS))(
        ECX, 0, a_type, scr_x, scr_y);
}

int Game__is_vid_idx_exists(Game* ECX, enVid vid_idx)
{
    return ((Game__is_vid_idx_exists_t)(FUNC_GAME_IS_VID_EXISTS))(ECX, 0,
                                                                  vid_idx);
}

Entity* Game__get_entity_at_pos(Game* ECX, enGetSpriteType a_type, float x,
                                float y, float z, int prev)
{
    return ((Game__get_entity_at_pos_t)(FUNC_GAME_GET_ENTITY_AT_POS))(
        ECX, 0, a_type, x, y, z, prev);
}

double Game__get_ground_z(Game* ECX, float x, float y)
{
    return ((Game__get_ground_z_t)(FUNC_GAME_GET_GROUND_Z))(ECX, 0, x, y);
}

Army* Game__get_army_by_idx(Game* ECX, int army_idx)
{
    return ((Game__get_army_by_idx_t)(FUNC_GAME_GET_ARMY_BY_IDX))(ECX, 0,
                                                                  army_idx);
}
