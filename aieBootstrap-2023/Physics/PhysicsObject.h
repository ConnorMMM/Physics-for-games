#pragma once
#include <glm/glm.hpp>

enum ShapeType {
	PLANE = 0,
	CIRCLE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) { m_elasticity = 1; m_color = glm::vec4(1, 1, 1, 1); }
	PhysicsObject(ShapeType a_shapeID, glm::vec4 a_color) : m_shapeID(a_shapeID), m_color(a_color) { m_elasticity = 1; }
	PhysicsObject(ShapeType a_shapeID, float elasticity, glm::vec4 a_color) : m_shapeID(a_shapeID), m_color(a_color), m_elasticity(elasticity) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw(float alpha) = 0;
	virtual void ResetPosition() {};

	// Getter
	ShapeType GetShapeID() { return m_shapeID; }
	float GetElasticity() { return m_elasticity; }
	virtual float GetKineticEnergy() = 0;
	virtual float GetEnergy() = 0;

	// Setter
	void SetColor(glm::vec4 color) { m_color = color; }
	void SetElasticity(float elasticity) { m_elasticity = elasticity; }

protected:
	ShapeType m_shapeID;

	float m_elasticity;
	glm::vec4 m_color;

};

