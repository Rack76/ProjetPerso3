#include "glew.h"
#include "Shader.h"
#include "glfw3.h"
#include "Object.h"
#include "Render.h"
#include <iostream>

void Renderer::render(GLFWwindow* window, Shader* shader, Camera* camera)
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        m_objects[i]->setOrientation(0.6, 0.0, 1.0, 0.0);
        m_objects[i]->update();
        shader->doShaderPlumbing(camera, m_objects[i]);
        m_objects[i]->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, m_objects[i]->vertexPositionsSize);
        glfwSwapBuffers(window);
    }
}