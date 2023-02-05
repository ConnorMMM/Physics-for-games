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
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);
	
	void Rigidbody::CalculateSmoothedPosition(float alpha);

	virtual float GetKineticEnergy();
	float GetPotentialEnergy();
	virtual float GetEnergy() { return GetKineticEnergy() + GetPotentialEnergy(); }

	// Getters
	glm::vec2 GetPosition() { return m_position; }
	glm::vec2 GetLastPosition() { return m_lastPosition; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }

	float GetOrientation() { return m_orientation; }
	float GetLastOrientation() { return m_lastOrientation; }
	float GetAngularVelocity() { return m_angularVelocity; }
	float GetMoment() { return m_moment; }

	glm::vec2 GetSmoothedPosition() { return m_smoothedPosition; }
	glm::vec2 GetSmoothedLocalX() { return m_smoothedLocalX; }
	glm::vec2 GetSmoothedLocalY() { return m_smoothedLocalY; }

	// Setters
	void SetPosition(glm::vec2 position) { m_position = position; }
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetMass(float mass) { m_mass = mass; }

	void SetOrientation(float orientation) { m_orientation = orientation; }
	void SetAngularVelocity(float angularVelocity) { m_angularVelocity = angularVelocity; }
	void SetMoment(float moment) { m_moment = moment; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_lastPosition;
	glm::vec2 m_velocity;
	float m_mass;

	float m_orientation; // 2D so we only need a single float to represent our orientation
	float m_lastOrientation;
	float m_angularVelocity;
	float m_moment;


	glm::vec2 m_smoothedPosition;
	glm::vec2 m_smoothedLocalX;
	glm::vec2 m_smoothedLocalY;

};