/*
 * draw.c - OpenGL ES 3.0 drawing primitives
 *
 * == NDC (Normalized Device Coordinates) ==
 *
 *       +1.0
 *         |
 *         |
 * -1.0 ---+--- +1.0
 *         |
 *         |
 *       -1.0
 *
 * == How a square is built from 2 triangles ==
 *
 *   Vertices:
 *     v0 -------- v1
 *     |            |
 *     |            |
 *     v3 -------- v2
 *
 *   Triangle 1: indices 0,1,3 -> v0, v1, v3
 *     v0 -------- v1
 *     |          /
 *     |        /
 *     |      /
 *     |    /
 *     |  /
 *     v3
 *
 *   Triangle 2: indices 1,2,3 -> v1, v2, v3
 *                v1
 *               / |
 *             /   |
 *           /     |
 *         /       |
 *       /         |
 *     v3 -------- v2
 *
 *   Combined (filled square):
 *     v0 -------- v1
 *     |         / |
 *     |       /   |
 *     |     /     |
 *     |   /       |
 *     | /         |
 *     v3 -------- v2
 *
 * == Why use an EBO (Element Buffer Object)? ==
 *
 *   Without indices: 6 vertices needed (3 per triangle, v1 and v3 duplicated)
 *   With indices:    4 vertices + 6 indices (v1 and v3 reused)
 *   On complex meshes with thousands of faces, the savings are significant.
 *
 * == Kotlin "K" logo shape ==
 *
 *   Uses indices 0,1,3 and 0,2,3 instead of the square's 0,1,3 and 1,2,3:
 *
 *   Triangle 1: 0,1,3 -> v0, v1, v3 (same as square)
 *     v0 -------- v1
 *     |          /
 *     |        /
 *     |      /
 *     |    /
 *     |  /
 *     v3
 *
 *   Triangle 2: 0,2,3 -> v0, v2, v3
 *     v0
 *     | \
 *     |   \
 *     |     \
 *     |       \
 *     |         \
 *     v3 -------- v2
 *
 *   Combined (K-like shape):
 *     v0 -------- v1
 *     | \        /
 *     |   \    /
 *     |    \ /
 *     |    / \
 *     |  /    \
 *     v3 -------- v2
 */

#include "draw.h"

void draw_setup(s_renderer *renderer) {
    // vertex shader: reads position from VBO (version must match AndroidManifest)
    const char *vertex_src =
            "#version 300 es\n"
            "layout(location = 0) in vec2 pos;\n"
            "void main(){\n"
            "  gl_Position = vec4(pos, 0.0, 1.0);\n"
            "  gl_PointSize = 20.0;\n"
            "}";

    // fragment shader: Kotlin purple (177, 37, 234)
    const char *fragment_src =
            "#version 300 es\n"
            "precision mediump float;\n"
            "out vec4 frag_color;\n"
            "void main(){\n"
            "  frag_color = vec4(0.694, 0.145, 0.918, 1.0);\n"
            "}";

    // compile vertex shader
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_src, NULL);
    glCompileShader(vertex_shader_id);

    // compile fragment shader
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_src, NULL);
    glCompileShader(fragment_shader_id);

    // link into a program
    renderer->program = glCreateProgram();
    glAttachShader(renderer->program, vertex_shader_id);
    glAttachShader(renderer->program, fragment_shader_id);
    glLinkProgram(renderer->program);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // create VAO and VBO
    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    // EBO for indexed triangle drawing
    glGenBuffers(1, &renderer->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);

    // 2 floats per vertex at location 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void draw_center_point(s_renderer *renderer) {
    float vertex[] = {0.0f, 0.0f};

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glUseProgram(renderer->program);
    glBindVertexArray(renderer->vao);
    glDrawArrays(GL_POINTS, 0, 1);
}

void draw_multiple_points(s_renderer *renderer, float vertices[], int vertex_count) {
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

    glUseProgram(renderer->program);
    glBindVertexArray(renderer->vao);
    glDrawArrays(GL_POINTS, 0, vertex_count);
}

void draw_square(s_renderer *renderer, float x, float y, float size) {
    float half = size / 2.0f;

    float vertices[] = {
            x - half, y + half,   // v0: top-left
            x + half, y + half,   // v1: top-right
            x + half, y - half,   // v2: bottom-right
            x - half, y - half,   // v3: bottom-left
    };

    GLuint indices[] = {
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
    };

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(renderer->program);
    glBindVertexArray(renderer->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_kotlin_logo(s_renderer *renderer, float x, float y, float width, float height) {
    float hw = width / 2.0f;
    float hh = height / 2.0f;

    float vertices[] = {
            x - hw, y + hh,   // v0: top-left
            x + hw, y + hh,   // v1: top-right
            x + hw, y - hh,   // v2: bottom-right
            x - hw, y - hh,   // v3: bottom-left
    };

    GLuint indices[] = {
            0, 1, 3,  // first triangle (upper-left)
            0, 2, 3   // second triangle (lower-left) -> K shape
    };

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(renderer->program);
    glBindVertexArray(renderer->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
