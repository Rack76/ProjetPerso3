#ifndef ADAPTIVE_TIME_STEP_SOLVER_H
#define ADAPTIVE_TIME_STEP_SOLVER_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Integrator.h"

class AdaptiveTimeStepSolver
{
public:
	void run(float dt, glm::vec4 &position, glm::vec4 &linearVelocity
					   , glm::mat4 &orientation, glm::mat4 &angularVelocity
				       , glm::vec4 netForce, glm::mat4 netTorque);
private:
	void resetDynamicState();
	float errorTolerance = 0.1;
	int substepCount = 1;
	int counter = 0;
	int acceptance = 3;
	Integrator integrator;

	glm::vec4 newPosition;
	glm::vec4 m_position;
	glm::vec4 newLinearVelocity;
	glm::vec4 m_linearVelocity;
	glm::vec4 newNetForce;
	glm::vec4 m_netForce;
	glm::mat4 newOrientation;
	glm::mat4 m_orientation;
	glm::mat4 newAngularVelocity;
	glm::mat4 m_angularVelocity;
	glm::mat4 newNetTorque;
	glm::mat4 m_netTorque;
};

#endif


