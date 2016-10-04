/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: BoxCollider.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "physics/colliders/BoxCollider.h"
#include "physics/core/PhysicsSystem.h"

namespace Physics
{
  BoxCollider::BoxCollider(void)
    : Collider()
    , m_Box()
  {
    m_szGuid = "{26D8B9CA-DE8F-4976-93A8-A741F2637F0B}";
  }

  BoxCollider::~BoxCollider(void)
  {

  }

  std::string BoxCollider::GetGuid(void)
  {
    return "{26D8B9CA-DE8F-4976-93A8-A741F2637F0B}";
  }

  void BoxCollider::Initialize(void)
  {
    // if this is called we can register with the physics system
    //PhysicsSystem::RegisterCollider(self);
  }

  void BoxCollider::Remove(void)
  {
    //PhysicsSystem::UnregisterCollider(self);
  }

  //void BoxCollider::Serialize(File &file, Variable var)
  //{
  //  BoxCollider *pBox = var.GetValue<BoxCollider*>();
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
  //  // write to my file 
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Box");
  //  mem = Variable(pBox->m_Box);
  //  mem.Serialize(file);
  //
  //  // done with members
  //  s->Padding(file, --s->GetPadLevel());
  //
  //  // write a bracket
  //  file.Write("}\n");
  //}
  //
  //void BoxCollider::Deserialize(File &file, Variable var)
  //{
  //  BoxCollider *pBox = var.GetValue<BoxCollider*>();
  //  Base::Serializer *s = Base::Serializer::Get();
  //
  //  // peek function to figure out the starting level?
  //  int nStartLevel = s->GetPadLevel();
  //
  //  std::string szLine = file.GetLine('{');
  //  char buffer[1024] = {0};
  //
  //  file.Read("%s", &buffer);
  //  Box box;
  //  GET_TYPE(Box)->Deserialize(file, box);
  //
  //  pBox->m_Box = box;
  //
  //  // not sure why these are necessary?
  //  file.GetLine('\n');
  //  file.GetLine('\n');
  //}

  COMP_LUA_DEP(BoxCollider);
}