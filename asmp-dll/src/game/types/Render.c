/**
 * @file Render.c
 * @brief Implementation of the API for interacting with the game renderer.
 *
 */
#include "Render.h"
#include "game/addresses.h"
#include "game/types/common.h"

typedef int(CC_THISCALL* Render__set_environment_t)(Render* this,
                                                    enEnvironment env);
typedef float(CC_THISCALL* Render__draw_colored_rect_t)(Render* this, float x1,
                                                        float y1, float x2,
                                                        float y2,
                                                        uint32_t color_ARGB);

float Render__draw_colored_rect(Render* ECX, float x1, float y1, float x2,
                                float y2, uint32_t color_ARGB)
{
    Render__draw_colored_rect_t Render__draw_colored_rect =
        (Render__draw_colored_rect_t)FUNC_RENDER_DRAW_COLORED_RECT;
    return Render__draw_colored_rect(ECX, 0, x1, y1, x2, y2, color_ARGB);
}
