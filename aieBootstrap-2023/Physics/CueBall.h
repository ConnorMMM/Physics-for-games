#pragma once
#include "Rigidbody.h"

class CueBall : public Rigidbody
{
public:
	CueBall(glm::vec2 position, glm::vec2 velocity, float mass,
		float radius, float elasticity);
	~CueBall();

	virtual void Draw(float alpha);

	// Getter
	float GetRadius() { return m_radius; }

	// Setter
	void SetRadius(float radius) { m_radius = radius; }

protected:
	float m_radius;

};

