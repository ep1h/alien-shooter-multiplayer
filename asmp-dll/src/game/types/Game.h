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
typedef void* LPARAM;
typedef void* WPARAM;
typedef void* UINT;
#endif /* !_WINDOWS_*/

typedef struct Game Game;
typedef struct Game_vtbl Game_vtbl;
typedef struct Army Army;
typedef struct Vid Vid;
typedef struct Entity Entity;
typedef struct Settings Settings;
typedef struct EntPlayer EntPlayer;

typedef Game*(CC_THISCALL* Game__constructor_t)(Game* this,
                                                HINSTANCE instance_handle,
                                                HINSTANCE prev_instance_handle,
                                                const char** argv, int show_cmd,
                                                Settings* settings);
typedef Game*(CC_THISCALL* Game__destroy_t)(Game* this, char free);
typedef int(CC_THISCALL* Game__tick_t)(Game* this);
typedef Vid*(CC_THISCALL* Game__get_vid_by_idx_t)(Game* this, enVid vid_idx);
typedef Entity*(CC_THISCALL* Game__create_entity_t)(Game* this, Vid* vid,
                                                    float x, float y, float z,
                                                    int a6, void* parent);
typedef Game*(CC_THISCALL* Game__init_t)(Game* this, HINSTANCE instance_handle,
                                         HINSTANCE prev_instance_handle,
                                         const char** argv, int show_cmd,
                                         Settings* settings);
typedef int(CC_THISCALL* Game__wnd_proc_t)(Game* this, HWND hwnd, uint32_t msg,
                                           uint32_t wparam, uint32_t lparam);
typedef void(CC_THISCALL* Game__load_map_t)(Game* this,
                                            const char* map_file_path);
typedef EntPlayer*(CC_THISCALL* Game__get_army_player_t)(Game* this,
                                                         char army_idx);
// typedef uint32_t(CC_THISCALL* Game__get_flagman_army_0x24_t)(Game* this);
typedef Entity*(CC_THISCALL* Game__get_entity_at_screen_pos_t)(
    Game* this, enGetSpriteType a_type, float scr_x, float scr_y);
typedef int(CC_THISCALL* Game__is_vid_idx_exists_t)(Game* this, int vid_idx);
typedef Entity*(CC_THISCALL* Game__get_entity_at_pos_t)(Game* this,
                                                        enGetSpriteType a_type,
                                                        float x, float y,
                                                        float z, int prev);
typedef double(CC_THISCALL* Game__get_ground_z_t)(Game* this, float x, float y);
typedef Army*(CC_THISCALL* Game__get_army_by_idx_t)(Game* this, char army_idx);

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
    List list_ground_single_unk;
    List list_68;
    List list_78;
    List list_gui_plates_in_game_unk;
    List list_effects_unk;
    List list_A8;
    List list_objects;
    List list_C8;
    List list_gui_plates_unk_2;
    List list_E8;
    List list_F8;
    List list_torch_ground;
    List list_torch_rays;
    List list_texts;
    List lists_138[3];
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

typedef struct Settings
{
    char app_name[16];
    uint32_t unk_10[134];
    uint32_t screen_x;
    uint32_t screen_y;
    uint32_t bpp;
    uint32_t fullscreen;
} Settings;


/**
 * @brief Creates a new game instance.
 *
 * @param[in] ECX                   Buffer to store the game instance.
 * @param[in] instance_handle       Handle to the current application instance.
 * @param[in] prev_instance_handle  Handle to the previous application instance.
 * @param[in] argv                  Command line arguments.
 * @param[in] show_cmd              Specifies how the window is to be shown.
 * @param[in] settings              Pointer to the settings structure.
 *
 * @return Pointer to the created game instance.
 */
Game* Game__constructor(Game* ECX, HINSTANCE instance_handle,
                        HINSTANCE prev_instance_handle, const char** argv,
                        int show_cmd, Settings* settings);

/**
 * @brief Destroys the game instance.
 *
 * @param[in] ECX  Game instance pointer.
 * @param[in] free Whether to free the memory allocated for Entity.
 *
 * @return Pointer to the destroyed game instance.
 */
Game* Game__destroy_405110(Game* ECX, char free);

/**
 * @brief Gets the video object by its index.
 *
 * @param[in] ECX      Game instance pointer.
 *
 * @return 0 or 1. As far as I got, 0 is if there are no messages to be
 *         processed, 1 otherwise.
 */
int Game__tick(Game* ECX);

/**
 * @brief Returns Vid object pointer by its index.
 *
 * @param[in] ECX      Game instance pointer.
 * @param[in] vid_idx  Index of the video object.
 *
 * @return Pointer to the Vid object (if exists).
 */
Vid* Game__get_vid_by_idx(Game* ECX, enVid vid_idx);

