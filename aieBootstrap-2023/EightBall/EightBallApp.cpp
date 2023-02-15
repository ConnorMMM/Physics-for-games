#include "EightBallApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"

#include "PhysicsScene.h"
#include "Circle.h"
#include "CueBall.h"
#include "BilliardBall.h"
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

	m_poolTable = new aie::Texture("./textures/pooltable.png");

	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01);
	m_physicsScene->SetGravity(glm::vec2(0));

	BoardStartUp();

	m_player1Turn = true;

	m_firstBallStruck = nullptr;
	m_firstSunk = false;
	m_whiteSunk = false;

	m_solidSunk = 0;
	m_stripeSunk = 0;

	m_inPlay = false;
	m_gameOver = false;

	m_ballSunk = false;
	m_foul = false;
	m_shotsLeft = 1;

	return true;
}

void EightBallApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void EightBallApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	m_physicsScene->Update(deltaTime);

	CheckPockets();
	UpdateGameState(input);

	// FOR TESTING
	if (input->isKeyDown(aie::INPUT_KEY_K))
	{
		m_cueBall->SetPosition(ScreenToWorld(glm::vec2(input->getMouseX(), input->getMouseY())));
		m_cueBall->SetVelocity(glm::vec2(0));
		m_cueBall->SetAngularVelocity(0);
	}


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void EightBallApp::draw() 
{
	m_physicsScene->Draw();

	if (m_whiteSunk && !m_inPlay)
		DrawCueBallPlacement();

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// Gameboard and objects in it
	m_2dRenderer->drawSprite(m_poolTable, getWindowWidth() / 2.f, getWindowHeight() / 2.f, 1050, 580.52f, 0, 0);
	DrawBalls();

	DrawUI();
	
	// done drawing sprites
	m_2dRenderer->end();

	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));

	aie::Gizmos::clear();
	
}

void EightBallApp::UpdateGameState(aie::Input* input)
{
	if (m_inPlay)
	{
		m_cueBall->SetDrawCue(false);

		if (m_physicsScene->AllStationary())
		{
			if (m_gameOver)
			{
				if (m_whiteSunk || m_foul)
					m_playerWin = false;
			}
			else
			{
				if (!m_foul)
				{
					if(BilliardBall* billiardBall = dynamic_cast<BilliardBall*>(m_firstBallStruck))
					{
						if (m_firstSunk)
						{
							if ((m_player1Solid && m_player1Turn) || (!m_player1Solid && !m_player1Turn))
							{
								if (billiardBall->GetState() == 2 || (billiardBall->GetState() == 0 && m_solidSunk != 7))
									m_foul = true;
							}
							else
							{
								if (billiardBall->GetState() == 1 || (billiardBall->GetState() == 0 && m_solidSunk != 7))
									m_foul = true;
							}
						}
						else
						{
							if (billiardBall->GetState() == 0)
								m_foul = true;
						}
					}
					else
					{
						m_foul = true;
					}
				}

				m_shotsLeft += m_ballSunk ? 1 : 0;
				m_shotsLeft--;
				if (m_foul || m_shotsLeft <= 0)
				{
					m_player1Turn = !m_player1Turn;
					m_shotsLeft = m_foul ? 2 : 1;
					m_foul = false;
				}
			}

			m_firstBallStruck = nullptr;
			m_ballSunk = false;
			m_inPlay = false;
		}
	}

	if (!m_gameOver && !m_inPlay)
	{
		glm::vec2 mousePos = ScreenToWorld(glm::vec2(input->getMouseX(), input->getMouseY()));
		if (m_whiteSunk)
		{
			if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT) &&
				mousePos.y < 34.8f && mousePos.y > -34.8f && mousePos.x < 80 && mousePos.x > 30)
			{
				m_cueBall->SetPosition(mousePos);
				m_cueBall->SetVelocity(glm::vec2(0));
				m_cueBall->SetAngularVelocity(0);
				m_whiteSunk = false;
			}
		}
		else
		{
			m_cueBall->SetMousePos(mousePos);

			if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				m_cueBall->SetHolding(true);
			}

			if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT) && m_cueBall->IsHolding())
			{
				m_cueBall->SetHolding(false);
				m_inPlay = true;
			}

			m_cueBall->SetDrawCue(true);
		}
	}
}

