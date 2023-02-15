#pragma once

#include "Circle.h"

class CueBall : public Circle
{
public:
	CueBall(glm::vec2 position, glm::vec2 velocity, float mass,
		float radius, float elasticity);
	~CueBall();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Draw(float alpha);

	// Getter
	bool IsHolding() { return m_holding; }

	// Setter
	void SetHolding(bool state);
	void SetMousePos(glm::vec2 mousePos);
	void SetDrawCue(bool state) { m_drawCue = state; }

protected:
	bool m_holding;

	float m_cuePower;
	glm::vec2 m_cueDir;

	float m_currentMouseDis;
	float m_heldMouseDis;

	bool m_drawCue;
};