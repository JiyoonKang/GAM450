/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: CapsuleCollider.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "physics/colliders/CapsuleCollider.h"
#include "physics/core/PhysicsSystem.h"

namespace Physics
{
  CapsuleCollider::CapsuleCollider(void)
    : Collider()
    , m_Capsule()
  {
    m_szGuid = "{38D4A227-8EE8-4211-BF0A-474EFCBC18C1}";
  }

  CapsuleCollider::~CapsuleCollider(void)
  {

  }

  std::string CapsuleCollider::GetGuid(void)
  {
    return "{38D4A227-8EE8-4211-BF0A-474EFCBC18C1}";
  }

  void CapsuleCollider::Initialize(void)
  {
    // Register with the physics stystem
    //PhysicsSystem::RegisterCollider(self);
  }

  void CapsuleCollider::Remove(void)
  {
    //PhysicsSystem::UnregisterCollider(self);
  }

  //void CapsuleCollider::Serialize(File &file, Variable var)
  //{
  //  CapsuleCollider *pCapsule = var.GetValue<CapsuleCollider*>();
  //
  //  // get a pointer to the serializer
  //  Base::Serializer *s = Base::Serializer::Get();
  //
  //  TypeInfo const *pInfo = var.GetTypeInfo();
  //
  //  // variagle for members
  //  Variable mem;
  //
  //  // write to the file the name of the type
  //  file.Write("%s\n", pInfo->Name());
  //
  //  // increase padding
  //  s->Padding(file, s->GetPadLevel()++);
  //
  //  // make a bracket and new line
  //  file.Write("{\n");
  //
  //  // write for my capsule
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Capsule");
  //  mem = Variable(pCapsule->m_Capsule);
  //  mem.Serialize(file);
  //
  //  // done with members
  //  s->Padding(file, --s->GetPadLevel());
  //
  //  // write a bracket
  //  file.Write("}\n");
  //}
  //
  //void CapsuleCollider::Deserialize(File &file, Variable var)
  //{
  //  CapsuleCollider *pCapsule = var.GetValue<CapsuleCollider*>();
  //  Base::Serializer *s = Base::Serializer::Get();
  //
  //  // peek function to figure out the starting level?
  //  int nStartLevel = s->GetPadLevel();
  //
  //  std::string szLine = file.GetLine('{');
  //  char buffer[1024] = {0};
  //
  //  file.Read("%s", &buffer);
  //  Capsule capsule;
  //  GET_TYPE(Capsule)->Deserialize(file, capsule);
  //
  //  pCapsule->m_Capsule = capsule;
  //
  //  // not sure why these are necessary?
  //  file.GetLine('\n');
  //  file.GetLine('\n');
  //}

  COMP_LUA_DEP(CapsuleCollider);
}