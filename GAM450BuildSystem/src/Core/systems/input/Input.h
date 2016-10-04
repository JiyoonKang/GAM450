/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Input.h
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "Core/systems/System.h"
#include "Core/systems/input/KeyboardManager.h"
#include "Core/systems/input/GamePad.h"
#include "Core/systems/input/Mouse.h"

namespace Input
{
	class InputHandler : public Core::ISystem
	{
	public:
		InputHandler(void);
		~InputHandler(void);

		virtual void Initialize(void) override;
		virtual void Shutdown(void) override;
		virtual void Update(float dt) override;
		virtual std::string GetName(void) const { return "Input"; }

		static InputHandler* GetInstance(void);
		static void ToggleKeyInput(void);
		static void SetKeyInput(bool input);
		static bool GetKeyInput(void);
	};
	static bool AllowKeyInput = false;
	extern InputHandler* INPUT;
}