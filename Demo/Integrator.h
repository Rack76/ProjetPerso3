#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Integrator
{
public:
	void run(float dt, glm::vec4& position, glm::vec4& linearVelocity
		, glm::mat4& orientation, glm::mat4& angularVelocity
		, glm::vec4 netForce, glm::mat4 netTorque);

	float estimateError(float dt, glm::vec4 position, glm::vec4 linearVelocity
		, glm::mat4 orientation, glm::mat4 angularVelocity);
};

#endif