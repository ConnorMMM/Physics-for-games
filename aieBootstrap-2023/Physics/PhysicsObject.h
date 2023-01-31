#pragma once
#include <glm/glm.hpp>

enum ShapeType {
	PLANE = 0,
	CIRCLE
	//BOX
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw(float alpha) = 0;
	virtual void ResetPosition() {};

	// Getter
	ShapeType GetShapeID() { return m_shapeID; }

	// Setter
	void SetColor(glm::vec4 color) { m_color = color; }

protected:
	ShapeType m_shapeID;
	glm::vec4 m_color;

};

