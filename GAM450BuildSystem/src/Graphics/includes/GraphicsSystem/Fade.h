#pragma once

#include <Core\components\base\Component.h>
#include <Core\components\transform\CTransform.h>

class Fade : public Core::GameComponent
{
	public:
		Fade(void);
		~Fade(void);

		//Base Methods
		virtual void Initialize() override;
		virtual void Remove(void) override;

		void Update(const float& dt);

		void SetFadeOutTime(float time);

		void SetFluctuate(bool fluctuate);
		bool IsFluctuating(void);

		void SetOpacity(float opacity);
		float GetOpacity(void);

		void FadeIn(void);
		void FadeOut(void);

		float m_opacity;
		float m_timer;
		float m_timeBetweenPulses;
		bool m_fluctuate;
		bool m_pulse;
		bool m_update;

	private:
		float m_currentTime;
		float m_delayTime;
		bool m_fadeIn;

}; 