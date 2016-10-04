/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DragEffect.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include <pch\precompiled.h>
#include <dynamics\effects\DragEffect.h>
#include <dynamics\RigidBody.h>
#include <util\Geometry.h>

namespace Physics
{
  DragEffect::DragEffect(void)
    : Effect()
    , m_fBTerm(1.f)
  {
    m_szGuid = "{07CD40F9-3D34-4412-B70C-8E48D4F48778}";
  }

  DragEffect::~DragEffect(void)
  {

  }

  void DragEffect::UpdateForce(RigidBody *pBody)
  {
    float4 Velocity = pBody->GetVelocity();
    float fSpeed = Velocity.Length();

    SetMagnitude(m_fBTerm * fSpeed);
    SetDirection(-Velocity.Normal());
  }

  float DragEffect::GetDampingFactor(void) const
  {
    return m_fBTerm;
  }

  void DragEffect::SetDampingFactor(float fBTerm)
  {
    fBTerm = Clamp(fBTerm, 0.f, 15.f);

    m_fBTerm = fBTerm;
  }

  //void DragEffect::Serialize(File &file, Variable var)
  //{
  //  DragEffect *pEffect = var.GetValue<DragEffect*>();
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
  //  // direction
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Linear Direction");
  //  mem = Variable(pEffect->m_Direction);
  //  mem.Serialize(file);
  //
  //  // magnitude
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Linear Magnitude");
  //  mem = Variable(pEffect->m_fMagnitude);
  //  mem.Serialize(file);
  //
  //  // active
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Active");
  //  mem = Variable(pEffect->m_bActive);
  //  mem.Serialize(file);
  //
  //  // b term
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Drag Coefficient");
  //  mem = Variable(pEffect->m_fBTerm);
  //  mem.Serialize(file);
  //
  //  s->Padding(file, --s->GetPadLevel());
  //
  //  file.Write("}\n");
  //}
  //
  //void DragEffect::Deserialize(File &file, Variable var)
  //{
  //  DragEffect *pEffect = var.GetValue<DragEffect*>();
  //  Base::Serializer *s = Base::Serializer::Get();
  //
  //  // peek function to figure out the starting level?
  //  int nStartLevel = s->GetPadLevel();
  //
  //  std::string szLine = file.GetLine('{');
  //  char buffer[1024] = {0};
  //
  //  // linear direction
  //  file.Read("%s", &buffer);
  //  float4 direction;
  //  GET_TYPE(float4)->Deserialize(file, direction);
  //
  //  // linear magnitude
  //  file.Read("%s", &buffer);
  //  float magnitude = 0.f;
  //  GET_TYPE(float)->Deserialize(file, magnitude);
  //
  //  // active
  //  file.Read("%s", &buffer);
  //  bool bActive = false;
  //  GET_TYPE(bool)->Deserialize(file, bActive);
  //
  //  // b term
  //  file.Read("%s", &buffer);
  //  float bTerm = 0.f;
  //  GET_TYPE(float)->Deserialize(file, bTerm);
  //
  //  pEffect->m_Direction = direction;
  //  pEffect->m_fMagnitude = magnitude;
  //  pEffect->m_bActive = bActive;
  //  pEffect->m_fBTerm = bTerm;
  //
  //  // not sure why these are necessary?
  //  file.GetLine('\n');
  //  file.GetLine('\n');
  //}
}