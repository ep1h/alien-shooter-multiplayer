/**
 * @file Game.h
 * @brief Provides an API for interacting with the main game class.
 *
 */
#ifndef TYPES_GAME_H
#define TYPES_GAME_H

#include "common.h"
#include "enums.h"
#include "List.h"
#include "File.h"
#include "Input.h"

#ifndef _WINDOWS_
typedef void* HINSTANCE;
typedef void* HWND;
typedef void* HWND;
typedef void* HACCEL;
#endif /* !_WINDOWS_*/

typedef struct Game Game;
typedef struct Game_vtbl Game_vtbl;
typedef struct Army Army;
typedef struct Vid Vid;
typedef struct Entity Entity;

typedef Game*(__thiscall* Game__destroy_t)(Game* this, char free);
typedef int(__thiscall* Game__wnd_proc_t)(Game* this, HWND hwnd, uint32_t msg,
                                          uint32_t wparam, uint32_t lparam);
typedef Vid*(__thiscall* Game__get_vid_by_idx_t)(Game* this, int vid_idx);
typedef Entity*(__thiscall* Game__create_entity_t)(Game* this, Vid* vid,
                                                   float x, float y, float z,
                                                   int a6, void* parent);
typedef int(__thiscall* Game__is_vid_idx_exists_t)(Game* this, int vid_idx);
typedef int(__thiscall* Game__tick_t)(Game* this);
typedef void(__thiscall* Game__load_map_t)(Game* this,
                                           const char* map_file_path);

/**
 * @brief Main game class.
 */
typedef struct __cppobj Game
{
    Game_vtbl* __vftable /*VFT*/;
    uint32_t field_4;
    uint32_t field_8;
    uint32_t flags;
    uint32_t field_10;
    float field_14;
    uint32_t field_18;
    char* cur_map_name;
    uint32_t field_20;
    char* prev_map_name;
    uint32_t field_28;
    uint32_t field_2C;
    uint32_t field_30;
    float w;
    float h;
    uint32_t field_3C;
    uint32_t field_40;
    uint32_t field_44;
    uint32_t field_48;
    uint32_t field_4C;
    float camera_x;
    float camera_y;
    List lists_58[17];
    List list_logic;
    List list_178;
    List list_188;
    uint32_t field_198[10];
    File file_1C0;
    PairedLists pair_vid_entity;
    uint32_t field_220;
    uint32_t field_224;
    uint32_t field_228;
    uint32_t field_22C;
    HINSTANCE hInstance;
    HWND hWnd;
    HACCEL hAccel;
    uint32_t mb_flagman_army;
    Army* army[4];
    Input input;
    ListMenu list_menu;
    uint32_t field_288[9];
    uint32_t field_2AC;
    uint32_t field_2B0;
    int vids_number;
    Vid* vids[2048];
    uint32_t field_22B8;
    uint32_t field_22BC;
    uint32_t field_22C0;
} Game;

/**
 * @brief Virtual method table of the main game class.
 */
typedef struct Game_vtbl
{
    Game__destroy_t destroy;
    Game__wnd_proc_t wnd_proc;
    void* unk_02;
    Game__tick_t tick;
    void* unk_04;
    void* unk_05;
    void* unk_06;
    void* unk_07;
    Game__create_entity_t create_entity;
} Game_vtbl;

/**
 * @brief Loads a map from a file.
 *
 * @param[in] ECX            Game instance pointer.
 * @param[in] map_file_path  Map file path (relative to the game directory).
 */
void Game__load_map(Game* ECX, const char* map_file_path);

#endif /* TYPES_GAME_H */
