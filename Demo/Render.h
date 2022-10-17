#ifndef RENDER_H
#define RENDER_h

#include "Object.h"
#include "glfw3.h"
#include "PhysicsEngine.h"

class Renderer 
{
public:
	Renderer(std::vector<RendererObject*> objects, PhysicsEngine* physics) {
		m_objects = objects;
		m_physicsp = physics;
		registerObjectsWithPhysicsSubsystem();
	}
	~Renderer()
	{
		m_objects.clear();
	}
	void render(GLFWwindow* window, Shader* shader, Camera* camera);
	void registerObjectsWithPhysicsSubsystem();
	void update();
private:
	std::vector<RendererObject*> m_objects;
	PhysicsEngine* m_physicsp;
};

#endif