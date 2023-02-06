#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demos.h"

#include "PhysicsScene.h"
#include "Circle.h"
#include "CueBall.h"
#include "Box.h"
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
#ifdef SymmetricalNewtonsCradle
	m_physicsScene->SetGravity(glm::vec2(0));  // turn off gravity

	Plane* plane1 = new Plane(glm::vec2(1, 0), -30, glm::vec4(1, 0, 0, 1));
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -30, glm::vec4(1, 0, 0, 1));

	Circle* ball1 = new Circle(glm::vec2(-16, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-8, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball4 = new Circle(glm::vec2(8, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball5 = new Circle(glm::vec2(16, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);

	ball1->ApplyForce(glm::vec2(-100, 0));

#endif // SymmetricalNewtonsCradle
#ifdef AsymmetricalNewtonsCradle
	m_physicsScene->SetGravity(glm::vec2(0));  // turn off gravity

	Plane* plane1 = new Plane(glm::vec2(1, 0), -30, glm::vec4(1, 0, 0, 1));
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -30, glm::vec4(1, 0, 0, 1));

	Circle* ball1 = new Circle(glm::vec2(-16, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-8, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball4 = new Circle(glm::vec2(8, 0), glm::vec2(0, 0), 10.0f, 4, glm::vec4(0, 0, 1, 1));
	Circle* ball5 = new Circle(glm::vec2(16, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);

	ball1->ApplyForce(glm::vec2(-98.6, 0), ball1->GetPosition());

#endif // AsymmetricalNewtonsCradle
#ifdef FallingOnAPlane
	m_physicsScene->SetGravity(glm::vec2(0, -9.81));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -40, glm::vec4(1, 0, 0, 1));
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -90, glm::vec4(1, 0, 0, 1));
	Plane* plane3 = new Plane(glm::vec2(1, 0), -90, glm::vec4(1, 0, 0, 1));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0, 0), 5.0f, 4, glm::vec4(0, 1, 0, 1));
	Box* box3 = new Box(glm::vec2(-20, 20), glm::vec2(0, 0), 5.f, glm::vec2(4, 4), glm::vec4(0, 1, 1, 1));

	Box* box1 = new Box(glm::vec2(0, 0), glm::vec2(0, 0), 5.f, glm::vec2(4, 4), glm::vec4(0, 1, 0, 1));
	Box* box2 = new Box(glm::vec2(20, 0), glm::vec2(0, 0), 5.f, glm::vec2(4, 4), glm::vec4(0, 1, 1, 1));
	
	Box* box4 = new Box(glm::vec2(-40, 20), glm::vec2(0, 0), 5.f, glm::vec2(4, 4), glm::vec4(0, 1, 1, 1));
	Box* box5 = new Box(glm::vec2(-40, 0), glm::vec2(0, 0), 5.f, glm::vec2(4, 4), glm::vec4(0, 1, 1, 1));
	
	Box* box6 = new Box(glm::vec2(-60, 20), glm::vec2(0, 0), 5.f, glm::vec2(4, 4), glm::vec4(0, 1, 1, 1));
	Box* box7 = new Box(glm::vec2(-60, 0), glm::vec2(0, 0), 5.f, glm::vec2(4, 4), glm::vec4(0, 1, 1, 1));

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(box3);

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);

	m_physicsScene->AddActor(box4);
	m_physicsScene->AddActor(box5);

	m_physicsScene->AddActor(box6);
	m_physicsScene->AddActor(box7);

	box2->ApplyForce(glm::vec2(0, 5), glm::vec2(4, 0));
	box7->ApplyForce(glm::vec2(0, 5), glm::vec2(4, 0));

#endif // FallingOnAPlane
#ifdef BounceToAStop
	m_physicsScene->SetGravity(glm::vec2(0, -9.81));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -40, 0.3f, glm::vec4(1, 0, 0, 1));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 3.0f, 4, 0.3f, glm::vec4(0, 1, 0, 1));
	Box* box1 = new Box(glm::vec2(0), glm::vec2(0), 5.f, glm::vec2(4, 4), 0.3f, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(box1);

#endif // BounceToAStop
#ifdef SetDreassingAPoolTable
	m_physicsScene->SetGravity(glm::vec2(0));

	//Walls
	Plane* plane1 = new Plane(glm::vec2( 0,  1), -40, 0.6f, glm::vec4(1, 0, 0, 1));
	Plane* plane2 = new Plane(glm::vec2( 0, -1), -40, 0.6f, glm::vec4(1, 0, 0, 1));
	Plane* plane3 = new Plane(glm::vec2( 1,  0), -80, 0.6f, glm::vec4(1, 0, 0, 1));
	Plane* plane4 = new Plane(glm::vec2(-1,  0), -80, 0.6f, glm::vec4(1, 0, 0, 1));
	
	CueBall* cueBall  = new CueBall(glm::vec2(40, 0),  glm::vec2(0), 3.0f, 3.4f, 0.8f);

	Circle* billiardBall1  = new Circle(glm::vec2(-30,  0   ), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall2  = new Circle(glm::vec2(-37,  4.5f), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall3  = new Circle(glm::vec2(-37, -4.5f), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall4  = new Circle(glm::vec2(-44,  8.5f),  glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall5  = new Circle(glm::vec2(-44,  0   ), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall6  = new Circle(glm::vec2(-44, -8.5f), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall7  = new Circle(glm::vec2(-51,  13  ), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall8  = new Circle(glm::vec2(-51,  4.5f), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall9  = new Circle(glm::vec2(-51, -4.5f), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall10 = new Circle(glm::vec2(-51, -13  ), glm::vec2(0), 5.0f, 4, 0.8f, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(cueBall);

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);

	m_physicsScene->AddActor(billiardBall1);
	m_physicsScene->AddActor(billiardBall2);
	m_physicsScene->AddActor(billiardBall3);
	m_physicsScene->AddActor(billiardBall4);
	m_physicsScene->AddActor(billiardBall5);
	m_physicsScene->AddActor(billiardBall6);
	m_physicsScene->AddActor(billiardBall7);
	m_physicsScene->AddActor(billiardBall8);
	m_physicsScene->AddActor(billiardBall9);
	m_physicsScene->AddActor(billiardBall10);

#endif // SetDreassingAPoolTable
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

#ifdef SetDreassingAPoolTable
	//for (int i = 0; i < m_physicsScene->)
	//{
	//
	//}

	CueBall* cue = dynamic_cast<CueBall*>(m_physicsScene->GetActor(0));
	if (cue != nullptr && cue->GetVelocity() == glm::vec2(0))
	{
		glm::vec2 force = glm::vec2(0);

		if (input->isKeyDown(aie::INPUT_KEY_A))
		{
			force.x += -400;
		}
		if (input->isKeyDown(aie::INPUT_KEY_D))
		{
			force.x += 400;
		}


		if (input->isKeyDown(aie::INPUT_KEY_W))
		{
			force.y += 400;
		}
		
		if (input->isKeyDown(aie::INPUT_KEY_S))
		{
			force.y += -400;
		}
		

		cue->ApplyForce(force, glm::vec2(0));
	}
#endif // SetDreassingAPoolTable

}

float PhysicsApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}