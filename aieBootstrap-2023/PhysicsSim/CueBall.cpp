#include "CueBall.h"

#include <Gizmos.h>

    CueBall::CueBall(glm::vec2 position, glm::vec2 velocity, float mass, float radius,
        float elasticity) :
        Circle(position, velocity, mass, radius, elasticity, glm::vec4(1, 1, 1, 1))
    {
        m_cueDir = glm::vec2(0);
        m_cueOffset = 0;

        m_holding = false;
        m_cueDistance = 0;
    }

    CueBall::~CueBall()
    {
    }

    void CueBall::FixedUpdate(glm::vec2 gravity, float timeStep)
    {
        Rigidbody::FixedUpdate(gravity, timeStep);

        if (m_holding)
        {
            m_cueDistance = (m_currentMouseDis - m_heldMouseDis) / 2.f;

            if (m_cueDistance >= 8) m_cueDistance = 8;
            if (m_cueDistance < 0) m_cueDistance = 0;
        }
        else
        {
            m_cueDistance = 0;
        }
    }

    void CueBall::Draw(float alpha)
    {
        CalculateSmoothedPosition(alpha);

        aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);

        aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(0, 0, 0, 1));

        if (GetVelocity() == glm::vec2(0))
        {
            if (m_holding)
            {
                aie::Gizmos::add2DCircle(m_smoothedPosition + (m_cueDir * m_heldMouseDis), 1, 12, m_color);
            }
            aie::Gizmos::add2DCircle(m_smoothedPosition + (m_cueDir * m_currentMouseDis), 1, 12, m_color);
            aie::Gizmos::add2DLine(m_smoothedPosition + (-m_cueDir * m_radius), m_smoothedPosition + (-m_cueDir * (m_cueDistance * 2.f + m_radius)), glm::vec4(1, 0, 0, 1));
            aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + (-m_cueDir * glm::vec2(50, 50)), m_color);
        }
    }

    void CueBall::SetHolding(bool state)
    {
        if (m_holding && !state)
        {
            ApplyForce(-m_cueDir * m_cueDistance * 120.f, glm::vec2(0));
        }
        else if (!m_holding && state)
        {
            m_heldMouseDis = m_currentMouseDis;
        }
        
        m_holding = state;
    }

    void CueBall::SetMousePos(glm::vec2 mousePos)
    {
        m_currentMouseDis = glm::distance(m_position, mousePos);
        m_cueDir = glm::normalize(mousePos - m_position);
    }


    void CueBall::AddToCueOffset(float offset)
    {
        m_cueOffset += offset;

        if (m_cueOffset > 3) m_cueOffset = 3;
        if (m_cueOffset < -3) m_cueOffset = -3;
    }
