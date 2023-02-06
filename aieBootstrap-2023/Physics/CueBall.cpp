#include "CueBall.h"

#include <Gizmos.h>

CueBall::CueBall(glm::vec2 position, glm::vec2 velocity, float mass, float radius,
    float elasticity) :
    Rigidbody(CIRCLE, position, velocity, 0, mass, elasticity, glm::vec4(1, 1, 1, 1))
{
    m_radius = radius;

    m_moment = 0.5f * mass * radius * radius;
}

CueBall::~CueBall()
{
}

void CueBall::Draw(float alpha)
{
    CalculateSmoothedPosition(alpha);

    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);

    aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(0, 0, 0, 1));
}