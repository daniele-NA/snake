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

    // We add all the OpenGl params to draw
    GLuint program;
    GLuint vao;       // descrive il formato dei dati
    GLuint vbo;     // carica i vertici sul GPU
    GLuint ebo;     // non duplica i vertici nel disegno dei triangoli
};

typedef struct s_renderer s_renderer;

#ifdef __cplusplus
extern "C" {
#endif

void renderer_init(ANativeWindow *pt_window);
void renderer_destroy(void);
void renderer_draw(void);

#ifdef __cplusplus
}
#endif
