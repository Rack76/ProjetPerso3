#include "glew.h"
#include "glfw3.h"
#include <iostream>

void setUpRenderingPipelineData()
{
    static const GLfloat vertices[9] =
    {
        -0.90f, -0.90f, 1.0f,
        0.0f, 0.90f, 1.0f,
        0.9f,  -0.90f, 1.0f
    };

    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void render(GLFWwindow* window)
{
    const static GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, red);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
}