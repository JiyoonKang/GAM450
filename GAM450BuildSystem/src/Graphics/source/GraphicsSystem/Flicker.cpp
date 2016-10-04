#include <pch/precompiled.h>
#include <Graphics/includes/GraphicsSystem/Flicker.h>

static const float s_Pi = 3.14159265358979323846f;

Flicker::Flicker() : m_offsetRange(1.50f), m_speed(1.0f), m_offset(0.0f), m_movementLimit(0.0f), m_twoPi( 2 * s_Pi)
{
}

Flicker::~Flicker()
{
}

void Flicker::Initialize()
{
}

void Flicker::Remove()
{
}

void Flicker::Update(const float& dt)
{
	m_movementLimit += s_Pi * dt * m_speed;

	if (m_movementLimit >= m_twoPi)
		m_movementLimit = 0;

	m_offset = sinf(m_movementLimit) * m_offsetRange;
}

float Flicker::GetOffset()
{
	return m_offset;
}

void Flicker::SetOffset(float offset)
{
	m_offset = offset;
}

float Flicker::GetRange()
{
	return m_offsetRange;
}

void Flicker::SetRange(float range)
{
	m_offsetRange = range;
}