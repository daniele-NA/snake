#pragma once

/*
 * Example renderer_draw — draws nothing, just clears screen.
 * Was used before renderer_draw_game() replaced it.
 */

#include <stdbool.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#ifdef __cplusplus
extern "C" {
#endif

void example_renderer_draw(void);

#ifdef __cplusplus
}
#endif
