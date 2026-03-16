#include "draw.h"

void draw_center_point(void) {
    // vertex shader: punto fisso al centro, 20px
    const char *vertex_src =
        "#version 300 es\n"
        "void main(){\n"
        "  gl_Position = vec4(0.0, 0.0, 0.0, 1.0);\n"
        "  gl_PointSize = 20.0;\n"
        "}";

    // fragment shader: colore verde, formato RGBA
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

    // linka in un programma
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader_id);
    glAttachShader(program, fragment_shader_id);
    glLinkProgram(program);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // disegna 1 punto
    glUseProgram(program);
    glDrawArrays(GL_POINTS, 0, 1);

    glDeleteProgram(program);
}

void draw_multiple_points(void) {
    // posizioni dei punti (x, y)
    float vertices[] = {
        0.4f, 0.4f,
        0.4f, 0.5f,
        0.4f, 0.6f,
    };

    // VAO: descrive il formato dei dati
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO: carica i vertici sul GPU
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 2 float per vertice, alla location 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // vertex shader: legge posizione dal VBO
    const char *vertex_src =
        "#version 300 es\n"
        "layout(location = 0) in vec2 pos;\n"
        "void main(){\n"
        "  gl_Position = vec4(pos, 0.0, 1.0);\n"
        "  gl_PointSize = 70.0;\n"
        "}";

    // fragment shader: colore ciano
    const char *fragment_src =
        "#version 300 es\n"
        "precision mediump float;\n"
        "out vec4 frag_color;\n"
        "void main(){\n"
        "  frag_color = vec4(0.0, 1.0, 1.0, 1.0);\n"
        "}";

    // compila vertex
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_src, NULL);
    glCompileShader(vertex_shader_id);

    // compila fragment
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_src, NULL);
    glCompileShader(fragment_shader_id);

    // linka in un programma
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader_id);
    glAttachShader(program, fragment_shader_id);
    glLinkProgram(program);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // disegna i punti
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, sizeof(vertices) / sizeof(vertices[0]) / 2);

    // pulizia risorse GPU
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
