#pragma once
#include <glm/glm.hpp>

enum ShapeType {
	PLANE = 0,
	CIRCLE,
	//BOX
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
	PhysicsObject(ShapeType a_shapeID, glm::vec4 a_color) : m_shapeID(a_shapeID), m_color(a_color) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw(float alpha) = 0;
	virtual void ResetPosition() {};

	// Getter
	ShapeType GetShapeID() { return m_shapeID; }
	virtual float GetKineticEnergy() = 0;
	virtual float GetEnergy() = 0;

	// Setter
	void SetColor(glm::vec4 color) { m_color = color; }

protected:
	ShapeType m_shapeID;
	glm::vec4 m_color;

};

