#pragma once

#include <stdbool.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/native_window.h>
#include "../app/game/snake.h"


typedef struct s_renderer {
    EGLDisplay pt_display;
    EGLSurface pt_surface;
    EGLContext pt_context;
    bool is_ready;

    GLuint program;
    GLint color_loc;  // uniform location for u_color
    GLuint vao;   // describes vertex data format
    GLuint vbo;   // uploads vertices to GPU
    GLuint ebo;   // avoids duplicating vertices for indexed drawing
} s_renderer;

typedef struct s_rgb{
    float r;
    float g;
    float b;
}s_rgb ;

// game colors
#define COLOR_SNAKE_BODY  (s_rgb){0.3f, 0.8f, 0.2f}
#define COLOR_SNAKE_HEAD  (s_rgb){0.2f, 0.9f, 0.2f}
#define COLOR_CHERRY      (s_rgb){0.85f, 0.12f, 0.15f}

#ifdef __cplusplus
extern "C" {
#endif

void renderer_init(ANativeWindow *pt_window);
void renderer_destroy(void);
void renderer_draw_cell(s_renderer *r,float x, float y,float size,s_rgb rgb);
void renderer_draw_game(s_game *game);

#ifdef __cplusplus
}
#endif
