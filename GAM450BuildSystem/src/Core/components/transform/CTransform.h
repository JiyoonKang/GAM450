/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: CTransform.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/*****************************************************************
Filename:
Project:
Author(s):

All content ?2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "Base/util/math/Float4.h"
#include "base/util/math/float4x4.h"
#include "base/util/math/Quaternion.h"
#include "Base/util/Macros.h"
#include "Base/util/file/File.h"
#include "Core/components/base/Component.h"

namespace Core
{
	struct Position
	{
		float4 translation;
		quaternion rotation;
		float4 scale;
		Position() : rotation()
		{
			translation = float4(0.f, 0.f, 0.f, 0.f);
			scale = float4(1.f, 1.f, 1.f, 0.f);
		}
	};

	class Transform : public GameComponent
	{
	public:
		Transform();
		virtual ~Transform() override;

		virtual void Initialize() override;

		bool operator==(const Transform& rhs);
		Transform& operator=(const Transform& rhs);

		float4 const &GetTranslation(void) const;
		float4 const &GetScale(void) const;
		quaternion const &GetRotation(void) const;

		void SetTranslation(float4 const &tran);
		void SetScale(float4 const &scale);
		void SetRotation(quaternion const &rot);
		void SetRotation(float4x4 const &rot);

		float4 const &GetRightVector(void) const;
		float4 const &GetUpVector(void) const;
		float4 const &GetForwardVector(void) const;

		void SetRightVector(float4 const &right);
		void SetUpVector(float4 const &up);
		void SetForwardVector(float4 const &forward);

		void Translate(float4 const &Translation);

		float4x4 GetRotationMatrix(void) const;
		float4x4 const &GetWorldMatrix(void) const;

		void UpdateWorldMatrix(void);

		//////////////////////////////////////////////////////////////////////////
		// SERIALIZATION ROUTINE

		static void Serialize(File& file, Variable var);
		static void Deserialize(File& file, Variable var);

		//////////////////////////////////////////////////////////////////////////

		// This is an example of using hooks
		void ExampleHookedFunction(float dt) {};

		static void RegisterPrivateMembers();

    COMP_LUA_USABLE;

		Position m_position;

		float4 m_right;
		float4 m_up;
		float4 m_forward;

	private:
		float4x4 m_world;
	};
}