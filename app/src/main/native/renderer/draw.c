#include "draw.h"

void draw_setup(s_renderer *renderer) {
    // vertex shader: legge posizione dal VBO
    const char *vertex_src =
            "#version 300 es\n"
            "layout(location = 0) in vec2 pos;\n"
            "void main(){\n"
            "  gl_Position = vec4(pos, 0.0, 1.0);\n"
            "  gl_PointSize = 20.0;\n"
            "}";

    // fragment shader: colore verde
    const char *fragment_src =
            "#version 300 es\n"
            "precision mediump float;\n"
            "out vec4 frag_color;\n"
            "void main(){\n"
            "  frag_color = vec4(0.0, 1.0, 0.0, 1.0);\n"
            "}";

    // compila vertex shader
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_src, NULL);
    glCompileShader(vertex_shader_id);

    // compila fragment shader
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_src, NULL);
    glCompileShader(fragment_shader_id);

    // linka in un programma e salva nel renderer
    renderer->program = glCreateProgram();
    glAttachShader(renderer->program, vertex_shader_id);
    glAttachShader(renderer->program, fragment_shader_id);
    glLinkProgram(renderer->program);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // crea VAO e VBO, salva nel renderer
    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    // utilizzato per i triangoli
    glGenBuffers(1, &renderer->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);

    // 2 float per vertice, alla location 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void draw_center_point(s_renderer *renderer) {
    // un punto al centro (0, 0)
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
    //  1   , 1,   20
    // half = 10
    //
    float vertices[] = {
            x - half, y + half,   // v0: in alto a sinistra
            x + half, y + half,   // v1: in alto a destra
            x + half, y - half,   // v2: in basso a destra
            x - half, y - half,   // v3: in basso a sinistra
    };

    GLuint indices[] = {
            0,1,3,   // primo triangolo
            1,2,3   // secondo triangolo
    };

    glBindBuffer(GL_ARRAY_BUFFER , renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices , GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices , GL_STATIC_DRAW);


}