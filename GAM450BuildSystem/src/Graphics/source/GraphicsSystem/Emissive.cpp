#include <pch/precompiled.h>
#include <Graphics/includes/GraphicsSystem/Emissive.h>
#include <GameLogic/PlayerController.h>

EmissiveComponent::EmissiveComponent() : m_emittedColor(float4())
{
}

EmissiveComponent::~EmissiveComponent()
{
}

void EmissiveComponent::Initialize()
{
	if (space->GetComponent(ePlayerController, owner))
		m_PlayerController = space->GetComponent(ePlayerController, owner)->self;

	if (m_PlayerController != Handle::null)
	{
		GameLogic::PlayerController* parent = space->GetHandles().GetAs<GameLogic::PlayerController>(m_PlayerController);

		if (parent)
			m_emittedColor = parent->GetColor();
	}
}

void EmissiveComponent::Remove()
{
}

void EmissiveComponent::Update()
{
	if (m_PlayerController != NULL)
	{
		GameLogic::PlayerController* parent = space->GetHandles().GetAs<GameLogic::PlayerController>(m_PlayerController);

		if (parent)
			m_emittedColor = parent->GetColor();
	}
}

const float4& EmissiveComponent::GetEmittedColor() const
{
	return m_emittedColor;
}

void EmissiveComponent::SetEmittedcolor(const float4& color)
{
	m_emittedColor = color;
}

