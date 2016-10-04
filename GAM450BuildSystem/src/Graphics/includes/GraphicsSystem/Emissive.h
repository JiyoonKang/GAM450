#pragma once

#include <Core\components\base\Component.h>
#include <Core\components\transform\CTransform.h>

class EmissiveComponent : public Core::GameComponent
{
	public:
		EmissiveComponent(void);
		~EmissiveComponent(void);

		//Base Methods
		virtual void Initialize() override;
		virtual void Remove(void) override;

		void Update(void);

		const float4& GetEmittedColor(void) const;
		void SetEmittedcolor(const float4& color);

		float4 m_emittedColor;
	private:
		Base::Handle m_PlayerController;
};