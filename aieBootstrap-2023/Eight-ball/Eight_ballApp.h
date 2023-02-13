#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include <vector>
#include <glm/vec2.hpp>

class PhysicsScene;
class PhysicsObject;

#define PI 3.14159265359

#define LINEAR_DRAG 0.3f
#define ANGULAR_DRAG 0.3f

#define MIN_LINEAR_THRESHOLD 0.01f
#define MIN_ANGULAR_THRESHOLD 0.01f

class Eight_ballApp : public aie::Application {
public:

	Eight_ballApp();
	virtual ~Eight_ballApp();

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
};