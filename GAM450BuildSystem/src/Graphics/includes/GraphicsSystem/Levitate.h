#pragma once

#include <Core\components\base\Component.h>

class Levitate : public Core::GameComponent
{
	public:
		Levitate(void);
		~Levitate(void);

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
		int m_direction;
		float m_twoPi;
}; 