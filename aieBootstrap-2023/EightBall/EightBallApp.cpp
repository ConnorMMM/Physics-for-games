#include "EightBallApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"

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

EightBallApp::EightBallApp() {

}

EightBallApp::~EightBallApp() {

}

bool EightBallApp::startup() {
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01);
	m_physicsScene->SetGravity(glm::vec2(0));

	BoardStartUp();

	m_score = 0;

	return true;
}

void EightBallApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void EightBallApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	for each (PhysicsObject* ball in m_ballsInPockets)
	{
		if (Circle* poolBall = dynamic_cast<Circle*>(ball))
		{
			if (CueBall* cueBall = dynamic_cast<CueBall*>(poolBall))
			{
				cueBall->SetPosition(glm::vec2(40, 0));
				cueBall->SetVelocity(glm::vec2(0));
				cueBall->SetAngularVelocity(0);
			}
			else
			{
				poolBall->SetPosition(glm::vec2(-90 + (m_score * 5), 53));
				poolBall->SetKinematic(true);
			}
		}
	}
	m_ballsInPockets.clear();

	// TODO: added rules for eight-ball
	if (CueBall* cueBall = m_physicsScene->GetCueBall())
	{
		if (cueBall->GetVelocity() == glm::vec2(0))
		{
			glm::vec2 mousePos = ScreenToWorld(glm::vec2(input->getMouseX(), input->getMouseY()));
			cueBall->SetMousePos(mousePos);

			if (input->isKeyDown(aie::INPUT_KEY_A))
			{
				cueBall->AddToCueOffset(.2f);
			}
			if (input->isKeyDown(aie::INPUT_KEY_D))
			{
				cueBall->AddToCueOffset(-.2f);
			}

			if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				cueBall->SetHolding(true);
			}

			if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				cueBall->SetHolding(false);
			}
		}
	}


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void EightBallApp::draw() {

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

void EightBallApp::BoardStartUp()
{
	// Walls
	std::vector<Box*> m_walls;
	m_walls.push_back(new Box(glm::vec2(-84, 0), glm::vec2(0), 0, 5.f,
		glm::vec2(4, 35.25f), 0.8f, glm::vec4(1, 1, 1, 1)));
	m_walls.push_back(new Box(glm::vec2(84, 0), glm::vec2(0), 0, 5.f,
		glm::vec2(4, 35.25f), 0.3f, glm::vec4(1, 1, 1, 1)));
	m_walls.push_back(new Box(glm::vec2(40, -44), glm::vec2(0), 0, 5.f,
		glm::vec2(35.25f, 4), 0.3f, glm::vec4(1, 1, 1, 1)));
	m_walls.push_back(new Box(glm::vec2(-40, -44), glm::vec2(0), 0, 5.f,
		glm::vec2(35.25f, 4), 0.3f, glm::vec4(1, 1, 1, 1)));
	m_walls.push_back(new Box(glm::vec2(40, 44), glm::vec2(0), 0, 5.f,
		glm::vec2(35.25f, 4), 0.3f, glm::vec4(1, 1, 1, 1)));
	m_walls.push_back(new Box(glm::vec2(-40, 44), glm::vec2(0), 0, 5.f,
		glm::vec2(35.25f, 4), 0.3f, glm::vec4(1, 1, 1, 1)));
	for each (Box* wall in m_walls)
	{
		m_physicsScene->AddActor(wall);
		wall->SetKinematic(true);
	}
	// end Walls

	// Pockets
	std::vector<Circle*> m_pockets;
	m_pockets.push_back(new Circle(glm::vec2(-81.5f, 41.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	m_pockets.push_back(new Circle(glm::vec2(0, 45.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	m_pockets.push_back(new Circle(glm::vec2(81.5f, 41.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	m_pockets.push_back(new Circle(glm::vec2(-81.5, -41.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	m_pockets.push_back(new Circle(glm::vec2(0, -45.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	m_pockets.push_back(new Circle(glm::vec2(81.5f, -41.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	for each (Circle* pocket in m_pockets)
	{
		m_physicsScene->AddActor(pocket);
		pocket->SetTrigger(true);
		pocket->triggerEnter = [=](PhysicsObject* other) {
			m_ballsInPockets.push_back(other);
		};
	}
	// end Pockets


	CueBall* cueBall = new CueBall(glm::vec2(40, 0), glm::vec2(0), 3.0f, 1.7f, 0.8f);


	Circle* billiardBall1  = new Circle(glm::vec2(-30, 0), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(1, 0, 0, 1));
	Circle* billiardBall9  = new Circle(glm::vec2(-33.5f, 2), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 0, 1, 1));
	Circle* billiardBall2  = new Circle(glm::vec2(-33.5f, -2), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(1, 0, 0, 1));
	Circle* billiardBall3  = new Circle(glm::vec2(-37, 4), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(1, 0, 0, 1));
	Circle* billiardBall8  = new Circle(glm::vec2(-37, 0), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 0, 0, 1));
	Circle* billiardBall10  = new Circle(glm::vec2(-37, -4), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 0, 1, 1));
	Circle* billiardBall11  = new Circle(glm::vec2(-40.5f, 6), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 0, 1, 1));
	Circle* billiardBall7  = new Circle(glm::vec2(-40.5f, 2), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(1, 0, 0, 1));
	Circle* billiardBall14  = new Circle(glm::vec2(-40.5f, -2), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 0, 1, 1));
	Circle* billiardBall4 = new Circle(glm::vec2(-40.5f, -6), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(1, 0, 0, 1));
	Circle* billiardBall5 = new Circle(glm::vec2(-44, 8), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(1, 0, 0, 1));
	Circle* billiardBall13 = new Circle(glm::vec2(-44, 4), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 0, 1, 1));
	Circle* billiardBall15 = new Circle(glm::vec2(-44, 0), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 0, 1, 1));
	Circle* billiardBall6 = new Circle(glm::vec2(-44, -4), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(1, 0, 0, 1));
	Circle* billiardBall12 = new Circle(glm::vec2(-44, -8), glm::vec2(0), 5.0f, 2, 0.8f, glm::vec4(0, 0, 1, 1));

	m_physicsScene->AddActor(cueBall);

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
}

float EightBallApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}

glm::vec2 EightBallApp::ScreenToWorld(glm::vec2 screenPos)
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
