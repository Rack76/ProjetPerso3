#include "PhysicsObject.h"

void PhysicsObject::move(float dt)
{
	solver.run(dt, m_position, linearVelocity, m_orientation, angularVelocity, netForce, netTorque, m_newPosition, m_newOrientation);
}