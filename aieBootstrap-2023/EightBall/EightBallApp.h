#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include <glm/vec2.hpp>
#include <vector>

#define PI 3.14159265359

class PhysicsScene;
class PhysicsObject;

class EightBallApp : public aie::Application {
public:

	EightBallApp();
	virtual ~EightBallApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void BoardStartUp();

	float DegreeToRadian(float degree);
	glm::vec2 ScreenToWorld(glm::vec2 screenPos);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	std::vector<PhysicsObject*> m_physicsObjects;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	std::vector<PhysicsObject*> m_ballsInPockets;

	int m_score;
};