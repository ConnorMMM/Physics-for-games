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

	void HoldingCue();
	void ReleaseCue();

	void AddToCueOffset(float offset);
	void CueDirFromMousePos(glm::vec2 mousePos);

	// Getter
	bool GetHoldingCue() { return m_holdingCue; }

	// Setter
	void SetCueDir(glm::vec2 cueDir) { m_cueDir = cueDir; }
	void SetCueOffset(float cueOffset) { m_cueOffset = cueOffset; }

protected:
	glm::vec2 m_cueDir;
	float m_cueOffset;

	bool m_holdingCue;
	float m_cueDistance;
};

