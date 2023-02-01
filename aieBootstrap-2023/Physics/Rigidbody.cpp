#include "Rigidbody.h"
#include "PhysicsScene.h"

#include <iostream>

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) : 
	PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep);
}

void Rigidbody::ApplyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::ApplyForceToActor(Rigidbody* otherActor, glm::vec2 force)
{
	otherActor->ApplyForce(force);
	ApplyForce(-force);
}

void Rigidbody::ResolveCollision(Rigidbody* actor2)
{
	glm::vec2 normal = glm::normalize(actor2->GetPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;

	// if the objects are already moving apart, we don't need to do anything
	if (glm::dot(normal, relativeVelocity) >= 0)
		return;

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / 
		((1 / m_mass) + (1 / actor2->GetMass()));

	glm::vec2 force = normal * j;

	float kePre = GetKineticEnergy() + actor2->GetKineticEnergy();

	ApplyForceToActor(actor2, force);

	float kePost = GetKineticEnergy() + actor2->GetKineticEnergy();

	float deltaKE = kePost - kePre;
	if (deltaKE > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}

float Rigidbody::GetKineticEnergy()
{
	float temp1 = .5f * m_mass * ((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));
	float temp2 = .5f * m_mass * glm::dot(m_velocity, m_velocity);
	return temp1;
}

float Rigidbody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}