void EightBallApp::CheckPockets()
{
	for each (PhysicsObject * ball in m_ballsInPockets)
	{
		if (CueBall* cueBall = dynamic_cast<CueBall*>(ball))
		{
			m_whiteSunk = true;
			m_foul = true;

			cueBall->SetPosition(glm::vec2(400, 0));
			cueBall->SetVelocity(glm::vec2(0));
			cueBall->SetAngularVelocity(0);
		}
		else if (BilliardBall* billiardBall = dynamic_cast<BilliardBall*>(ball))
		{
			int ballState = billiardBall->GetState();
			m_ballSunk = true;

			if (ballState == 1)
			{
				m_solidSunk++;
			}
			else if (ballState == 2)
			{
				m_stripeSunk++;
			}
			else
			{
				m_gameOver = true;
				CheckGameOver();
			}

			if (!m_firstSunk)
			{
				m_player1Solid = (m_player1Turn && ballState == 1) ||
					(!m_player1Turn && ballState == 2) ? true : false;
				m_firstSunk = true;
			}

			if ((m_player1Solid && m_player1Turn) || (!m_player1Solid && !m_player1Turn))
			{
				if (ballState == 2)
					m_foul = true;
			}
			else
			{
				if (ballState == 1)
					m_foul = true;
			}

			billiardBall->SetPosition(glm::vec2(-90 + ((m_solidSunk + m_stripeSunk) * 5), 53));
			billiardBall->SetKinematic(true);
		}
	}
	m_ballsInPockets.clear();
}

/// <summary>
/// Checks if the current players has sunk all there balls.
/// </summary>
void EightBallApp::CheckGameOver()
{
	if (!m_firstSunk)
	{
		m_playerWin = false;
		return;
	}

	if ((m_player1Solid && m_player1Turn) || (!m_player1Solid && !m_player1Turn))
	{
		m_playerWin = m_solidSunk == 7;
	}
	else
	{
		m_playerWin = m_stripeSunk == 7;
	}
}

/// <summary>
/// This Initilises all the objects/actors in the scene. This includes the walls, 
/// pockets, billiard balls and the cue ball.
/// </summary>
void EightBallApp::BoardStartUp()
{
	// Walls
	std::vector<Box*> walls;
	walls.push_back(new Box(glm::vec2(-77.2f, 0), glm::vec2(0), 0, 5.f,
		glm::vec2(4, 30.8f), 0.8f, glm::vec4(1, 1, 1, 1)));
	walls.push_back(new Box(glm::vec2(77.2f, 0), glm::vec2(0), 0, 5.f,
		glm::vec2(4, 30.8f), 0.3f, glm::vec4(1, 1, 1, 1)));
	walls.push_back(new Box(glm::vec2(35.9f, -40.4f), glm::vec2(0), 0, 5.f,
		glm::vec2(31.8f, 4), 0.3f, glm::vec4(1, 1, 1, 1)));
	walls.push_back(new Box(glm::vec2(-35.9f, -40.4f), glm::vec2(0), 0, 5.f,
		glm::vec2(31.8f, 4), 0.3f, glm::vec4(1, 1, 1, 1)));
	walls.push_back(new Box(glm::vec2(35.9f, 40.4f), glm::vec2(0), 0, 5.f,
		glm::vec2(31.8f, 4), 0.3f, glm::vec4(1, 1, 1, 1)));
	walls.push_back(new Box(glm::vec2(-35.9f, 40.4f), glm::vec2(0), 0, 5.f,
		glm::vec2(31.8f, 4), 0.3f, glm::vec4(1, 1, 1, 1)));
	for each (Box* wall in walls)
	{
		m_physicsScene->AddActor(wall);
		wall->SetKinematic(true);
		wall->SetHidden(true);
	}
	// end Walls

	// Pockets
	std::vector<Circle*> pockets;
	pockets.push_back(new Circle(glm::vec2(-75, 38.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	pockets.push_back(new Circle(glm::vec2(0, 42), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	pockets.push_back(new Circle(glm::vec2(75, 38.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	pockets.push_back(new Circle(glm::vec2(-75, -38.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	pockets.push_back(new Circle(glm::vec2(0, -42), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	pockets.push_back(new Circle(glm::vec2(75, -38.5f), glm::vec2(0), 5.0f, 4.75f, 0.8f, glm::vec4(1, 1, 0, 1)));
	for each (Circle* pocket in pockets)
	{
		m_physicsScene->AddActor(pocket);
		pocket->SetTrigger(true);
		pocket->SetHidden(true);
		pocket->triggerEnter = [=](PhysicsObject* other) {
			m_ballsInPockets.push_back(other);
		};
	}
	// end Pockets

	// Cue Ball
	m_cueBall = new CueBall(glm::vec2(40, 0), glm::vec2(0), 3.0f, 1.7f, 0.8f);
	m_physicsScene->AddActor(m_cueBall);
	m_cueBall->collisionCallback = [=](PhysicsObject* other) {
		if (!m_firstBallStruck)
			m_firstBallStruck = other;
	};
	// end Cue Ball

	// Billiard Balls
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-30, 0), glm::vec2(0), 5.0f, 2, 0.8f, 1));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball1.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-33.5f, 2), glm::vec2(0), 5.0f, 2, 0.8f, 2));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball11.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-33.5f, -2), glm::vec2(0), 5.0f, 2, 0.8f, 1));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball6.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-37, 4), glm::vec2(0), 5.0f, 2, 0.8f, 1));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball2.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-37, 0), glm::vec2(0), 5.0f, 2, 0.8f, 0));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball8.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-37, -4), glm::vec2(0), 5.0f, 2, 0.8f, 2));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball14.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-40.5f, 6), glm::vec2(0), 5.0f, 2, 0.8f, 2));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball12.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-40.5f, 2), glm::vec2(0), 5.0f, 2, 0.8f, 1));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball5.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-40.5f, -2), glm::vec2(0), 5.0f, 2, 0.8f, 2));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball9.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-40.5f, -6), glm::vec2(0), 5.0f, 2, 0.8f, 1));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball3.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-44, 8), glm::vec2(0), 5.0f, 2, 0.8f, 1));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball7.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-44, 4), glm::vec2(0), 5.0f, 2, 0.8f, 2));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball10.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-44, 0), glm::vec2(0), 5.0f, 2, 0.8f, 2));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball13.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-44, -4), glm::vec2(0), 5.0f, 2, 0.8f, 1));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball4.png"));
	m_billiardballs.push_back(new BilliardBall(glm::vec2(-44, -8), glm::vec2(0), 5.0f, 2, 0.8f, 2));
	m_billiardBallTextures.push_back(new aie::Texture("./textures/poolballs/ball15.png"));
	for each (BilliardBall* ball in m_billiardballs)
	{
		m_physicsScene->AddActor(ball);
	}
	// end Billiard Balls
}

