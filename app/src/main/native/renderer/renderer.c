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
    // connessione al GPU
    g_renderer.pt_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(g_renderer.pt_display, NULL, NULL);

    // configurazione surface: RGBA 8-bit, OpenGL ES 3
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

    // contesto OpenGL ES 3.0
    const EGLint context_attribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 0,
        EGL_NONE,
    };

    g_renderer.pt_context = eglCreateContext(
        g_renderer.pt_display, config, EGL_NO_CONTEXT, context_attribs);

    if (g_renderer.pt_context == EGL_NO_CONTEXT) {
        LOG_E("Creazione contesto EGL fallita");
        return;
    }

    // surface collegata alla finestra Android
    g_renderer.pt_surface = eglCreateWindowSurface(
        g_renderer.pt_display, config, pt_window, NULL);

    // attiva contesto e surface
    eglMakeCurrent(
        g_renderer.pt_display, g_renderer.pt_surface,
        g_renderer.pt_surface, g_renderer.pt_context);

    // colore di sfondo (blu)
    glClearColor(0.1f, 0.1f, 1.0f, 1.0f);

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

    // pulisce lo schermo col colore di sfondo
    glClear(GL_COLOR_BUFFER_BIT);

    // == DISEGNA QUI == //
    draw_multiple_points();

    // scambia il buffer nascosto con quello visibile
    eglSwapBuffers(g_renderer.pt_display, g_renderer.pt_surface);
}
