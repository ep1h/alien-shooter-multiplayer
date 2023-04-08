/**
 * @file Render.h
 * @brief Provides an API for interacting with the game's render system.
 *
 */
#ifndef TYPES_RENDER_H
#define TYPES_RENDER_H

#include <stdint.h>
#include "enums.h"

#ifndef IDirect3D8
typedef void* IDirect3D8;
#endif /* IDirect3D8 */
#ifndef IDirect3DDevice8
typedef void* IDirect3DDevice8;
#endif /* IDirect3DDevice8 */

/**
 * @brief Structure that represents the game's render system.
 */
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

/**
 * @brief Draws a colored rectangle on the screen.
 *
 * @param[in] ECX Render instance pointer.
 * @param[in] x1  x coordinate of the first point.
 * @param[in] y1  y coordinate of the first point.
 * @param[in] x2  x coordinate of the second point.
 * @param[in] y2  y coordinate of the second point.
 *
 * @return unk
 */
float Render__draw_colored_rect(Render* ECX, float x1, float y1, float x2,
                                float y2, uint32_t color_ARGB);

#endif /* TYPES_RENDER_H */
