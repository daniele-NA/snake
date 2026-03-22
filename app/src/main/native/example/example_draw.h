#pragma once

#include <GLES3/gl3.h>
#include "../renderer/renderer.h"

#ifdef __cplusplus
extern "C" {
#endif

// example draw functions (points, square, kotlin logo)
void draw_center_point(s_renderer *renderer);
void draw_multiple_points(s_renderer *renderer, float vertices[], int vertex_count);
void draw_square(s_renderer *renderer, float x, float y, float size);
void draw_kotlin_logo(s_renderer *renderer, float x, float y, float width, float height);

#ifdef __cplusplus
}
#endif
