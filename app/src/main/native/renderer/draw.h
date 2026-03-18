#pragma once

#include <GLES3/gl3.h>
#include "renderer.h"

#ifdef __cplusplus
extern "C" {
#endif
    // Setuppiamo solo una volta il tutto
void draw_setup(s_renderer * renderer);

    // Tutti i metodi si prendono il renderer
void draw_center_point(s_renderer * renderer);
void draw_multiple_points(s_renderer * renderer ,float vertices[], int vertex_count);
void draw_square(s_renderer * renderer, float x, float y , float size);
#ifdef __cplusplus
}
#endif
