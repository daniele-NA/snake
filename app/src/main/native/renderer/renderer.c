#include <assert.h>
#include "renderer.h"
#include "../utils/utils.h"

static void renderer_setup_gl(s_renderer *renderer) {
    const char *vertex_src =
            "#version 300 es\n"
            "layout(location = 0) in vec2 pos;\n"
            "void main(){\n"
            "  gl_Position = vec4(pos, 0.0, 1.0);\n"
            "  gl_PointSize = 20.0;\n"
            "}";

    const char *fragment_src =
            "#version 300 es\n"
            "precision mediump float;\n"
            "out vec4 frag_color;\n"
            "void main(){\n"
            "  frag_color = vec4(0.694, 0.145, 0.918, 1.0);\n"
            "}";

    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_src, NULL);
    glCompileShader(vertex_shader_id);

    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_src, NULL);
    glCompileShader(fragment_shader_id);

    renderer->program = glCreateProgram();
    glAttachShader(renderer->program, vertex_shader_id);
    glAttachShader(renderer->program, fragment_shader_id);
    glLinkProgram(renderer->program);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    glGenBuffers(1, &renderer->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

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
    renderer_setup_gl(&g_renderer);

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

    // draw_kotlin_logo(&g_renderer, 0.0f, 0.0f, 1.0f, 0.5f);

    // swap back buffer to screen
    eglSwapBuffers(g_renderer.pt_display, g_renderer.pt_surface);
}
