#include "Integrator.h"

void Integrator::run(float dt, glm::vec4& position, glm::vec4& linearVelocity
	, glm::mat4& orientation, glm::mat4& angularVelocity
	, glm::vec4 netForce, glm::mat4 netTorque)
{

}

float Integrator::estimateError(float dt, glm::vec4 position, glm::vec4 linearVelocity
	, glm::mat4 orientation, glm::mat4 angularVelocity)
{
	return 0.1;
}