/// <summary>
/// Draws the different balls onto the screen
/// </summary>
void EightBallApp::DrawBalls()
{
	// Billiard Balls
	for (int i = 0; i < m_billiardballs.size(); i++)
	{
		aie::Texture* ballTexture = m_billiardBallTextures[i];
		glm::vec2 ballPos = WorldToScreen(m_billiardballs[i]->GetSmoothedPosition());
		float widthandHeight = m_billiardballs[i]->GetRadius() * 12.6f;
		m_2dRenderer->drawSprite(ballTexture, ballPos.x, ballPos.y, widthandHeight, widthandHeight,
			m_billiardballs[i]->GetOrientation(), 0);
	}
	// Billiard Balls

	
}

/// <summary>
/// Draws the area that the cue ball can be placed into after it was sunk. Also 
/// draws a Gizmo showing where it would be placed.
/// </summary>
void EightBallApp::DrawCueBallPlacement()
{
	aie::Gizmos::add2DAABBFilled(glm::vec2(57.5, 0), glm::vec2(18, 34.8f), glm::vec4(0, .7f, 0, .4f));
	aie::Input* input = aie::Input::getInstance();
	aie::Gizmos::add2DCircle(ScreenToWorld(glm::vec2(input->getMouseX(), input->getMouseY())), 
		1.7f, 12, glm::vec4(1, 1, 1, 1));
}

void EightBallApp::DrawUI()
{
	float windowWidth = getWindowWidth();
	float windowHeight = getWindowHeight();

	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	std::string playerText = m_player1Turn ? "Player 1 Turn" : "Player 2 Turn";
	if (m_firstSunk)
	{
		playerText += (m_player1Solid && m_player1Turn) || (!m_player1Solid && !m_player1Turn) ? ": Solid" : ": Stripe";
	}
	m_2dRenderer->drawText(m_font, playerText.c_str(), (windowWidth * .5f) - 115, windowHeight - 30);

	std::string shots = "Shots Left: " + std::to_string(m_shotsLeft);
	m_2dRenderer->drawText(m_font, shots.c_str(), windowWidth - 250, windowHeight - 30);


	if (!m_inPlay && m_gameOver)
		DrawWinState();
}

/// <summary>
/// 
/// </summary>
void EightBallApp::DrawWinState()
{
	if ((m_player1Turn && m_playerWin) || (!m_player1Turn && !m_playerWin))
		m_2dRenderer->drawText(m_font, "Player 1 Wins", 300, 300);
	else
		m_2dRenderer->drawText(m_font, "Player 2 Wins", 300, 300);
}

/// <summary>
/// Converts degrees to radians.
/// </summary>
/// <param name="degree"> The degree that is to be converted </param>
/// <returns></returns>
float EightBallApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}

/// <summary>
/// Converts screen position to world position.
/// </summary>
/// <param name="screenPos"> The screen position that it to be converted </param>
/// <returns></returns>
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
/// <summary>
/// Converts world position to screen position.
/// </summary>
/// <param name="worldPos"> The world position that it to be converted </param>
/// <returns></returns>
glm::vec2 EightBallApp::WorldToScreen(glm::vec2 worldPos)
{
	glm::vec2 screenPos = worldPos;
	
	// scale according to our extents
	screenPos.x /= 2.0f * m_extents / getWindowWidth();
	screenPos.y /= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	// move the centre of the screen to (0,0)
	screenPos.x += getWindowWidth() / 2;
	screenPos.y += getWindowHeight() / 2;

	return screenPos;
}
