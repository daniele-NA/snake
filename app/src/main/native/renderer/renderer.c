#include <assert.h>
#include "renderer.h"
#include "draw.h"
#include "../utils/utils.h"

static s_renderer g_renderer = {
    .pt_display = EGL_NO_DISPLAY,
    .pt_surface = EGL_NO_SURFACE,
    .pt_context = EGL_NO_CONTEXT,
    .is_ready = false,
};

void renderer_init(ANativeWindow *pt_window) {
    // connect to GPU
    g_renderer.pt_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(g_renderer.pt_display, NULL, NULL);

    // surface config: RGBA 8-bit, OpenGL ES 3
    const EGLint config_attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_ALPHA_SIZE,      8,
        EGL_NONE,
    };

    EGLConfig config;
    EGLint num_configs;
    eglChooseConfig(g_renderer.pt_display, config_attribs, &config, 1, &num_configs);

    // OpenGL ES 3.0 context
    const EGLint context_attribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 0,
        EGL_NONE,
    };

    g_renderer.pt_context = eglCreateContext(
        g_renderer.pt_display, config, EGL_NO_CONTEXT, context_attribs);

    if (g_renderer.pt_context == EGL_NO_CONTEXT) {
        LOG_E("EGL context creation failed");
        return;
    }

    // surface linked to the Android window
    g_renderer.pt_surface = eglCreateWindowSurface(
        g_renderer.pt_display, config, pt_window, NULL);

    // activate context and surface
    eglMakeCurrent(
        g_renderer.pt_display, g_renderer.pt_surface,
        g_renderer.pt_surface, g_renderer.pt_context);

    float *colors = opengl_color(0, 0, 0, 255);
    glClearColor(colors[0], colors[1], colors[2], colors[3]);

    // setup shaders, VAO, VBO (once)
    draw_setup(&g_renderer);

    g_renderer.is_ready = true;
    LOG_I("OpenGL ES %s", glGetString(GL_VENDOR));
}

void renderer_destroy(void) {
    if (g_renderer.pt_display == EGL_NO_DISPLAY) return;

    eglMakeCurrent(g_renderer.pt_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    if (g_renderer.pt_surface != EGL_NO_SURFACE)
        eglDestroySurface(g_renderer.pt_display, g_renderer.pt_surface);

    if (g_renderer.pt_context != EGL_NO_CONTEXT)
        eglDestroyContext(g_renderer.pt_display, g_renderer.pt_context);

    eglTerminate(g_renderer.pt_display);

    g_renderer.pt_display = EGL_NO_DISPLAY;
    g_renderer.pt_surface = EGL_NO_SURFACE;
    g_renderer.pt_context = EGL_NO_CONTEXT;
    g_renderer.is_ready = false;
}

void renderer_draw(void) {
    if (!g_renderer.is_ready) return;

    // clear screen with background color
    glClear(GL_COLOR_BUFFER_BIT);

    draw_kotlin_logo(&g_renderer, 0.0f, 0.0f, 1.0f, 0.5f);

    // swap back buffer to screen
    eglSwapBuffers(g_renderer.pt_display, g_renderer.pt_surface);
}
