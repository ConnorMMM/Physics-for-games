#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demos.h"

#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"

#include <glm/ext.hpp>

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01);

	m_timeSteps = 0;

	DemoStartUp(1);

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	DemoUpdates(input, deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::DemoStartUp(int num)
{
#ifdef NewtonsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);

#endif // NewtonsFirstLaw
#ifdef NewtonsSecondLaw
	m_physicsScene->SetGravity(glm::vec2(0, -10));

	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);

#endif // NewtonsSecondLaw
#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0));  // turn off gravity

	Circle* ball1 = new Circle(glm::vec2(-10, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForceToActor(ball2, glm::vec2(-2, 0));

#endif // NewtonsThirdLaw
#ifdef SimulatingCollision
	m_physicsScene->SetGravity(glm::vec2(0));  // turn off gravity

	Circle* ball1 = new Circle(glm::vec2(-10, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForceToActor(ball2, glm::vec2(-4, 0));

#endif // SimulatingCollision
#ifdef SimulatingRocket
	m_physicsScene->SetGravity(glm::vec2(0, 0));  // turn off gravity

	Circle* rocket = new Circle(glm::vec2(0, -50), glm::vec2(0, 0), 1000.0f, 6, glm::vec4(1, 0, 0, 1));
	rocketDir = 180;

	m_physicsScene->AddActor(rocket);

#endif // SimulatingRocket
#ifdef TestCirclePlaneCollision
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Plane* plane = new Plane(glm::vec2(0, 1), -30, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(plane);

#endif // TestCirclePlaneCollision
#ifdef LecturesNewtonsCradle
	m_physicsScene->SetGravity(glm::vec2(0));  // turn off gravity

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 10.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(100, 0));

#endif // LecturesNewtonsCradle
}

void PhysicsApp::DemoUpdates(aie::Input* input, float dt)
{
#ifdef SimulatingRocket
	m_timeSteps++;

	if (m_timeSteps <= 4) return;

	Circle* rocket = dynamic_cast<Circle*>(m_physicsScene->GetActor(0));
	if (rocket != nullptr)
	{
		if (input->isKeyDown(aie::INPUT_KEY_A))
		{
			rocketDir += 10;
		}
		else if (input->isKeyDown(aie::INPUT_KEY_D))
		{
			rocketDir += -10;
		}
		
		glm::vec2 exhaustDir = glm::vec2(sin(DegreeToRadian(rocketDir)), cos(DegreeToRadian(rocketDir)));

		rocket->SetMass(rocket->GetMass() - 3);
		Circle* exhaust = new Circle(glm::vec2(rocket->GetPosition().x + (7 * exhaustDir.x), rocket->GetPosition().y + (7 * exhaustDir.y)), glm::vec2(0, 0), 3.0f, 1.5f, glm::vec4(.4, .4, .4, 1));
		m_physicsScene->AddActor(exhaust);

		rocket->ApplyForceToActor(exhaust, glm::vec2(800 * exhaustDir.x, 800 * exhaustDir.y));
	}

	m_timeSteps = 0;

#endif // SimulatingRocket

}

float PhysicsApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}
