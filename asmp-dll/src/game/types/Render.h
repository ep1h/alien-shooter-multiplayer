#ifndef TYPES_RENDER_H
#define TYPES_RENDER_H

#include "common.h"
#include "enums.h"

#ifndef IDirect3D8
typedef void* IDirect3D8;
#endif /* IDirect3D8 */
#ifndef IDirect3DDevice8
typedef void* IDirect3DDevice8;
#endif /* IDirect3DDevice8 */

typedef struct Render
{
    uint32_t unk_00[144];
    float width;
    float height;
    uint32_t unk_248;
    uint32_t unk_24C;
    uint32_t unk_250;
    float unk_254;
    float unk_258;
    float unk_25C;
    float unk_260;
    uint32_t unk_264;
    uint32_t unk_268;
    uint32_t unk_26C;
    uint32_t unk_270[733];
    enEnvironment environment;
    uint32_t wind_direction;
    float wind_force;
    void* movie_player;
    uint32_t unk_DF4;
    uint32_t unk_DF8;
    uint32_t unk_DFC;
    uint32_t unk_E00;
    uint32_t unk_E04;
    uint32_t unk_E08;
    uint32_t unk_E0C;
    uint32_t unk_E10;
    uint32_t field_E14;
    IDirect3D8* IDirect3D8;
    IDirect3DDevice8* IDirect3DDevice8;
    uint32_t field_E20;
    uint32_t field_E24;
    uint32_t field_E28;
    uint32_t field_E2C;
} Render;

typedef int(__thiscall* Render__set_environment_t)(Render* this,
                                                   enEnvironment env);
typedef float(__thiscall* Render__draw_colored_rect_t)(Render* this, float x1,
                                                       float y1, float x2,
                                                       float y2,
                                                       uint32_t color_ARGB);

#endif /* TYPES_RENDER_H */
