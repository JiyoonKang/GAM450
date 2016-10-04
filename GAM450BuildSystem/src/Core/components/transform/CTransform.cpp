/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: CTransform.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/*****************************************************************
Filename:
Project:
Author(s):

Need to change this

All content ?2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CTransform.h"

namespace
{
	using namespace Core;

	float4x4 CalculateWorldMatrix(float4 const &Position, quaternion const &Orientation, float4 const &Scale)
	{
		return float4x4::CreateTranslationMatrix(float3(Position.x, Position.y, Position.z)) *
			Orientation.ToMatrix() * float4x4::CreateScaleMatrix(float3(Scale.x, Scale.y, Scale.z));
	}
}

namespace Core
{

	void Transform::Initialize()
	{
		//EXAMPLE
#if(0)
		/* Game space hooking */
		space->hooks.Add("TestSpaceHook", self, BUILD_FUNCTION(Transform::ExampleHookedFunction));

		/* Game Object hooking */
		space->GetGameObject(owner)->hooks.Add("TestObjectHook", self,
			BUILD_FUNCTION(Transform::ExampleHookedFunction));

		/* Call the hooks! */
		space->hooks.Call("TestSpaceHook", 0.167f); // Called in game space
		space->hooks.Call("TestObjectHook", 0.5f); // Does nothing since there is no TestObjectHook in the game space
		space->GetGameObject(owner)->hooks.Call("TestObjectHook", 0.2f); // Call the event on the object, this works!
