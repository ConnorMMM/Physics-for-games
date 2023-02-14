#pragma once
#include "Circle.h"

#include <string>

class BilliardBall : public Circle
{
public:
	BilliardBall(glm::vec2 position, glm::vec2 velocity, float mass,
		float radius, float elasticity, int state);
	~BilliardBall();

	virtual void Draw(float alpha);

	// Getter
	int GetState() { return m_state; }

protected:
	int m_state;

};

