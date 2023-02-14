#include "CueBall.h"

#include <Gizmos.h>

    CueBall::CueBall(glm::vec2 position, glm::vec2 velocity, float mass, float radius,
        float elasticity) :
        Circle(position, velocity, mass, radius, elasticity, glm::vec4(1, 1, 1, 1))
    {
        m_cueDir = glm::vec2(0);

        m_holding = false;
        m_drawCue = false;
    }

    CueBall::~CueBall()
    {
    }

    void CueBall::FixedUpdate(glm::vec2 gravity, float timeStep)
    {
        Rigidbody::FixedUpdate(gravity, timeStep);

        if (m_holding)
        {
            m_cuePower = (m_currentMouseDis - m_heldMouseDis) / 2.f;

            if (m_cuePower >= 8) m_cuePower = 8;
            if (m_cuePower < 0) m_cuePower = 0;
        }
        else
        {
            m_cuePower = 0;
        }
    }

    void CueBall::Draw(float alpha)
    {
        CalculateSmoothedPosition(alpha);

        aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);

        aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(0, 0, 0, 1));

        if (m_drawCue)
        {
            if (m_holding)
            {
                aie::Gizmos::add2DCircle(m_smoothedPosition + (m_cueDir * m_heldMouseDis), 1, 12, m_color);
            }
            aie::Gizmos::add2DCircle(m_smoothedPosition + (m_cueDir * m_currentMouseDis), 1, 12, m_color);
            aie::Gizmos::add2DLine(m_smoothedPosition + (-m_cueDir * m_radius), m_smoothedPosition + (-m_cueDir * (m_cuePower * 2.f + m_radius)), glm::vec4(1, 0, 0, 1));
            aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + (-m_cueDir * glm::vec2(50, 50)), m_color);
        }
    }

    void CueBall::SetHolding(bool state)
    {
        if (m_holding && !state)
        {
            ApplyForce(-m_cueDir * m_cuePower * 120.f, glm::vec2(0));
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