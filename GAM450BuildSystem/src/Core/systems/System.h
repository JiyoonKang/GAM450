/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: System.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Core/types/message/Message.h"
#include <string>

namespace Core
{
	class ISystem
	{
	public:
    ISystem() {};
    // Virtual destructor
    virtual ~ISystem() {};

    // Register any system components with this function
    virtual void RegisterComponents() {};

		// Initialization call for the system
		virtual void Initialize() {};

		// All system can be updated every frame through this call
		virtual void Update(float dt) = 0;


		// Systems receive and handle messages in different ways
		virtual void ReceiveMessage(Message& msg) {};


		// Get the system's name
		virtual std::string GetName() const = 0;

    virtual void Shutdown() {};

    // Function to return debug data from the systems
    virtual const void* GetDebugData(void) {  return nullptr; };

	};
}