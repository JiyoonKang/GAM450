#pragma once

#include <Core\components\base\Component.h>

class Flicker : public Core::GameComponent
{
	public:
		Flicker(void);
		~Flicker(void);

		//Base Methods
		virtual void Initialize() override;
		virtual void Remove(void) override;

		void Update(const float& dt);

		float GetOffset(void);
		void SetOffset(float offset);

		float GetRange(void);
		void SetRange(float range);

		float m_offsetRange;
		float m_speed;
	private:
		float m_offset;
		float m_movementLimit;
		float m_twoPi;
}; 