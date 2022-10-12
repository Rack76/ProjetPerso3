#ifndef RENDER_H
#define RENDER_h

#include "Object.h"
#include "glfw3.h"

class Renderer 
{
public:
	Renderer(std::vector<RendererObject*> objects) {
		m_objects = objects;
	}
	void render(GLFWwindow* window, Shader* shader, Camera* camera);
private:
	std::vector<RendererObject*> m_objects;
};

#endif