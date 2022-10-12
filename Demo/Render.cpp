#include "glew.h"
#include "glfw3.h"
#include "Object.h"
#include <iostream>

void render(GLFWwindow* window, RendererObject* object)
{
    object->bind();
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(0);
    const static GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, red);
    glDrawArrays(GL_TRIANGLES, 0, object->vertexPositionsSize);
    glfwSwapBuffers(window);
}