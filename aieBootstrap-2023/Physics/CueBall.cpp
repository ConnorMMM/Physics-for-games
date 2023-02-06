#include "CueBall.h"

#include "Demos.h"

#include <Gizmos.h>

CueBall::CueBall(glm::vec2 position, glm::vec2 velocity, float mass, float radius,
    float elasticity) :
    Circle(position, velocity, mass, radius, elasticity, glm::vec4(1, 1, 1, 1))
{
    m_cueDir = glm::vec2(0);
    m_cueOffset = 0;

    m_holdingCue = false;
    m_cueDistance = 0;
}

CueBall::~CueBall()
{
}

void CueBall::FixedUpdate(glm::vec2 gravity, float timeStep)
{
    Rigidbody::FixedUpdate(gravity, timeStep);

    if (m_holdingCue)
    {
        if (m_cueDistance >= 7) m_cueDistance = 7;
        else m_cueDistance += .1f;
    }
}

void CueBall::Draw(float alpha)
{
    CalculateSmoothedPosition(alpha);

    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);

    aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(0, 0, 0, 1));

    aie::Gizmos::add2DCircle(m_smoothedPosition + (m_cueDir * 8.f) + (m_cueDir * m_cueDistance) + glm::vec2(1 * m_cueOffset, 0),
        1, 12, m_color);
}

void CueBall::HoldingCue()
{
    m_holdingCue = true;
    m_cueDistance = 0;
}

void CueBall::ReleaseCue()
{
    if (m_holdingCue)
    {
        glm::vec2 forceDir = -1.f * m_cueDir;
        ApplyForce(forceDir * m_cueDistance * 120.f, glm::vec2(0));
        m_cueDistance = 0;
        m_holdingCue = false;
    }
}

void CueBall::AddToCueOffset(float offset)
{
    m_cueOffset += offset;

    if (m_cueOffset > 3) m_cueOffset = 3;
    if (m_cueOffset < -3) m_cueOffset = -3;
}

void CueBall::CueDirFromMousePos(glm::vec2 mousePos)
{
    m_cueDir = glm::normalize(mousePos - m_position);
}
