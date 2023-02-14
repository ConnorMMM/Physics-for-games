#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Texture.h"
#include "Input.h"

#include <glm/vec2.hpp>
#include <vector>

#define PI 3.14159265359

class PhysicsScene;
class PhysicsObject;
class BilliardBall;

class EightBallApp : public aie::Application {
public:

	EightBallApp();
	virtual ~EightBallApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void UpdateGameState(aie::Input* input);

	void CheckPockets();
	void CheckGameOver();

	void BoardStartUp();


	float DegreeToRadian(float degree);
	glm::vec2 ScreenToWorld(glm::vec2 screenPos);
	glm::vec2 WorldToScreen(glm::vec2 worldPos);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	aie::Texture* m_poolTable;

	std::vector<PhysicsObject*> m_physicsObjects;

	std::vector<BilliardBall*> m_billiardballs;
	std::vector<aie::Texture*> m_billiardBallTextures;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	std::vector<PhysicsObject*> m_ballsInPockets;

	bool m_player1Turn;
	bool m_player1Solid;


	bool m_firstSunk;
	bool m_whiteSunk;

	int m_solidSunk;
	int m_stripeSunk;

	bool m_inPlay;
	bool m_gameOver;
	bool m_playerWin;

	bool m_foul;
	bool m_ballSunk;
	int m_shotsLeft;
};