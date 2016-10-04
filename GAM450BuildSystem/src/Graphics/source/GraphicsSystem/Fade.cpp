#include <pch/precompiled.h>
#include <Graphics/includes/GraphicsSystem/Fade.h>
#include <GameLogic/PlayerController.h>

#include "Editor/InGameEditor/InGameEditor.h"

Fade::Fade() : m_opacity(1.0f), m_timer(3.0f), m_timeBetweenPulses(10.0f), m_fluctuate(false), m_pulse(false), m_update(true), m_currentTime(3.0f), m_delayTime(10.0f), m_fadeIn(false)
{
  
}

Fade::~Fade()
{
}

void Fade::Initialize()
{
}

void Fade::Remove()
{
}

void Fade::Update(const float& dt)
{
	//OH MY GOD SO MANY NESTED IFS.... I HATE MYSELF
	if (m_update)
	{
		if (m_pulse)
		{
			m_delayTime += dt;
			if (m_delayTime > m_timeBetweenPulses)
			{
				if (!m_fadeIn)
				{
					m_currentTime -= dt;

					m_opacity -= dt / m_timer;

					if (m_opacity < 0)
						m_opacity = 0;

					if (m_currentTime < 0)
					{
						m_fadeIn = true;
						m_delayTime = 0.0f;
					}
				}

				else
				{
					m_currentTime += dt;
					m_opacity += dt / m_timer;

					if (m_opacity > 1)
						m_opacity = 1;

					if (m_currentTime > m_timer)
						m_fadeIn = false;
				}
			}
		}

		else if (m_fluctuate)
		{
			if (!m_fadeIn)
			{
				m_currentTime -= dt;

				m_opacity -= dt / m_timer;

				if (m_opacity < 0)
					m_opacity = 0;

				if (m_currentTime < 0)
					m_fadeIn = true;
			}

			else
			{
				m_currentTime += dt;
				m_opacity += dt / m_timer;

				if (m_opacity > 1)
					m_opacity = 1;

				if (m_currentTime > m_timer)
					m_fadeIn = false;
			}
		}
		else
		{
			if (m_fadeIn)
			{
				if (m_currentTime < m_timer)
				{
					m_currentTime += dt;
					m_opacity += dt / m_timer;

					if (m_opacity > 1)
						m_opacity = 1;
				}
				
			}

			else
			{
				if (m_currentTime > 0)
				{
					m_currentTime -= dt;
					m_opacity -= dt / m_timer;

					if (m_opacity < 0)
						m_opacity = 0;
				}
			}
			
		}
	}
}

void Fade::SetFadeOutTime(float time)
{
	m_timer = time;
	m_currentTime = time;
}

void Fade::SetFluctuate(bool fluctuate)
{
	m_fluctuate = fluctuate;
}

bool Fade::IsFluctuating()
{
	return m_fluctuate;
}

void Fade::SetOpacity(float opacity)
{
	m_opacity = opacity;

	if (m_opacity > 1.0f)
		m_opacity = 1.0f;

	if (m_opacity < 0)
		m_opacity = 0.0f;

	m_currentTime = m_timer;
}

float Fade::GetOpacity()
{
	return m_opacity;
}

void Fade::FadeIn()
{
	m_fadeIn = true;
	m_opacity = 0.0f;

}

void Fade::FadeOut()
{
	m_fadeIn = false;
	m_opacity = 1.0f;
}