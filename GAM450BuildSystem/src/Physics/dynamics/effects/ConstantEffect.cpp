/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ConstantEffect.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "dynamics/effects/ConstantEffect.h"

namespace Physics
{
  ConstantEffect::ConstantEffect(void)
    : Effect()
    , m_TorqueDirection()
    , m_fTorqueMagnitude(0.f)
  {
    m_szGuid = "{292F5F0E-BA7D-4922-8763-72FF9FDFADCF}";
  }

  ConstantEffect::ConstantEffect(float4 const &Direction, float fMagnitude)
    : Effect(Direction, fMagnitude)
    , m_TorqueDirection()
    , m_fTorqueMagnitude(0.f)
  {
    m_szGuid = "{292F5F0E-BA7D-4922-8763-72FF9FDFADCF}";
  }

  ConstantEffect::~ConstantEffect(void)
  {
  }

  float4 ConstantEffect::GetTotalTorque(void) const
  {
    return m_TorqueDirection * m_fTorqueMagnitude;
  }

  float4 const &ConstantEffect::GetTorqueDirection(void) const
  {
    return m_TorqueDirection;
  }

  void ConstantEffect::SetTorqueDirection(float4 const &Direction)
  {
    m_TorqueDirection = Direction;
  }

  float ConstantEffect::GetTorqueMagnitude(void) const
  {
    return m_fTorqueMagnitude;
  }

  void ConstantEffect::SetTorqueMagnitude(float fMagnitude)
  {
    m_fTorqueMagnitude = fMagnitude;
  }

  //void ConstantEffect::Serialize(File &file, Variable var)
  //{
  //  ConstantEffect *pEffect = var.GetValue<ConstantEffect*>();
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
  //  // torque direction
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Angular Direction");
  //  mem = Variable(pEffect->m_TorqueDirection);
  //  mem.Serialize(file);
  //
  //  // torque magnitude
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Angular Torque");
  //  mem = Variable(pEffect->m_fTorqueMagnitude);
  //  mem.Serialize(file);
  //
  //  // done with members
  //  s->Padding(file, --s->GetPadLevel());
  //
  //  // write a bracket
  //  file.Write("}\n");
  //}
  //
  //void ConstantEffect::Deserialize(File &file, Variable var)
  //{
  //  ConstantEffect *pEffect = var.GetValue<ConstantEffect*>();
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
  //  // angular direction
  //  file.Read("%s", &buffer);
  //  float4 torqueDirection;
  //  GET_TYPE(float4)->Deserialize(file, torqueDirection);
  //
  //  // angular magnitude
  //  file.Read("%s", &buffer);
  //  float torqueMagnitude = 0.f;
  //  GET_TYPE(float)->Deserialize(file, torqueMagnitude);
  //
  //  // set my members
  //  pEffect->m_Direction = direction;
  //  pEffect->m_fMagnitude = magnitude;
  //  pEffect->m_bActive = bActive;
  //  pEffect->m_TorqueDirection = torqueDirection;
  //  pEffect->m_fTorqueMagnitude = torqueMagnitude;
  //
  //  // not sure why these are necessary?
  //  file.GetLine('\n');
  //  file.GetLine('\n');
  //}
}