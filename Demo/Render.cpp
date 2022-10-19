#include "glew.h"
#include "Shader.h"
#include "glfw3.h"
#include "Object.h"
#include "Render.h"
#include <iostream>

void Renderer::registerObjectsWithPhysicsSubsystem()
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        m_objects[i]->physicsObjectHandle = m_physicsp->registerObject(m_objects[i]->faceInfoList, m_objects[i]->edgeInfoList, m_objects[i]->normals, m_objects[i]->vertices,
                                                               m_objects[i]->m_mass, m_objects[i]->m_cof, m_objects[i]->m_cor, m_objects[i]->position, m_objects[i]->orientation);
    }
}

void Renderer::update()
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        m_physicsp->mapObject(m_objects[i]->physicsObjectHandle, m_objects[i]->position, m_objects[i]->orientation);
        m_objects[i]->update();
    }
}

void Renderer::render(GLFWwindow* window, Shader* shader, Camera* camera)
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        update();
        shader->doShaderPlumbing(camera, m_objects[i]);
        m_objects[i]->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, m_objects[i]->vertexPositionsSize);
        glfwSwapBuffers(window);
    }
}