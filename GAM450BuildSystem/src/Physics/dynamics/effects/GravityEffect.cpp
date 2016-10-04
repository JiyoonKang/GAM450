/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: GravityEffect.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include <pch\precompiled.h>
#include <Dynamics\Effects\GravityEffect.h>

namespace Physics
{
  GravityEffect::GravityEffect(void)
    : Effect(float4(0.f, -1.f, 0.f, 0.f), 9.8f)
  {
    m_szGuid = "{9E1879D0-6D94-4D60-A2AF-969CAE56BE43}";
  }

  GravityEffect::GravityEffect(float4 const &Direction, float fMagnitude)
    : Effect(Direction, fMagnitude)
  {
    m_szGuid = "{9E1879D0-6D94-4D60-A2AF-969CAE56BE43}";
  }

  GravityEffect::~GravityEffect(void)
  {

  }

  //void GravityEffect::Serialize(File &file, Variable var)
  //{
  //  GravityEffect *pEffect = var.GetValue<GravityEffect*>();
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
  //  s->Padding(file, --s->GetPadLevel());
  //
  //  file.Write("}\n");
  //}
  //
  //void GravityEffect::Deserialize(File &file, Variable var)
  //{
  //  GravityEffect *pEffect = var.GetValue<GravityEffect*>();
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
  //  pEffect->m_Direction = direction;
  //  pEffect->m_fMagnitude = magnitude;
  //  pEffect->m_bActive = bActive;
  //
  //  // not sure why these are necessary?
  //  file.GetLine('\n');
  //  file.GetLine('\n');
  //}
}