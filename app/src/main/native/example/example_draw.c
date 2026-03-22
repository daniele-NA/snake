/*
 * example_draw.c - OpenGL ES 3.0 drawing primitives (examples)
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

#include "example_draw.h"

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
