#include "AdaptiveTimeStepSolver.h"

void AdaptiveTimeStepSolver::run(float dt, glm::vec4& position, glm::vec4& linearVelocity
	, glm::mat4& orientation, glm::mat4& angularVelocity
	, glm::vec4 netForce, glm::mat4 netTorque)
{
	m_position = position;
	m_linearVelocity = linearVelocity;
	m_orientation = orientation;
	m_angularVelocity = angularVelocity;
	m_netForce = netForce;
	m_netTorque = netTorque;
	bool a = true;

	resetDynamicState();

	for (int i = 0; i < substepCount; i++)
	{
		if (!a)
		{
			i = 0;
			a = true;
		}
		integrator.run(dt, newPosition, newLinearVelocity, newOrientation, newAngularVelocity, newNetForce, newNetTorque);
		float error = integrator.estimateError(dt, newPosition, newLinearVelocity, newOrientation, newAngularVelocity);
		if (error <= errorTolerance)
			counter++;
		else
		{
			substepCount *= 2;
			counter = 0;
			i = 0;
			a = false;
			resetDynamicState();
		}
		if (counter == acceptance)
		{
			if (substepCount > 1)
				substepCount /= 2;
			counter = 0;
		}
	}
	linearVelocity = newLinearVelocity;
	angularVelocity = newAngularVelocity;
}

void AdaptiveTimeStepSolver::resetDynamicState()
{
	newPosition = m_position;
	newLinearVelocity = m_linearVelocity;
	newOrientation = m_orientation;
	newAngularVelocity = m_angularVelocity;
	newNetForce = m_netForce;
	newNetTorque = m_netTorque;
}
