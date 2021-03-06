/*******************************************************************************
 * All content � 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Sandbox.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Sandbox.h"
#include "Core/systems/input/Input.h"
#include "Base/util/debug/DebugAssist.h"
#include "Core/types/space/Space.h"
#include "Core/engine/core/Engine.h"
#include "Core/components/transform/CTransform.h"
#include "Audio/components/emitter/SoundEmitter.h"
#include "Core/types/object/Object.h"
#include "Graphics/includes/GraphicsSystem/Renderable.h"
#include "Graphics/includes/GraphicsSystem/Paintable.h"
#include "base/util/math/MathUtil.h"

#include "physics/colliders/BoxCollider.h"
#include "Physics/colliders/CapsuleCollider.h"
#include "Physics/colliders/SphereCollider.h"
#include "Physics/dynamics/RigidBody.h"
#include "physics/dynamics/effects/GravityEffect.h"
#include "physics/dynamics/effects/DragEffect.h"

#include "GameLogic/PlayerController.h"
#include "GameLogic/Timer.h"
#include "core/components/base/Component.h"

#include "Lua/LuaInterface.h"

#define KEY_NAME(name) #name
static const char* s_KeyNames[Input::VirtualKey::NUM_KEY_CODES] = { "Unused" };

#define DEFAULT_NORMAL_TEXTURE "6670-normal.jpg"
#define DEFAULT_TEXTURE "6670-diffuse.jpg"

namespace
{
  Core::GameSpace *g_GameSpace = nullptr;

  Core::GameObject *g_Player1 = nullptr;
  Core::GameObject *g_Player2 = nullptr;
  Core::GameObject *g_Player3 = nullptr;
  Core::GameObject *g_Player4 = nullptr;

  class ObjectBuilder
  {
    std::string m_szName;
    Core::GameObject *m_pCurrObject;
  
  public:
    ObjectBuilder(void) 
    : m_pCurrObject(nullptr)
    {
    }

    ObjectBuilder &CreateObject(std::string const &szName = "")
    {
      m_pCurrObject = g_GameSpace->CreateEmptyObject();
      m_szName = szName;
      return *this;
    }

    Core::GameObject *Finish(void)
    {
      Core::GameObject *pTemp = m_pCurrObject;
      m_pCurrObject = nullptr;
      pTemp->Initialize();
      pTemp->name = m_szName;
      return pTemp;
    }

    ObjectBuilder &AddTransform(
      float4 const &position = float4(0.f, 0.f, 0.f, 0.f),
      float4 const &scale = float4(1.f, 1.f, 1.f, 0.f),
      quaternion const &orientation = quaternion(1.f, 0.f, 0.f, 0.f))
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Core::Transform *pTransform = static_cast<Core::Transform*>(g_GameSpace->CreateComponent(eTransform));
      m_pCurrObject->AddComponent(pTransform);
      pTransform->SetTranslation(position);
      pTransform->SetRotation(orientation);
      pTransform->SetScale(scale);

      return *this;
    }

    ObjectBuilder &AddBoxCollider(float4 const &extents = float4(1.f, 1.f, 1.f, 0.f))
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Physics::BoxCollider *pBox = static_cast<Physics::BoxCollider*>(g_GameSpace->CreateComponent(eBoxCollider));
      m_pCurrObject->AddComponent(pBox);
      pBox->SetExtents(extents);
      return *this;
    }

    ObjectBuilder &AddSphereCollider(float radius = 0.5f)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Physics::SphereCollider *pSphere = static_cast<Physics::SphereCollider*>(g_GameSpace->CreateComponent(eSphereCollider));
      m_pCurrObject->AddComponent(pSphere);
      pSphere->SetRadius(radius);
      return *this;
    }

    ObjectBuilder &AddCapsuleCollider(float height = 1.f, float radius = 0.5f)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Physics::CapsuleCollider *pCapsule = static_cast<Physics::CapsuleCollider*>(g_GameSpace->CreateComponent(eCapsuleCollider));
      m_pCurrObject->AddComponent(pCapsule);
      pCapsule->SetHeight(height);
      pCapsule->SetRadius(radius);
      return *this;
    }

    ObjectBuilder &AddRenderable(std::string const &szModel, std::string const &texture, std::string const normalTexture)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Renderable *pRender = static_cast<Renderable*>(g_GameSpace->CreateComponent(eRenderable));
      m_pCurrObject->AddComponent(pRender);
      pRender->SetModel(szModel);
      pRender->SetTexture(texture);
      pRender->SetNormalTexture(normalTexture);
      return *this;
    }

    ObjectBuilder &AddRigidBody(Physics::DynamicState state = Physics::DynamicState::DYNAMIC)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Physics::RigidBody *pBody = static_cast<Physics::RigidBody*>(g_GameSpace->CreateComponent(eRigidBody));
      m_pCurrObject->AddComponent(pBody);
      pBody->SetState(state);
      return *this;
    }

    ObjectBuilder &AddGravityEffect(float4 const &direction = float4(0.f, -1.f, 0.f, 0.f),
      float fMag = 9.81f)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Physics::GravityEffect *pGravity = static_cast<Physics::GravityEffect*>(g_GameSpace->CreateComponent(eGravityEffect));
      m_pCurrObject->AddComponent(pGravity);
      pGravity->SetDirection(direction);
      pGravity->SetMagnitude(fMag);
      return *this;
    }

    ObjectBuilder &AddDragEffect(float b = 1.f)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Physics::DragEffect *pDrag = static_cast<Physics::DragEffect*>(g_GameSpace->CreateComponent(eDragEffect));
      m_pCurrObject->AddComponent(pDrag);
      pDrag->SetDampingFactor(b);
      return *this;
    }

    ObjectBuilder &AddSoundEmitter(void)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      Audio::SoundEmitter *pEmitter = static_cast<Audio::SoundEmitter*>(g_GameSpace->CreateComponent(eSoundEmitter));
      m_pCurrObject->AddComponent(pEmitter);
      return *this;
    }

    ObjectBuilder &AddPlayerController(unsigned nGamePad)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      GameLogic::PlayerController *pController = static_cast<GameLogic::PlayerController*>(g_GameSpace->CreateComponent(ePlayerController));
      pController->SetGamePad(nGamePad);
      m_pCurrObject->AddComponent(pController);
      return *this;
    }
    ObjectBuilder &AddTimer(float startTime)
    {
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject)
        return *this;

      GameLogic::Timer *pTimer = static_cast<GameLogic::Timer*>(g_GameSpace->CreateComponent(eTimer));
      pTimer->SetTime(startTime);
      m_pCurrObject->AddComponent(pTimer);
      return *this;
    }
  };

  void SampleCreationCode(void)
  {
    if (!g_GameSpace)
      return;

    // Create an empty object
    Core::GameObject* testobject = g_GameSpace->CreateEmptyObject();

    // Attach components
    Core::Transform* transformComp = (Core::Transform*)g_GameSpace->CreateComponent(eTransform);
    testobject->AddComponent(transformComp);

    Audio::SoundEmitter* emitter = (Audio::SoundEmitter*)g_GameSpace->CreateComponent(eSoundEmitter);
    testobject->AddComponent(emitter);


    //testobject->Initialize();

    //testobject->GetComponent<Audio::SoundEmitter>(eSoundEmitter)->Play("Music/Menu.Music");

    //CREATE GRID
    Core::GameObject* grid = g_GameSpace->CreateEmptyObject();
    Core::Transform* gridTransform = (Core::Transform*)g_GameSpace->CreateComponent(eTransform);
    gridTransform->SetTranslation(float4(0, -20, 200, 0));
    gridTransform->SetScale(float4(250, 10, 250, 0));
    gridTransform->SetRotation(quaternion(float4(-1.f, 0.f, 0.f, 0.f), 15.f * DEG_TO_RAD));
    // the rigidbody and collider of this floor
    Physics::RigidBody *gridBody = (Physics::RigidBody *)g_GameSpace->CreateComponent(eRigidBody);
    gridBody->SetState(Physics::DynamicState::STATIC);
    Physics::BoxCollider *gridBox = (Physics::BoxCollider *)g_GameSpace->CreateComponent(eBoxCollider);
    gridBox->SetExtents(float4(250, 10, 250, 0));

    grid->AddComponent(gridTransform);
    grid->AddComponent(gridBody);
    grid->AddComponent(gridBox);

    Renderable* gridRender = (Renderable*)g_GameSpace->CreateComponent(eRenderable);
    gridRender->SetModel("Cube");
    gridRender->SetTexture("6670-diffuse.jpg");
    gridRender->SetNormalTexture("6670-normal.jpg");
    grid->AddComponent(gridRender);
    grid->Initialize();

    //debug sphere object
    Core::GameObject* sphere = g_GameSpace->CreateEmptyObject();
    Core::Transform* sphereTransform = (Core::Transform*)g_GameSpace->CreateComponent(eTransform);
    sphereTransform->SetTranslation(float4(0, 20, 150, 0));
    sphereTransform->SetScale(float4(5, 5, 5, 0));

    Renderable* debugSphere = (Renderable*)g_GameSpace->CreateComponent(eRenderable);
    debugSphere->SetModel("Sphere");
    debugSphere->SetTexture("6670-diffuse.jpg");
    debugSphere->SetNormalTexture("6670-normal.jpg");

    sphere->AddComponent(sphereTransform);
    sphere->AddComponent(debugSphere);
    sphere->Initialize();


    Core::GameObject* cube = g_GameSpace->CreateEmptyObject();
    Core::Transform* cubeTransform = (Core::Transform*)g_GameSpace->CreateComponent(eTransform);
    cubeTransform->SetTranslation(float4(0, 0, 150, 0));
    cubeTransform->SetScale(float4(5, 5, 5, 0));
    cubeTransform->SetRotation(quaternion(float4(1.0f, 1.0f, 0.0f, 0.0f), 45.0f * DEG_TO_RAD));

    Renderable* debugCube = (Renderable*)g_GameSpace->CreateComponent(eRenderable);
    debugCube->SetModel("Cube");
    debugCube->SetTexture("6670-diffuse.jpg");
    debugCube->SetNormalTexture("6670-normal.jpg");

    cube->AddComponent(cubeTransform);
    cube->AddComponent(debugCube);
    cube->Initialize();

    float fAngle = 0.f;

    //int i = 0;
    for (int i = 0; i < 10; ++i)
    {
      Core::GameObject* object = g_GameSpace->CreateEmptyObject();
      Core::Transform* transform = (Core::Transform*)g_GameSpace->CreateComponent(eTransform);
      transform->SetTranslation(float4(cos((i * 2 * PI) / 10) * 50, 10, sin((i * 2 * PI) / 10) * 50, 0));
      transform->Translate(float4(0.f, 0.f, 200.f, 0.f));
      transform->SetScale(float4(5, 5, 5, 0));

      // this is a cube
      if (i % 3 == 0)
        transform->SetRotation(quaternion(float4(1.f, 0.f, 0.f, 0.f), 30.f * DEG_TO_RAD));
      // this is a capsule
      if (i % 3 == 1)
        transform->SetRotation(quaternion(float4(0.f, 0.f, 1.f, 0.f), fAngle * DEG_TO_RAD));
      // do nothing for a sphere

      object->AddComponent(transform);

      Renderable* objectRender = (Renderable*)g_GameSpace->CreateComponent(eRenderable);

      // this is a cube
      if (i % 3 == 0)
        objectRender->SetModel("Cube");
      // this is a capsule, use a cylinder for now
      else if (i % 3 == 1)
        objectRender->SetModel("Cylinder");
      // this is a sphere
      else
        objectRender->SetModel("Sphere");

      objectRender->SetTexture("6670-diffuse.jpg");
      objectRender->SetNormalTexture("6670-normal.jpg");
      object->AddComponent(objectRender);

      // adding rigidbodies and gravity effects
      Physics::RigidBody *body = (Physics::RigidBody *)g_GameSpace->CreateComponent(eRigidBody);
      Physics::GravityEffect *gravity = (Physics::GravityEffect*)g_GameSpace->CreateComponent(eGravityEffect);

      object->AddComponent(body);
      object->AddComponent(gravity);

      // adding colliders
      if (i % 3 == 0)
      {
        Physics::BoxCollider *box = (Physics::BoxCollider*)g_GameSpace->CreateComponent(eBoxCollider);
        box->SetExtents(float4(5, 5, 5, 0));
        object->AddComponent(box);
      }
      else if (i % 3 == 1)
      {
        Physics::CapsuleCollider *capsule = (Physics::CapsuleCollider*)g_GameSpace->CreateComponent(eCapsuleCollider);
        capsule->SetRadius(2.5f);
        capsule->SetHeight(5.f);
        object->AddComponent(capsule);
      }
      else
      {
        Physics::SphereCollider *sphere = (Physics::SphereCollider*)g_GameSpace->CreateComponent(eSphereCollider);
        sphere->SetRadius(5.f);
        object->AddComponent(sphere);
      }

      object->Initialize();

      fAngle += 18.5f;
    }
  }

  void InitializeKeyStringArray(void)
  {
    for (int i = 0; i < Input::VirtualKey::NUM_KEY_CODES; ++i)
    {
      s_KeyNames[i] = "Unused";
    }

    s_KeyNames[Input::VirtualKey::KEY_NULL] = KEY_NAME(KEY_NULL);
    s_KeyNames[Input::VirtualKey::KEY_SPACE] = KEY_NAME(KEY_SPACE);
    s_KeyNames[Input::VirtualKey::KEY_APOSTROPHE] = KEY_NAME(KEY_APOSTROPHE);
    s_KeyNames[Input::VirtualKey::KEY_COMMA] = KEY_NAME(KEY_COMMA);
    s_KeyNames[Input::VirtualKey::KEY_MINUS] = KEY_NAME(KEY_MINUS);
    s_KeyNames[Input::VirtualKey::KEY_PERIOD] = KEY_NAME(KEY_PERIOD);
    s_KeyNames[Input::VirtualKey::KEY_SLASH] = KEY_NAME(KEY_SLASH);
    s_KeyNames[Input::VirtualKey::KEY_0] = KEY_NAME(KEY_0);
    s_KeyNames[Input::VirtualKey::KEY_1] = KEY_NAME(KEY_1);
    s_KeyNames[Input::VirtualKey::KEY_2] = KEY_NAME(KEY_2);
    s_KeyNames[Input::VirtualKey::KEY_3] = KEY_NAME(KEY_3);
    s_KeyNames[Input::VirtualKey::KEY_4] = KEY_NAME(KEY_4);
    s_KeyNames[Input::VirtualKey::KEY_5] = KEY_NAME(KEY_5);
    s_KeyNames[Input::VirtualKey::KEY_6] = KEY_NAME(KEY_6);
    s_KeyNames[Input::VirtualKey::KEY_7] = KEY_NAME(KEY_7);
    s_KeyNames[Input::VirtualKey::KEY_8] = KEY_NAME(KEY_8);
    s_KeyNames[Input::VirtualKey::KEY_9] = KEY_NAME(KEY_9);
    s_KeyNames[Input::VirtualKey::KEY_SEMICOLON] = KEY_NAME(KEY_SEMICOLON);
    s_KeyNames[Input::VirtualKey::KEY_EQUAL] = KEY_NAME(KEY_EQUAL);
    s_KeyNames[Input::VirtualKey::KEY_A] = KEY_NAME(KEY_A);
    s_KeyNames[Input::VirtualKey::KEY_B] = KEY_NAME(KEY_B);
    s_KeyNames[Input::VirtualKey::KEY_C] = KEY_NAME(KEY_C);
    s_KeyNames[Input::VirtualKey::KEY_D] = KEY_NAME(KEY_D);
    s_KeyNames[Input::VirtualKey::KEY_E] = KEY_NAME(KEY_E);
    s_KeyNames[Input::VirtualKey::KEY_F] = KEY_NAME(KEY_F);
    s_KeyNames[Input::VirtualKey::KEY_G] = KEY_NAME(KEY_G);
    s_KeyNames[Input::VirtualKey::KEY_H] = KEY_NAME(KEY_H);
    s_KeyNames[Input::VirtualKey::KEY_I] = KEY_NAME(KEY_I);
    s_KeyNames[Input::VirtualKey::KEY_J] = KEY_NAME(KEY_J);
    s_KeyNames[Input::VirtualKey::KEY_K] = KEY_NAME(KEY_K);
    s_KeyNames[Input::VirtualKey::KEY_L] = KEY_NAME(KEY_L);
    s_KeyNames[Input::VirtualKey::KEY_M] = KEY_NAME(KEY_M);
    s_KeyNames[Input::VirtualKey::KEY_N] = KEY_NAME(KEY_N);
    s_KeyNames[Input::VirtualKey::KEY_O] = KEY_NAME(KEY_O);
    s_KeyNames[Input::VirtualKey::KEY_P] = KEY_NAME(KEY_P);
    s_KeyNames[Input::VirtualKey::KEY_Q] = KEY_NAME(KEY_Q);
    s_KeyNames[Input::VirtualKey::KEY_R] = KEY_NAME(KEY_R);
    s_KeyNames[Input::VirtualKey::KEY_S] = KEY_NAME(KEY_S);
    s_KeyNames[Input::VirtualKey::KEY_T] = KEY_NAME(KEY_T);
    s_KeyNames[Input::VirtualKey::KEY_U] = KEY_NAME(KEY_U);
    s_KeyNames[Input::VirtualKey::KEY_V] = KEY_NAME(KEY_V);
    s_KeyNames[Input::VirtualKey::KEY_W] = KEY_NAME(KEY_W);
    s_KeyNames[Input::VirtualKey::KEY_X] = KEY_NAME(KEY_X);
    s_KeyNames[Input::VirtualKey::KEY_Y] = KEY_NAME(KEY_Y);
    s_KeyNames[Input::VirtualKey::KEY_Z] = KEY_NAME(KEY_Z);
    s_KeyNames[Input::VirtualKey::KEY_LEFT_BRACKET] = KEY_NAME(KEY_LEFT_BRACKET);
    s_KeyNames[Input::VirtualKey::KEY_BACKSLASH] = KEY_NAME(KEY_BACKSLASH);
    s_KeyNames[Input::VirtualKey::KEY_RIGHT_BRACKET] = KEY_NAME(KEY_RIGHT_BRACKET);
    s_KeyNames[Input::VirtualKey::KEY_GRAVE_ACCENT] = KEY_NAME(KEY_GRAVE_ACCENT);
    s_KeyNames[Input::VirtualKey::KEY_ESCAPE] = KEY_NAME(KEY_ESCAPE);
    s_KeyNames[Input::VirtualKey::KEY_ENTER] = KEY_NAME(KEY_ENTER);
    s_KeyNames[Input::VirtualKey::KEY_TAB] = KEY_NAME(KEY_TAB);
    s_KeyNames[Input::VirtualKey::KEY_BACKSPACE] = KEY_NAME(KEY_BACKSPACE);
    s_KeyNames[Input::VirtualKey::KEY_INSERT] = KEY_NAME(KEY_INSERT);
    s_KeyNames[Input::VirtualKey::KEY_DELETE] = KEY_NAME(KEY_DELETE);
    s_KeyNames[Input::VirtualKey::KEY_RIGHT] = KEY_NAME(KEY_RIGHT);
    s_KeyNames[Input::VirtualKey::KEY_LEFT] = KEY_NAME(KEY_LEFT);
    s_KeyNames[Input::VirtualKey::KEY_DOWN] = KEY_NAME(KEY_DOWN);
    s_KeyNames[Input::VirtualKey::KEY_UP] = KEY_NAME(KEY_UP);
    s_KeyNames[Input::VirtualKey::KEY_PAGE_UP] = KEY_NAME(KEY_PAGE_UP);
    s_KeyNames[Input::VirtualKey::KEY_PAGE_DOWN] = KEY_NAME(KEY_PAGE_DOWN);
    s_KeyNames[Input::VirtualKey::KEY_HOME] = KEY_NAME(KEY_HOME);
    s_KeyNames[Input::VirtualKey::KEY_END] = KEY_NAME(KEY_END);
    s_KeyNames[Input::VirtualKey::KEY_CAPS_LOCK] = KEY_NAME(KEY_CAPS_LOCK);
    s_KeyNames[Input::VirtualKey::KEY_SCROLL_LOCK] = KEY_NAME(KEY_SCROLL_LOCK);
    s_KeyNames[Input::VirtualKey::KEY_NUM_LOCK] = KEY_NAME(KEY_NUM_LOCK);
    s_KeyNames[Input::VirtualKey::KEY_PRINT_SCREEN] = KEY_NAME(KEY_PRINT_SCREEN);
    s_KeyNames[Input::VirtualKey::KEY_PAUSE] = KEY_NAME(KEY_PAUSE);
    s_KeyNames[Input::VirtualKey::KEY_F1] = KEY_NAME(KEY_F1);
    s_KeyNames[Input::VirtualKey::KEY_F2] = KEY_NAME(KEY_F2);
    s_KeyNames[Input::VirtualKey::KEY_F3] = KEY_NAME(KEY_F3);
    s_KeyNames[Input::VirtualKey::KEY_F4] = KEY_NAME(KEY_F4);
    s_KeyNames[Input::VirtualKey::KEY_F5] = KEY_NAME(KEY_F5);
    s_KeyNames[Input::VirtualKey::KEY_F6] = KEY_NAME(KEY_F6);
    s_KeyNames[Input::VirtualKey::KEY_F7] = KEY_NAME(KEY_F7);
    s_KeyNames[Input::VirtualKey::KEY_F8] = KEY_NAME(KEY_F8);
    s_KeyNames[Input::VirtualKey::KEY_F9] = KEY_NAME(KEY_F9);
    s_KeyNames[Input::VirtualKey::KEY_F10] = KEY_NAME(KEY_F10);
    s_KeyNames[Input::VirtualKey::KEY_F11] = KEY_NAME(KEY_F11);
    s_KeyNames[Input::VirtualKey::KEY_F12] = KEY_NAME(KEY_F12);
    s_KeyNames[Input::VirtualKey::KEY_F13] = KEY_NAME(KEY_F13);
    s_KeyNames[Input::VirtualKey::KEY_F14] = KEY_NAME(KEY_F14);
    s_KeyNames[Input::VirtualKey::KEY_F15] = KEY_NAME(KEY_F15);
    s_KeyNames[Input::VirtualKey::KEY_F16] = KEY_NAME(KEY_F16);
    s_KeyNames[Input::VirtualKey::KEY_F17] = KEY_NAME(KEY_F17);
    s_KeyNames[Input::VirtualKey::KEY_F18] = KEY_NAME(KEY_F18);
    s_KeyNames[Input::VirtualKey::KEY_F19] = KEY_NAME(KEY_F19);
    s_KeyNames[Input::VirtualKey::KEY_F20] = KEY_NAME(KEY_F20);
    s_KeyNames[Input::VirtualKey::KEY_F21] = KEY_NAME(KEY_F21);
    s_KeyNames[Input::VirtualKey::KEY_F22] = KEY_NAME(KEY_F22);
    s_KeyNames[Input::VirtualKey::KEY_F23] = KEY_NAME(KEY_F23);
    s_KeyNames[Input::VirtualKey::KEY_F24] = KEY_NAME(KEY_F24);
    s_KeyNames[Input::VirtualKey::KEY_F25] = KEY_NAME(KEY_F25);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_0] = KEY_NAME(KEY_KEYPAD_0);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_1] = KEY_NAME(KEY_KEYPAD_1);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_2] = KEY_NAME(KEY_KEYPAD_2);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_3] = KEY_NAME(KEY_KEYPAD_3);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_4] = KEY_NAME(KEY_KEYPAD_4);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_5] = KEY_NAME(KEY_KEYPAD_5);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_6] = KEY_NAME(KEY_KEYPAD_6);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_7] = KEY_NAME(KEY_KEYPAD_7);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_8] = KEY_NAME(KEY_KEYPAD_8);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_9] = KEY_NAME(KEY_KEYPAD_9);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_DECIMAL] = KEY_NAME(KEY_KEYPAD_DECIMAL);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_DIVIDE] = KEY_NAME(KEY_KEYPAD_DIVIDE);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_MULTIPLY] = KEY_NAME(KEY_KEYAD_MULTIPLY);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_SUBTRACT] = KEY_NAME(KEY_KEYAD_SUBTRACT);
    s_KeyNames[Input::VirtualKey::KEY_KEYPAD_ADD] = KEY_NAME(KEY_KEYPAD_ADD);
    s_KeyNames[Input::VirtualKey::KEY_LEFT_SHIFT] = KEY_NAME(KEY_LEFT_SHIFT);
    s_KeyNames[Input::VirtualKey::KEY_LEFT_CONTROL] = KEY_NAME(KEY_LEFT_CONTROL);
    s_KeyNames[Input::VirtualKey::KEY_LEFT_ALT] = KEY_NAME(KEY_LEFT_ALT);
    s_KeyNames[Input::VirtualKey::KEY_RIGHT_SHIFT] = KEY_NAME(KEY_RIGHT_SHIFT);
    s_KeyNames[Input::VirtualKey::KEY_RIGHT_CONTROL] = KEY_NAME(KEY_RIGHT_CONTROL);
    s_KeyNames[Input::VirtualKey::KEY_RIGHT_ALT] = KEY_NAME(KEY_RIGHT_ALT);
    s_KeyNames[Input::VirtualKey::KEY_MENU] = KEY_NAME(KEY_MENU);
  }

  // Took from Jacque's engine to test
  void TestInputKeys(void)
  {
    for (int i = 5; i < Input::VirtualKey::NUM_KEY_CODES; ++i)
    {
      const char* currentKey = s_KeyNames[i];
      if (Input::KeyboardEvent::IsKeyPressed(i))
      {
        PrintOut("Key with code %s was pressed", currentKey);
      }

      if (Input::KeyboardEvent::IsKeyHeld(i))
      {
        PrintOut("Key with code %s is being held", currentKey);
      }

      if (Input::KeyboardEvent::IsKeyReleased(i))
      {
        PrintOut("Key with code %s was released", currentKey);
      }
    }
  }

  void SampleUpdate(void)
  {
    TestInputKeys();

	  Audio::SoundInstance instance;
	  if ((Input::KeyboardEvent::IsKeyPressed(Input::VirtualKey::KEY_1)))
	  {
      if (!Audio::AUDIOMANAGER->GetAllMute())
      {
        Audio::AUDIOMANAGER->PlayEvent("MenuHover", &instance);
      }
	  }
  }
}

void Sandbox::Initialize(void)
{
  InitializeKeyStringArray();
  int leveSelect = 1;
  //testspace.hooks.Call("LogicUpdate", 0.0167f);

  // Create a Space
  if (!g_GameSpace)
    g_GameSpace = Core::ENGINE->CreateSpace("Segment Prototype");

  ObjectBuilder builder;
  Core::GameObject *pAnchor = builder.CreateObject()
    .AddTransform(float4(0.f, 30.f, 0.f, 0.f))
    .AddTimer(90.f)
    .Finish();

  pAnchor->SetName("pAnchor");
  GameLogic::Timer* pPlayerGrabber = (GameLogic::Timer*)pAnchor->GetComponent("Timer");
  if (leveSelect == 0)  //Just a large box
  {
    
	  g_Player1 = builder.CreateObject()
		  .AddTransform(float4(40.f, -5.f, 40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(0)
		  .Finish();

    g_Player1->SetName("Player1");
    pPlayerGrabber->thePlayers.push_back(g_Player1);

    g_Player2 = builder.CreateObject()
		  .AddTransform(float4(-40.f, -5.f, 40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(1)
		  .Finish();

    g_Player2->SetName("Player2");
    pPlayerGrabber->thePlayers.push_back(g_Player2);

    g_Player3 = builder.CreateObject()
		  .AddTransform(float4(40.f, -5.f, -40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(2)
		  .Finish();

    pPlayerGrabber->thePlayers.push_back(g_Player3);

    g_Player4 = builder.CreateObject()
		  .AddTransform(float4(-40.f, -5.f, -40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(3)
		  .Finish();

    pPlayerGrabber->thePlayers.push_back(g_Player4);

	  Core::GameObject *pFloor = builder.CreateObject()
		  .AddTransform(float4(0.f, -10.f, 0.f, 0.f), float4(100.f, 1.f, 100.f, 0.f))
		  .AddBoxCollider(float4(100.f, 1.f, 100.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();
      

	  Core::GameObject *pWallRight = builder.CreateObject()
		  .AddTransform(float4(50.1f, 40.1f, 0.f, 0.f), float4(1.f, 100.f, 100.f, 0.f))
		  .AddBoxCollider(float4(1.f, 100.f, 100.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pWallLeft = builder.CreateObject()
		  .AddTransform(float4(-50.1f, 40.1f, 0.f, 0.f), float4(1.f, 100.f, 100.f, 0.f))
		  .AddBoxCollider(float4(1.f, 100.f, 100.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pWallFront = builder.CreateObject()
		  .AddTransform(float4(0.f, 40.1f, 50.1f, 0.f), float4(100.f, 100.f, 1.f, 0.f))
		  .AddBoxCollider(float4(100.f, 100.f, 1.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pWallBack = builder.CreateObject()
		  .AddTransform(float4(0.f, 40.1f, -50.1f, 0.f), float4(100.f, 100.f, 1.f, 0.f))
		  .AddBoxCollider(float4(100.f, 100.f, 1.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pCeiling = builder.CreateObject()
		  .AddTransform(float4(0.f, 90.1f, 0.f, 0.f), float4(100.f, 1.f, 100.f, 0.f))
		  .AddBoxCollider(float4(100.f, 1.f, 100.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();
  }
  else if (leveSelect == 1)
  {
    g_Player1 = builder.CreateObject()
		  .AddTransform(float4(40.f, -5.f, 40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(0)
		  .Finish();

    g_Player1->SetName("Player1");
    Core::FACTORY->SaveSpaceToFile(g_GameSpace, "test", true);


    /*Paintable* paint = (Paintable*)g_GameSpace->CreateComponent(ePaintable);
    g_Player1->AddComponent(paint);
    paint->Initialize();


    pPlayerGrabber->thePlayers.push_back(g_Player1);

    g_Player2 = builder.CreateObject()
		  .AddTransform(float4(-40.f, -5.f, 40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
      .AddPlayerController(1)
		  .Finish();

    paint = (Paintable*)g_GameSpace->CreateComponent(ePaintable);
    g_Player2->AddComponent(paint);
    paint->Initialize();

    pPlayerGrabber->thePlayers.push_back(g_Player2);

    g_Player3 = builder.CreateObject()
		  .AddTransform(float4(40.f, -5.f, -40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(2)
		  .Finish();

    paint = (Paintable*)g_GameSpace->CreateComponent(ePaintable);
    g_Player3->AddComponent(paint);
    paint->Initialize();

    pPlayerGrabber->thePlayers.push_back(g_Player3);

    g_Player4 = builder.CreateObject()
		  .AddTransform(float4(-40.f, -5.f, -40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(3)
		  .Finish();

    paint = (Paintable*)g_GameSpace->CreateComponent(ePaintable);
    g_Player4->AddComponent(paint);
    paint->Initialize();

    pPlayerGrabber->thePlayers.push_back(g_Player4);

	  Core::GameObject *pMainFloor = builder.CreateObject()
		  .AddTransform(float4(0.f, -10.f, 0.f, 0.f), float4(100.f, 1.f, 100.f, 0.f))
		  .AddBoxCollider(float4(100.f, 1.f, 100.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

    paint = (Paintable*)g_GameSpace->CreateComponent(ePaintable);
    pMainFloor->AddComponent(paint);
    paint->Initialize();
    

	  Core::GameObject *pCeiling = builder.CreateObject()
		  .AddTransform(float4(0.f, 40.f, 0.f, 0.f), float4(100.f, 1.f, 100.f, 0.f))
		  .AddBoxCollider(float4(100.f, 1.f, 100.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

    paint = (Paintable*)g_GameSpace->CreateComponent(ePaintable);
    pCeiling->AddComponent(paint);
    paint->Initialize();

	  Core::GameObject *pMainWallRight = builder.CreateObject()
		  .AddTransform(float4(50.f, 15.f, 0.f, 0.f), float4(1.f, 50.f, 100.f, 0.f))
		  .AddBoxCollider(float4(1.f, 100.f, 200.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMainWallLeft = builder.CreateObject()
		  .AddTransform(float4(-50.f, 15.f, 0.f, 0.f), float4(1.f, 50.f, 100.f, 0.f))
		  .AddBoxCollider(float4(1.f, 100.f, 200.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMainWallFront = builder.CreateObject()
		  .AddTransform(float4(0.f, 15.f, 50.f, 0.f), float4(100.f, 50.f, 1.f, 0.f))
		  .AddBoxCollider(float4(100.f, 100.f, 1.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMainWallBack = builder.CreateObject()
		  .AddTransform(float4(0.f, 15.f, -50.f, 0.f), float4(100.f, 50.f, 1.f, 0.f))
		  .AddBoxCollider(float4(100.f, 100.f, 1.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMiddleSegment = builder.CreateObject()
		  .AddTransform(float4(0.f, -5.f, 0.f, 0.f), float4(50.f, 10.f, 50.f, 0.f))
		  .AddBoxCollider(float4(50.f, 10.f, 50.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pRightSegment = builder.CreateObject()
		  .AddTransform(float4(37.5f, -5.f, 0.f, 0.f), float4(25.f, 10.f, 25.f, 0.f))
		  .AddBoxCollider(float4(25.f, 10.f, 25.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pLeftSegment = builder.CreateObject()
		  .AddTransform(float4(-37.5f, -5.f, 0.f, 0.f), float4(25.f, 10.f, 25.f, 0.f))
		  .AddBoxCollider(float4(25.f, 10.f, 25.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pInnerWallUpRight = builder.CreateObject()
		  .AddTransform(float4(25.f, 20.f, 18.75f, 0.f), float4(1.f, 40.f, 12.5f, 0.f))
		  .AddBoxCollider(float4(1.f, 40.f, 25.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pInnerWallDownRight = builder.CreateObject()
		  .AddTransform(float4(25.f, 20.f, -18.75f, 0.f), float4(1.f, 40.f, 12.5f, 0.f))
		  .AddBoxCollider(float4(1.f, 40.f, 25.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();
	  Core::GameObject *pInnerWallUpLeft = builder.CreateObject()
		  .AddTransform(float4(-25.f, 20.f, 18.75f, 0.f), float4(1.f, 40.f, 12.5f, 0.f))
		  .AddBoxCollider(float4(1.f, 40.f, 12.5f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pInnerWallDownLeft = builder.CreateObject()
		  .AddTransform(float4(-25.f, 20.f, -18.75f, 0.f), float4(1.f, 40.f, 12.5f, 0.f))
		  .AddBoxCollider(float4(1.f, 40.f, 12.5f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();*/

    
  }
  else if (leveSelect == 2)
  {
    g_Player1 = builder.CreateObject()
		  .AddTransform(float4(40.f, -5.f, 40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(0)
		  .Finish();

    pPlayerGrabber->thePlayers.push_back(g_Player1);

    g_Player2 = builder.CreateObject()
		  .AddTransform(float4(-40.f, -5.f, 40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(1)
		  .Finish();

    pPlayerGrabber->thePlayers.push_back(g_Player2);

    g_Player3 = builder.CreateObject()
		  .AddTransform(float4(40.f, -5.f, -40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(2)
		  .Finish();

    pPlayerGrabber->thePlayers.push_back(g_Player3);

    g_Player4 = builder.CreateObject()
		  .AddTransform(float4(-40.f, -5.f, -40.f, 0.f))
		  .AddBoxCollider()
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody()
      .AddGravityEffect(float4(0.f, -1.f, 0.f, 0.f), 40.f)
      .AddDragEffect(1.f)
		  .AddPlayerController(3)
		  .Finish();

    pPlayerGrabber->thePlayers.push_back(g_Player4);

	  Core::GameObject *pMainFloor = builder.CreateObject()
		  .AddTransform(float4(0.f, -10.f, 0.f, 0.f), float4(100.f, 1.f, 100.f, 0.f))
		  .AddBoxCollider(float4(100.f, 1.f, 100.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pCeiling = builder.CreateObject()
		  .AddTransform(float4(0.f, 40.f, 0.f, 0.f), float4(100.f, 1.f, 100.f, 0.f))
		  .AddBoxCollider(float4(100.f, 1.f, 100.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMainWallRight = builder.CreateObject()
		  .AddTransform(float4(50.f, 15.f, 0.f, 0.f), float4(1.f, 50.f, 100.f, 0.f))
		  .AddBoxCollider(float4(1.f, 100.f, 200.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMainWallLeft = builder.CreateObject()
		  .AddTransform(float4(-50.f, 15.f, 0.f, 0.f), float4(1.f, 50.f, 100.f, 0.f))
		  .AddBoxCollider(float4(1.f, 100.f, 200.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMainWallFront = builder.CreateObject()
		  .AddTransform(float4(0.f, 15.f, 50.f, 0.f), float4(100.f, 50.f, 1.f, 0.f))
		  .AddBoxCollider(float4(100.f, 100.f, 1.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMainWallBack = builder.CreateObject()
		  .AddTransform(float4(0.f, 15.f, -50.f, 0.f), float4(100.f, 50.f, 1.f, 0.f))
		  .AddBoxCollider(float4(100.f, 100.f, 1.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pMiddleSegment = builder.CreateObject()
		  .AddTransform(float4(0.f, 0.f, 0.f, 0.f), float4(50.f, 20.f, 50.f, 0.f))
		  .AddBoxCollider(float4(50.f, 20.f, 50.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pRightSegment = builder.CreateObject()
		  .AddTransform(float4(37.5f, -5.f, 0.f, 0.f), float4(25.f, 10.f, 50.f, 0.f))
		  .AddBoxCollider(float4(25.f, 10.f, 50.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pLeftSegment = builder.CreateObject()
		  .AddTransform(float4(-37.5f, -5.f, 0.f, 0.f), float4(25.f, 10.f, 50.f, 0.f))
		  .AddBoxCollider(float4(25.f, 10.f, 50.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pFrontSegment = builder.CreateObject()
		  .AddTransform(float4(0.f, -5.f, 37.5f, 0.f), float4(50.f, 10.f, 25.f, 0.f))
		  .AddBoxCollider(float4(50.f, 10.f, 25.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();

	  Core::GameObject *pBackSegment = builder.CreateObject()
		  .AddTransform(float4(0.f, -5.f, -37.5f, 0.f), float4(50.f, 10.f, 25.f, 0.f))
		  .AddBoxCollider(float4(50.f, 10.f, 25.f, 0.f))
		  .AddRenderable("Cube", DEFAULT_TEXTURE, DEFAULT_NORMAL_TEXTURE)
		  .AddRigidBody(Physics::DynamicState::STATIC)
		  .Finish();
  }

  //////////////////////////////////////////////////////////////////////////
  // Lua test
  Object *newObject = new Object();
  luabridge::push(Core::ENGINE->Lua(), newObject);
  lua_setglobal(Core::ENGINE->Lua(), "newObject");

  // Creating a new table
  luabridge::LuaRef newTable = luabridge::newTable(Core::ENGINE->Lua());
  luabridge::push(Core::ENGINE->Lua(), newTable);
  lua_setglobal(Core::ENGINE->Lua(), "newTable");

  bool check = newTable.isTable();

  // Pushing a value in the table
  newTable[1] = 10;
  int num = newTable[1];


  //Lua::LoadScript(Core::ENGINE->Lua(), "content/lua/engine/includes/luatest.lua");
  luabridge::getGlobal(Core::ENGINE->Lua(), "PrintLua")();

  // Get a table
  luabridge::LuaRef ObjTable2 = luabridge::getGlobal(Core::ENGINE->Lua(), "ObjTable2");

  // Get the size of the table, the amount of elements
  int length = ObjTable2.length();

  // Getting a member in the table
  luabridge::LuaRef ObjTable2Inst = luabridge::getGlobal(Core::ENGINE->Lua(), "ObjTable2")[1];

  num = ObjTable2Inst.cast<int>();

  


}

void Sandbox::ReceiveMessage(Core::Message& msg)
{

}

void Sandbox::Update(float dt)
{
  std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
  for each(auto sp in Spaces)
  {
    Base::ObjectAllocator *PlayerArray = sp->GetComponents(ePlayerController);
    for (unsigned nIdx = 0; nIdx < PlayerArray->Size(); ++nIdx)
    {
      Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*PlayerArray)[nIdx]);
      if (!pComp)
        continue;

      GameLogic::PlayerController *pPlayer = dynamic_cast<GameLogic::PlayerController*>(pComp);
      if (!pPlayer)
        continue;
      
      //pPlayer->Update(dt);
    }
    Base::ObjectAllocator *TimerArray = sp->GetComponents(eTimer);
    for (unsigned nIdx = 0; nIdx < TimerArray->Size(); ++nIdx)
    {
      Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*TimerArray)[nIdx]);
      if (!pComp)
        continue;

      GameLogic::Timer *pTimer = dynamic_cast<GameLogic::Timer*>(pComp);
      if (!pTimer)
        continue;

      pTimer->Update(dt);
    }
  }

  //GRAPHICS->DrawString("Test Text", 128.0f, 100, 100, 0xffffffff);
}

void Sandbox::RegisterComponents(void)
{
  REGISTER_COMPONENT(PlayerController, GameLogic::PlayerController);
  REGISTER_COMPONENT(Timer, GameLogic::Timer);
}