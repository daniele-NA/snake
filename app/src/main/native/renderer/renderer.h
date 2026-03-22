#pragma once

#include <stdbool.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/native_window.h>

typedef struct s_renderer s_renderer;

struct s_renderer {
    EGLDisplay pt_display;
    EGLSurface pt_surface;
    EGLContext pt_context;
    bool is_ready;

    GLuint program;
    GLuint vao;   // describes vertex data format
    GLuint vbo;   // uploads vertices to GPU
    GLuint ebo;   // avoids duplicating vertices for indexed drawing
};

#ifdef __cplusplus
extern "C" {
#endif

void renderer_init(ANativeWindow *pt_window);
void renderer_destroy(void);
void renderer_draw(void);

#ifdef __cplusplus
}
#endif
