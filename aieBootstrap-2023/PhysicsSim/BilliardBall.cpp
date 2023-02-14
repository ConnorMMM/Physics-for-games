#include "BilliardBall.h"

#include <Gizmos.h>

BilliardBall::BilliardBall(glm::vec2 position, glm::vec2 velocity, float mass, 
	float radius, float elasticity, int state) :
	Circle(position, velocity, mass, radius, elasticity, glm::vec4(0, 0, 0, 1))
{
	m_state = state;
	if (m_state == 1)
		m_color = glm::vec4(1, 0, 0, 1);
	else if (m_state == 2)
		m_color = glm::vec4(0, 0, 1, 1);
	else
		m_color = glm::vec4(0, 0, 0, 1);
}

BilliardBall::~BilliardBall()
{
}

void BilliardBall::Draw(float alpha)
{
	CalculateSmoothedPosition(alpha);

	//aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);

	//aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(1, 1, 1, 1));
}
