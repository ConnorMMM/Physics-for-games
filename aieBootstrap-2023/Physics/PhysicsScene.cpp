#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Circle.h"
#include "Plane.h"
#include "Demos.h"

#include <glm/glm.hpp>
#include <iostream>

glm::vec2 PhysicsScene::m_gravity(0, 0);

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0);
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	if (actor != nullptr)
		m_actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	if (actor != nullptr)
	{
		for (int i = 0; i < m_actors.size(); i++)
		{
			if (m_actors.at(i) == actor)
			{
				m_actors.erase(m_actors.begin() + i);
				return;
			}
		}
	}
}

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,
};

void PhysicsScene::Update(float dt)
{
	std::cout << GetTotalEnergy();

	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

#ifndef SimulatingRocket
		CheckForCollision();
#endif // !SimulatingRocket
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw(1);
	}
}

void PhysicsScene::CheckForCollision()
{
	int actorCount = m_actors.size();

	// need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->GetShapeID();
			int shapeId2 = object2->GetShapeID();

			// using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// reverse the order of arguments, as obj1 is the plane and obj2 is the Circle
	return Circle2Plane(obj2, obj1);
}

bool PhysicsScene::Circle2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Circle* circle = dynamic_cast<Circle*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	//if we are successful then test for collision
	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float sphereToPlane = glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();

		float intersection = circle->GetRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(circle->GetVelocity(), plane->GetNormal());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			//set Circle velocity to zero here
			plane->ResolveCollision(circle);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast objects to Circle and Circle
	Circle* circle1 = dynamic_cast<Circle*>(obj1);
	Circle* circle2 = dynamic_cast<Circle*>(obj2);
	// if we are successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		// TODO do the necessary math in here
		// TODO if the Circles touch, set their velocities to zero for now
		glm::vec2 dist = circle1->GetPosition() - circle2->GetPosition();
		if (glm::length(dist) < circle1->GetRadius() + circle2->GetRadius())
		{
			circle1->ResolveCollision(circle2);
			return true;
		}
	}

	return false;
}

float PhysicsScene::GetTotalEnergy()
{
	float total = 0;
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* obj = *it;
		total += obj->GetEnergy();
	}
	return total;
}