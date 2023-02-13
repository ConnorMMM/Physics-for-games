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
#include "Spring.h"
#include "SoftBody.h"

#include <glm/ext.hpp>
#include <iostream>
#include <string>
#include <vector>

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

	m_score = 0;

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
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	m_2dRenderer->drawText(m_font, std::to_string(m_score).c_str(), 0, 30);

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
	Box* box1 = new Box(glm::vec2(0), glm::vec2(0), DegreeToRadian(45), 5.f, glm::vec2(4, 4), 0.3f, glm::vec4(0, 1, 0, 1));
	Box* box2 = new Box(glm::vec2(0, 12), glm::vec2(0), DegreeToRadian(45), 5.f, glm::vec2(4, 4), 0.3f, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);

#endif // BounceToAStop
#ifdef SetDreassingAPoolTable
	m_physicsScene->SetGravity(glm::vec2(0));
	
	// Walls
	Box* leftWall = new Box(glm::vec2(-84, 0), glm::vec2(0), 0, 5.f,
		glm::vec2(4, 35.25f), 0.8f, glm::vec4(1, 1, 1, 1));
	Box* rightWall = new Box(glm::vec2(84, 0), glm::vec2(0), 0, 5.f,
		glm::vec2(4, 35.25f), 0.3f, glm::vec4(1, 1, 1, 1));

	Box* topWall1 = new Box(glm::vec2(40, -44), glm::vec2(0), 0, 5.f,
		glm::vec2(35.25f, 4), 0.3f, glm::vec4(1, 1, 1, 1));
	Box* topWall2 = new Box(glm::vec2(-40, -44), glm::vec2(0), 0, 5.f,
		glm::vec2(35.25f, 4), 0.3f, glm::vec4(1, 1, 1, 1));

	Box* bottomWall1 = new Box(glm::vec2(40, 44), glm::vec2(0), 0, 5.f,
		glm::vec2(35.25f, 4), 0.3f, glm::vec4(1, 1, 1, 1));
	Box* bottomWall2 = new Box(glm::vec2(-40, 44), glm::vec2(0), 0, 5.f,
		glm::vec2(35.25f, 4), 0.3f, glm::vec4(1, 1, 1, 1));

	// Pockets
	std::vector<Circle*> m_pockets;
	Circle* pocket1 = new Circle(glm::vec2(-81.5f, 41.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1));
	m_pockets.push_back(pocket1);
	Circle* pocket2 = new Circle(glm::vec2(0, 45.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1));
	m_pockets.push_back(pocket2);
	Circle* pocket3 = new Circle(glm::vec2(81.5f, 41.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1));
	m_pockets.push_back(pocket3);
	Circle* pocket4 = new Circle(glm::vec2(-81.5, -41.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1));
	m_pockets.push_back(pocket4);
	Circle* pocket5 = new Circle(glm::vec2(0, -45.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1));
	m_pockets.push_back(pocket5);
	Circle* pocket6 = new Circle(glm::vec2(81.5f, -41.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1));
	m_pockets.push_back(pocket6);

	for each (Circle* pocket in m_pockets)
	{
		m_physicsScene->AddActor(pocket);
		pocket->SetTrigger(true);
		pocket->triggerEnter = [=](PhysicsObject* other) {
			Circle* ball = dynamic_cast<Circle*>(other);
			ball->SetPosition(glm::vec2(-90 + (m_score * 5), 53));
			ball->SetKinematic(true);
			m_score++;
		};
	}

	
	CueBall* cueBall  = new CueBall(glm::vec2(40, 0),  glm::vec2(0), 3.0f, 1.7f, 0.8f);


	Circle* billiardBall1  = new Circle(glm::vec2(-30,     0), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall2  = new Circle(glm::vec2(-33.5f,  2), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall3  = new Circle(glm::vec2(-33.5f, -2), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall4  = new Circle(glm::vec2(-37,     4), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall5  = new Circle(glm::vec2(-37,     0), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall6  = new Circle(glm::vec2(-37,    -4), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall7  = new Circle(glm::vec2(-40.5f,  6), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall8  = new Circle(glm::vec2(-40.5f,  2), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall9  = new Circle(glm::vec2(-40.5f, -2), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall10 = new Circle(glm::vec2(-40.5f, -6), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall11 = new Circle(glm::vec2(-44,     8), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall12 = new Circle(glm::vec2(-44,     4), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall13 = new Circle(glm::vec2(-44,     0), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall14 = new Circle(glm::vec2(-44,    -4), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));
	Circle* billiardBall15 = new Circle(glm::vec2(-44,    -8), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(cueBall);

	m_physicsScene->AddActor(leftWall);
	m_physicsScene->AddActor(rightWall);
	m_physicsScene->AddActor(topWall1);
	m_physicsScene->AddActor(topWall2);
	m_physicsScene->AddActor(bottomWall1);
	m_physicsScene->AddActor(bottomWall2);

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
	m_physicsScene->AddActor(billiardBall11);
	m_physicsScene->AddActor(billiardBall12);
	m_physicsScene->AddActor(billiardBall13);
	m_physicsScene->AddActor(billiardBall14);
	m_physicsScene->AddActor(billiardBall15);

	leftWall->SetKinematic(true);
	rightWall->SetKinematic(true);
	topWall1->SetKinematic(true);
	topWall2->SetKinematic(true);
	bottomWall1->SetKinematic(true);
	bottomWall2->SetKinematic(true);



#endif // SetDreassingAPoolTable
#ifdef BouncePads
	m_physicsScene->SetGravity(glm::vec2(0, -9.81));

	Circle* ball1 = new Circle(glm::vec2(0, 0), glm::vec2(0), 3.0f, 4, 1.f, glm::vec4(0, 1, 0, 1));
	Box* box1 = new Box(glm::vec2(0 , -40), glm::vec2(0), 0, 5.f, glm::vec2(4, 4), 1.f, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(box1);

	box1->SetKinematic(true);

#endif // BouncePads
#ifdef PhysicsBuckets
	m_physicsScene->SetGravity(glm::vec2(0, -9.81));

	Circle* ball1 = new Circle(glm::vec2(0, 0), glm::vec2(0), 3.0f, 4, 1.f, glm::vec4(0, 1, 0, 1));
	Box* box1 = new Box(glm::vec2(0, 20), glm::vec2(0), 0, 5.f, glm::vec2(4, 4), 1.f, glm::vec4(0, 0, 1, 1));
	Box* box2 = new Box(glm::vec2(0, -20), glm::vec2(0), 0, 5.f, glm::vec2(4, 4), 1.f, glm::vec4(0, 0, 1, 1));
	
	Box* box3 = new Box(glm::vec2(20, -40), glm::vec2(0), DegreeToRadian(45), 5.f, glm::vec2(15, 15), 1.f, glm::vec4(1, 0, 0, 1));
	Box* box4 = new Box(glm::vec2(-20, -40), glm::vec2(0), DegreeToRadian(45), 5.f, glm::vec2(15, 15), 1.f, glm::vec4(1, 0, 0, 1));

	Circle* ball2 = new Circle(glm::vec2(-30, 0), glm::vec2(0), 3.0f, 4, 1.f, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(-30, -20), glm::vec2(0), 3.0f, 4, 1.f, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	m_physicsScene->AddActor(box3);
	m_physicsScene->AddActor(box4);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);

	box3->SetKinematic(true);
	box4->SetKinematic(true);
	ball3->SetKinematic(true);

#endif // PhysicsBuckets
#ifdef Tutorial09
	m_physicsScene->SetGravity(glm::vec2(0, -9.81));

	Circle* ball1 = new Circle(glm::vec2(0, 0), glm::vec2(0), 3.0f, 4, 1.f, glm::vec4(0, 1, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-10, 0), glm::vec2(0), 3.0f, 4, 1.f, glm::vec4(1, 0, 0, 1));
	Spring* spring = new Spring(ball1, ball2, 10, 10, 30);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(spring);

	ball2->SetKinematic(true);

#endif // Tutorial09
#ifdef RopTest
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* prev = nullptr;
	for (int i = 0; i < 10; i++)
	{
		// spawn a circle to the right and below the previous one, so that the whole rope acts under gravity and swings
		Circle* circle = new Circle(glm::vec2(i * 3, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(1, 0, 0, 1));
		if (i == 0 || i == 9)
			circle->SetKinematic(true);
		m_physicsScene->AddActor(circle);
		if (prev)
			m_physicsScene->AddActor(new Spring(circle, prev, 500, 10, 7));
		prev = circle;
	}

	// add a kinematic box at an angle for the rope to drape over
	Box* box = new Box(glm::vec2(15, -20), glm::vec2(0), 20, 20, glm::vec2(2, 8), 1, glm::vec4(0, 0, 1, 1));
	box->SetKinematic(true);
	m_physicsScene->AddActor(box);

#endif // RopTest
#ifdef SoftBodyTest
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	std::vector<std::string> c;
	c.push_back("000000");
	c.push_back("000000");
	c.push_back("000000");
	c.push_back("000...");
	c.push_back("000...");
	c.push_back("000...");
	c.push_back("000000");
	c.push_back("000000");
	c.push_back("000000");
	SoftBody::Build(m_physicsScene, glm::vec2(-100, 0), 5.0f, 10.0f, 6, c);

	std::vector<std::string> o;
	o.push_back("000000");
	o.push_back("000000");
	o.push_back("000000");
	o.push_back("00..00");
	o.push_back("00..00");
	o.push_back("00..00");
	o.push_back("000000");
	o.push_back("000000");
	o.push_back("000000");
	SoftBody::Build(m_physicsScene, glm::vec2(-60, 0), 1.0f, 40.0f, 6, o);

	std::vector<std::string> n;
	n.push_back("00..00");
	n.push_back("00.000");
	n.push_back("00.000");
	n.push_back("000000");
	n.push_back("000000");
	n.push_back("000000");
	n.push_back("000.00");
	n.push_back("000.00");
	n.push_back("00..00");
	SoftBody::Build(m_physicsScene, glm::vec2(-20, 0), 5.0f, 10.0f, 6, n);
	SoftBody::Build(m_physicsScene, glm::vec2(30, 0), 5.0f, 10.0f, 6, n);
	SoftBody::Build(m_physicsScene, glm::vec2(70, 0), 5.0f, 10.0f, 6, o);

	Plane* plane1 = new Plane(glm::vec2(0, 1), -40, 0.6f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(plane1);
	

#endif // SoftBodyTest

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
	
	if (CueBall* cueBall = m_physicsScene->GetCueBall())
	{
		if (cueBall->GetVelocity() == glm::vec2(0))
		{
			glm::vec2 mousePos = ScreenToWorld(glm::vec2(input->getMouseX(), input->getMouseY()));
			cueBall->CueDirFromMousePos(mousePos);

			if (input->isKeyDown(aie::INPUT_KEY_A))
			{
				cueBall->AddToCueOffset(.2f);
			}
			if (input->isKeyDown(aie::INPUT_KEY_D))
			{
				cueBall->AddToCueOffset(-.2f);
			}

			if (input->wasKeyPressed(aie::INPUT_KEY_S))
			{
				cueBall->HoldingCue();
			}

			if (cueBall->GetHoldingCue() && input->isKeyUp(aie::INPUT_KEY_S))
			{
				cueBall->ReleaseCue();
			}
		}
	}

#endif // SetDreassingAPoolTable

}

float PhysicsApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}

glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// scale according to our extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}
