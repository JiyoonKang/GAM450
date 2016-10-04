#include <pch/precompiled.h>
#include <Graphics/includes/GraphicsSystem/Levitate.h>

static const float s_Pi = 3.14159265358979323846f;

Levitate::Levitate() : m_offsetRange(0.50f), m_speed(1.0f), m_offset(0.0f), m_movementLimit(0.0f), m_direction(1), m_twoPi( 2 * s_Pi)
{
}

Levitate::~Levitate()
{
}

void Levitate::Initialize()
{
}

void Levitate::Remove()
{
}

void Levitate::Update(const float& dt)
{
	m_movementLimit += s_Pi * dt * m_speed;

	if (m_movementLimit >= m_twoPi)
		m_movementLimit = 0;

	m_offset = sinf(m_movementLimit) * m_offsetRange;
}

float Levitate::GetOffset()
{
	return m_offset;
}

void Levitate::SetOffset(float offset)
{
	m_offset = offset;
}

float Levitate::GetRange()
{
	return m_offsetRange;
}

void Levitate::SetRange(float range)
{
	m_offsetRange = range;
}