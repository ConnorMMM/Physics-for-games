#include "Eight_ballApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"

#include "Phys"
//#include "Circle.h"
//#include "CueBall.h"
//#include "Box.h"
//#include "Plane.h"
//#include "Spring.h"
//#include "SoftBody.h"

#include <glm/ext.hpp>

Eight_ballApp::Eight_ballApp() {

}

Eight_ballApp::~Eight_ballApp() {

}

bool Eight_ballApp::startup() {
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	//m_physicsScene = new PhysicsScene();
	//m_physicsScene->SetTimeStep(0.01);

	BoardStartUp();

	return true;
}

void Eight_ballApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void Eight_ballApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	//m_physicsScene->Update(deltaTime);
	//m_physicsScene->Draw();

	// TODO: Add code for the functionality of Eight-ball


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Eight_ballApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void Eight_ballApp::BoardStartUp()
{
	//Box* box1 = new Box(glm::vec2(0), glm::vec2(0), DegreeToRadian(45), 5.f, glm::vec2(4, 4), 0.3f, glm::vec4(0, 1, 0, 1));
}

float Eight_ballApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}

glm::vec2 Eight_ballApp::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2.f;
	worldPos.y -= getWindowHeight() / 2.f;

	// scale according to our extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}
