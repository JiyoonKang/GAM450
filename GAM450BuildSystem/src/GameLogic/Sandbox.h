/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Sandbox.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Core/systems/System.h"
#include <iostream>

class Sandbox : public Core::ISystem
{
public:
  Sandbox() {}
  ~Sandbox() {}

  virtual void Initialize(void);
  virtual void ReceiveMessage(Core::Message& msg);
  virtual void Update(float dt);
  virtual void RegisterComponents(void);

  virtual std::string GetName() const { return "Sandbox"; }

private:

};

//////////////////////////////////////////////////////////////////////////
// Luabridge test

class Object
{
public:
  Object() : value(0), name(""), a(0) {}

  Object(std::string s) : a(0)
  {
    name = s;
  }

  ~Object() {}

  std::string GetName() const
  {
    return name;
  }

  void SetName(std::string s)
  {
    name = s;
  }

  void PrintTest()
  {
    printf("THIS IS A TEST\n");
  }

  void SetNum(int num)
  {
    std::cout << "Set to " << num << std::endl;
    a = num;
  }

  int GetNum() const
  {
    return a;
  }

  int value;

private:
  std::string name;
  int a;
};