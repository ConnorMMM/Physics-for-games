#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position,
		glm::vec2 velocity, float orientation, float mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force);
	void ApplyForcetoActor(Rigidbody* otherActor, glm::vec2 force);

	// Getters
	glm::vec2 GetPosition() { return m_position; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetOrientation() { return m_orientation; }

	// Setters
	void SetPosition(glm::vec2 position) { m_position = position; }
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetMass(float mass) { m_mass = mass; }
	void SetOrientation(float orientation) { m_orientation = orientation; }
	void SetColor(glm::vec4 color) { m_color = color; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; // 2D so we only need a single float to represent our orientation

	glm::vec4 m_color;

};
