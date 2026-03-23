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
            "uniform vec4 u_color;\n"
            "void main(){\n"
            "  frag_color = u_color;\n"
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

    // link this pointer with shader
    (*renderer).color_loc = glGetUniformLocation(renderer->program, "u_color");

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
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
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

// draw a square
void renderer_draw_cell(s_renderer *r, float x, float y, float size, s_rgb rgb) {
    float half = size / 2.0f;
    float vertices[] = {
            x - half, y + half,   // top-left
            x + half, y + half,   // top-right
            x + half, y - half,   // bottom-right
            x - half, y - half,   // bottom-left
    };

    // see example_draw.c
    GLuint indices[] = {0, 1, 3, 1, 2, 3};

    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(r->program);
    glUniform4f(r->color_loc, rgb.r, rgb.g, rgb.b, 1.0f);
    glBindVertexArray(r->vao);
    glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(indices[0])), GL_UNSIGNED_INT, 0);
}

void renderer_draw_game(s_game *game) {
    if (!g_renderer.is_ready) return;


    glClear(GL_COLOR_BUFFER_BIT);

    float cell_size = 1.8f / GRID_SIZE;
    float draw_size = cell_size * 0.85f;
    float head_size = cell_size * 0.9f;

    // cherry position
    float cx = -0.9f + (game->cherry.x + 0.5f) * cell_size;
    float cy = 0.9f - (game->cherry.y + 0.5f) * cell_size;

    renderer_draw_cell(&g_renderer, cx, cy, draw_size, COLOR_CHERRY);

    // snake body (skip head)
    for (int i = 1; i < game->length; i++) {
        float bx = -0.9f + (game->body[i].x + 0.5f) * cell_size;
        float by = 0.9f - (game->body[i].y + 0.5f) * cell_size;
        renderer_draw_cell(&g_renderer, bx, by, draw_size, COLOR_SNAKE_BODY);
    }

    // snake head

    float hx = -0.9f + (game->body[0].x + 0.5f) * cell_size;
    float hy = 0.9f - (game->body[0].y + 0.5f) * cell_size;
    renderer_draw_cell(&g_renderer, hx, hy, head_size, COLOR_SNAKE_HEAD);



    eglSwapBuffers(g_renderer.pt_display, g_renderer.pt_surface);

}