/**
 * @brief Creates an entity.
 *
 * @param[in] ECX      Game instance pointer.
 * @param[in] vid      Vid object pointer.
 * @param[in] x        x coordinate.
 * @param[in] y        y coordinate.
 * @param[in] z        z coordinate.
 * @param[in] a6       Unknown.
 * @param[in] parent   Parent entity pointer.
 *
 * @return Pointer to the created entity if successful
 * @return NULL otherwise.
 */
Entity* Game__create_entity(Game* ECX, Vid* vid, float x, float y, float z,
                            int a6, void* parent);

/**
 * @brief Initializes the main game class.
 *
 * @param[in] ECX                   Game instance pointer.
 * @param[in] instance_handle       Handle to the current application instance.
 * @param[in] prev_instance_handle  Handle to the previous application instance.
 * @param[in] argv                  Command line arguments.
 * @param[in] show_cmd              Specifies how the window is to be shown.
 * @param[in] settings              Pointer to the settings structure.
 *
 * @return Pointer to the initialized game instance.
 */
Game* Game__init(Game* ECX, HINSTANCE instance_handle,
                 HINSTANCE prev_instance_handle, const char** argv,
                 int show_cmd, Settings* settings);

/**
 * @brief Destroys the game instance.
 *
 * @param[in] ECX  Game instance pointer.
 * @param[in] free Whether to free the memory allocated for Entity.
 *
 * @return Pointer to the destroyed game instance.
 */
Game* Game__destroy_40B140(Game* ECX, char free);

/**
 * @brief Processes window messages.
 *
 * @param[in] ECX      Game instance pointer.
 * @param[in] hWnd     Handle to the window.
 * @param[in] uMsg     Message identifier.
 * @param[in] wParam   Message parameter.
 * @param[in] lParam   Message parameter.
 *
 * @return // TODO: Reverse engineer return value.
 */
int Game__wnd_proc(Game* ECX, HWND hWnd, uint32_t uMsg, uint32_t wParam,
                   uint32_t lParam);

/**
 * @brief Loads a map from a file.
 *
 * @param[in] ECX            Game instance pointer.
 * @param[in] map_file_path  Map file path (relative to the game directory).
 */
void Game__load_map(Game* ECX, const char* map_file_path);

/**
 * @brief Gets the army's player entity.
 *
 * @param[in] ECX        Game instance pointer.
 * @param[in] army_idx   Army index.
 *
 * @return Pointer to the army flagman entity.
 */
EntPlayer* Game__get_army_player(Game* ECX, int army_idx);

// /**
//  * @brief Gets an index of flagman army.
//  *
//  * There are 4 armies in the game. This function returns an index of the army
//  * the player is controlling. Almost always it is 0.
//  *
//  * @param[in] ECX  Game instance pointer.
//  *
//  * @return Index of the flagman army.
//  */
// uint32_t Game__get_flagman_army_0x24(Game* ECX);

/**
 * @brief Gets an entity at the specified screen coordinates.
 *
 * @param[in] ECX      Game instance pointer.
 * @param[in] a_type   Type of the entity to get.
 * @param[in] scr_x    x coordinate on the screen.
 * @param[in] scr_y    y coordinate on the screen.
 *
 * @return Pointer to the entity if found.
 * @return NULL otherwise.
 */
Entity* Game__get_entity_at_screen_pos(Game* ECX, enGetSpriteType a_type,
                                       int scr_x, int scr_y);

/**
 * @brief Returns 1 if vid with specified index is loaded.
 *
 * @param[in] ECX      Game instance pointer.
 * @param[in] vid_idx  Index of vid to check.
 *
 * @return 1 if vid is loaded.
 * @return 0 otherwise (or if vid_idx is out of range).
 */
int Game__is_vid_idx_exists(Game* ECX, enVid vid_idx);

/**
 * @brief Gets an entity at the specified world coordinates.
 *
 * @param[in] ECX      Game instance pointer.
 * @param[in] a_type   Type of the entity to get.
 * @param[in] x        x coordinate.
 * @param[in] y        y coordinate.
 * @param[in] z        z coordinate.
 * @param[in] prev     Unknown.
 *
 * @return Pointer to the entity if found.
 * @return NULL otherwise.
 */
Entity* Game__get_entity_at_pos(Game* ECX, enGetSpriteType a_type, float x,
                                float y, float z, int prev);

/**
 * @brief Returns ground z coordinate at the specified world coordinates.
 *
 * @param[in] ECX      Game instance pointer.
 * @param[in] x        x coordinate.
 * @param[in] y        y coordinate.
 *
 * @return Ground z coordinate.
 */
double Game__get_ground_z(Game* ECX, float x, float y);

/**
 * @brief Returns pointer to army object by its index.
 *
 * There are 4 aries in the game, so allowed indices are 0-3.
 *
 * @param[in] ECX        Game instance pointer.
 * @param[in] army_idx   Army index.
 *
 * @return Pointer to the army object.
 */
Army* Game__get_army_by_idx(Game* ECX, int army_idx);

#endif /* TYPES_GAME_H */
