#pragma once

#include <Core\components\base\Component.h>
#include <Base/util/math/VectorMath.h>
#include <Core\components\transform\CTransform.h>

class LightDeath : public Core::GameComponent
{
	public:
		LightDeath(void);
		~LightDeath(void);

		//Base Methods
		virtual void Initialize() override;
		virtual void Remove(void) override;

		void Update(const float& dt);

		void Kill(void);
	
		bool IsDead(void);

		float2 GetOffset(void);
		void SetAnimationLength(float length);

		float animationLength;

	private:
		bool m_Kill;
		bool m_isDead;
		float2 m_rangeAttenuation;
		float2 m_rangeAttenuationOffset;
		Base::Handle m_pointLight;
}; 