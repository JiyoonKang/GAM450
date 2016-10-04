/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SphereCollider.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include <pch\precompiled.h>

#include "physics/colliders/SphereCollider.h"
#include "physics/core/PhysicsSystem.h"

namespace Physics
{
  SphereCollider::SphereCollider(void)
    : Collider()
    , m_Sphere()
  {
    m_szGuid = "{4A987C76-A314-4CEA-8096-57C5658C8FFD}";
  }

  SphereCollider::~SphereCollider(void)
  {

  }

  std::string SphereCollider::GetGuid(void)
  {
    return "{4A987C76-A314-4CEA-8096-57C5658C8FFD}";
  }

  void SphereCollider::Initialize(void)
  {
    // register with the physics system
    //PhysicsSystem::RegisterCollider(self);
  }

  void SphereCollider::Remove(void)
  {
    //PhysicsSystem::UnregisterCollider(self);
  }

  //void SphereCollider::Serialize(File &file, Variable var)
  //{
  //  SphereCollider *pSphere = var.GetValue<SphereCollider*>();
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
  //  // write for my sphere
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Sphere");
  //  mem = Variable(pSphere->m_Sphere);
  //  mem.Serialize(file);
  //
  //  // done with members
  //  s->Padding(file, --s->GetPadLevel());
  //
  //  // write a bracket
  //  file.Write("}\n");
  //}
  //
  //void SphereCollider::Deserialize(File &file, Variable var)
  //{
  //  SphereCollider *pSphere = var.GetValue<SphereCollider*>();
  //  Base::Serializer *s = Base::Serializer::Get();
  //
  //  // peek function to figure out the starting level?
  //  int nStartLevel = s->GetPadLevel();
  //
  //  std::string szLine = file.GetLine('{');
  //  char buffer[1024] = {0};
  //
  //  file.Read("%s", &buffer);
  //  Sphere sphere;
  //  GET_TYPE(Sphere)->Deserialize(file, sphere);
  //
  //  pSphere->m_Sphere = sphere;
  //
  //  // not sure why these are necessary?
  //  file.GetLine('\n');
  //  file.GetLine('\n');
  //}
}