#endif
	}


	// TODO: Make sure the right, up and forward vectors are correct
	Transform::Transform()
		: m_position()
		, m_right(1.f, 0.f, 0.f, 0.f)
		, m_up(0.f, 1.f, 0.f, 0.f)
		, m_forward(0.f, 0.f, 1.f, 0.f)
		, m_world()
	{
		m_world = CalculateWorldMatrix(m_position.translation, m_position.rotation, m_position.scale);
	}

	Transform::~Transform()
	{
	}

	Transform& Transform::operator=(const Transform& rhs)
	{
		if (this != &rhs)
		{
			m_position.translation = rhs.GetTranslation();
			m_position.rotation = rhs.GetRotation();
			m_position.scale = rhs.GetScale();
			m_right = rhs.GetRightVector();
			m_up = rhs.GetUpVector();
			m_forward = rhs.GetForwardVector();
			m_world = rhs.GetWorldMatrix();
		}
		return *this;
	}

	bool Transform::operator==(const Transform& rhs)
	{
		return ((m_position.translation == rhs.m_position.translation) &&
			(m_position.scale == rhs.m_position.scale) && (m_position.rotation == rhs.m_position.rotation));
	}

	void Transform::SetTranslation(float4 const &tran)
	{
		m_position.translation = tran;
		m_world = CalculateWorldMatrix(m_position.translation, m_position.rotation, m_position.scale);
	}

	void Transform::SetScale(float4 const &scale)
	{
		m_position.scale = scale;
		m_world = CalculateWorldMatrix(m_position.translation, m_position.rotation, m_position.scale);
	}

	void Transform::SetRotation(quaternion const &rot)
	{
		m_position.rotation = rot;
		m_position.rotation.Normalize();
		m_world = CalculateWorldMatrix(m_position.translation, m_position.rotation, m_position.scale);
	}

	void Transform::SetRotation(float4x4 const &rot)
	{
		m_position.rotation = rot.ToQuaternion();
		m_position.rotation.Normalize();
		m_world = CalculateWorldMatrix(m_position.translation, m_position.rotation, m_position.scale);
	}

	float4 const &Transform::GetTranslation(void) const
	{
		return m_position.translation;
	}

	float4 const &Transform::GetScale(void) const
	{
		return m_position.scale;
	}

	quaternion const &Transform::GetRotation(void) const
	{
		return m_position.rotation;
	}

	float4 const &Transform::GetRightVector(void) const
	{
		return m_right;
	}

	float4 const &Transform::GetUpVector(void) const
	{
		return m_up;
	}

	float4 const &Transform::GetForwardVector(void) const
	{
		return m_forward;
	}

	// TODO: Check this cross product, I might need to reverse this expression
	void Transform::SetRightVector(float4 const &right)
	{
		float4 newUp = m_forward.Cross(right);
		float4 newForward = right.Cross(newUp);
		float4 newRight = right;

		newRight.Normalize();
		newUp.Normalize();
		newForward.Normalize();

		m_right = newRight;
		m_up = newUp;
		m_forward = newForward;

		// TODO: Make sure these are correct, should it -forward?
		SetRotation(float4x4(newRight, newUp, newForward, float4()));
	}

	// TODO: Check this cross product, I might need to reverse this expression
	void Transform::SetUpVector(float4 const &up)
	{
		float4 newRight = up.Cross(m_forward);
		float4 newForward = newRight.Cross(up);
		float4 newUp = up;

		newRight.Normalize();
		newUp.Normalize();
		newForward.Normalize();

		m_right = newRight;
		m_up = newUp;
		m_forward = newForward;

		// TODO: Make sure these are correct, should it -forward?
		SetRotation(float4x4(newRight, newUp, newForward, float4()));
	}

	// TODO: Check this cross product, I might need to reverse this expression
	void Transform::SetForwardVector(float4 const &forward)
	{
		float4 newRight = m_up.Cross(forward);
		float4 newUp = forward.Cross(newRight);
		float4 newForward = forward;

		newRight.Normalize();
		newUp.Normalize();
		newForward.Normalize();

		m_right = newRight;
		m_up = newUp;
		m_forward = newForward;

		// TODO: Make sure these are correct, should it -forward?
		SetRotation(float4x4(newRight, newUp, newForward, float4()));
	}

	void Transform::Translate(float4 const &Translation)
	{
		m_position.translation += Translation;
		m_world = CalculateWorldMatrix(m_position.translation, m_position.rotation, m_position.scale);
	}

	float4x4 Transform::GetRotationMatrix(void) const
	{
		return m_position.rotation.ToMatrix();
	}

	float4x4 const &Transform::GetWorldMatrix(void) const
	{
		return m_world;
	}

	void Transform::UpdateWorldMatrix(void)
	{
		m_world = CalculateWorldMatrix(m_position.translation, m_position.rotation, m_position.scale);
	}

	//////////////////////////////////////////////////////////////////////////
	// SERIALIZATIN ROUTINE

	// TODO: Update the transform serialization routine for new variables
	void Transform::Serialize(File& file, Variable var)
	{
		Transform* tr = var.GetValue<Transform*>();

		// Cache our local Translation, Rotation, and Scale inside for the component itself
		tr->m_position.translation = tr->GetTranslation();
		tr->m_position.rotation = tr->GetRotation();
		tr->m_position.scale = tr->GetScale();

		// Get a pointer to the serializer
		Serializer* s = Serializer::Get();

		// Store a pointer to the typeinfo for later use
		const TypeInfo* info = var.GetTypeInfo();

		// Variable for members (translation and stuff)
		Variable mem;

		// Write to the file the name of the type
		file.Write("%s\n", info->Name());

		// Increase padding
		s->Padding(file, s->GetPadLevel()++);

		// Make a bracket and newline
		file.Write("{\n");

		// Translation
		s->Padding(file, s->GetPadLevel());
		file.Write("%s ", "Translation");
		mem = Variable(tr->m_position.translation);
		mem.Serialize(file);

		// Rotation
		s->Padding(file, s->GetPadLevel());
		file.Write("%s ", "Rotation");
		mem = Variable(tr->m_position.rotation);
		mem.Serialize(file);

		// Scale
		s->Padding(file, s->GetPadLevel());
		file.Write("%s ", "Scale");
		mem = Variable(tr->m_position.scale);
		mem.Serialize(file);

		// Decrease padding level
		s->Padding(file, --s->GetPadLevel());

		// Write a bracket
		file.Write("}\n");
	}

	void Transform::Deserialize(File& file, Variable var)
	{
		Transform* tr = var.GetValue<Transform*>();
		Serializer* s = Serializer::Get();

		// Peek function can figure out the starting level
		int startLevel = s->GetPadLevel();

		std::string line = file.GetLine('{');
		char buf[32];

		// Read past Translation
		file.Read("%s", &buf);

		float4 trans;
		GET_TYPE(float4)->Deserialize(file, trans);

		// Read past Rotation
		file.Read("%s", &buf);

                quaternion rot;
                GET_TYPE(quaternion)->Deserialize(file, rot);

		// Read past Scale
		file.Read("%s", &buf);

		float4 scale;
		GET_TYPE(float4)->Deserialize(file, scale);

		tr->m_position.translation = trans;
		tr->m_position.rotation = rot;
		tr->m_position.scale = scale;

		file.GetLine('\n');
		file.GetLine('\n');
	}


	void Transform::RegisterPrivateMembers()
	{
		TYPE_ADD_MEMBER(Transform, m_position);
	}

  COMP_LUA_DEP(Transform);
}
