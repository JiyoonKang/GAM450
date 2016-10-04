/*******************************************************************************
 * All content � 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PlayerController.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogic/PlayerController.h"
#include "core/types/space/Space.h"
#include "core/types/object/Object.h"
#include "core/components/transform/CTransform.h"

#include "physics/dynamics/RigidBody.h"
#include "physics/core/PhysicsSystem.h"
#include "physics/dynamics/Joint.h"
#include "physics/util/Geometry.h"

#include "graphics/includes/GraphicsSystem/PointLight.h"
#include "graphics/includes/GraphicsSystem/Fade.h"

#include "GameLogic/Level.h"
#include "GameLogic/Stats.h"
#include "GameLogic/Timer.h"
#include "GameLogic/Flag.h"

#include "core/systems/input/KeyboardManager.h"
#include "core/systems/input/Mouse.h"
#include "start/config/Config.h"

#include "SkyeAwesomium/includes/SkyeAwesomium/JSBroadcaster.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/SkyeAwesomium.h"

#include "Editor/InGameEditor/InGameEditor.h"

#include <iostream>
#include <sstream>

namespace GameLogic
{
  PlayerController::PlayerController(void)
    : m_pGamePad(nullptr)
    , m_pBody(nullptr)
    , m_pCollider(nullptr)
    , m_pTransform(nullptr)
    , m_pCamera(nullptr)
    , m_WorldUp(float4(0.f, 1.f, 0.f, 0.f))
    , m_bJumping(false)
    , m_bGrounded(false)
    , m_fJumpSpeed(5.f)
    , m_fJumpCancel(0.5f)
    , m_fMaxMoveSpeed(10.f)
    , m_DefaultForward(float4(0.f, 0.f, 1.f, 0.f))
    , m_PlayerForward(float4(0.f, 0.f, 1.f, 0.f))
    , m_CameraForward(float4(0.f, 0.f, 1.f, 0.f))
    // every frame our stick is fully on one side we rotate 2 degrees in that direction
    , m_fTheta(2.f)
    , m_fCurrTheta(0.f)
    , m_fPhi(2.f)
    , m_fCurrPhi(0.f)
    , m_nGamePadID(0)
    , m_fLastShot(0.3f)
    , m_fTimeToShoot(0.3f)
    , m_fOverHeatTime(0.3)
    , m_fTimeTillCooling(0.5f)
    , m_bHasFired(false)
    , m_bCheatsActive(false)
    , m_bGodMode(false)
    , m_Color(float4(1.f, 0.f, 0.f, 1.f))
    //, m_iAmmo(20)
    //, m_iAmmoMax(m_iAmmo)
    , m_fOverHeat(0.0f)
    , m_fOverHeatUpdate(1.0f)
    , m_bIsOverHeated(false)
    , m_bHasFlag(false)
    , m_fBuzzTime(0.25f)
    , m_fCurBuzz(0.0f)
    , m_active(true)
    , m_CameraPosition()
    , m_rotationFixed(false)
  {
    
  }

  Input::GamePad const *PlayerController::GetGamePad(void) const
  {
    return m_pGamePad;
  }

  Input::GamePad *PlayerController::GetGamePad(void)
  {
    return m_pGamePad;
  }

  Skye::Camera const *PlayerController::GetCamera(void) const
  {
    return m_pCamera;
  }

  Skye::Camera *PlayerController::GetCamera(void)
  {
    return m_pCamera;
  }

  bool PlayerController::CheckValidPtrs(void)
  {
    if (!m_pBody)
    {
      m_pBody = GetOwner()->GetComponent<Core::GameComponent>(eRigidBody);
      if (!m_pBody)
        return false;
    }

    if (!m_pTransform)
    {
      m_pTransform = GetOwner()->GetComponent<Core::GameComponent>(eTransform);
      if (!m_pTransform)
        return false;
    }

    if (!m_pCollider)
    {
      Core::GameComponent *pBox = GetOwner()->GetComponent<Core::GameComponent>(eBoxCollider);
      Core::GameComponent *pSphere = GetOwner()->GetComponent<Core::GameComponent>(eSphereCollider);
      Core::GameComponent *pCapsule = GetOwner()->GetComponent<Core::GameComponent>(eCapsuleCollider);

      if (pBox)
        m_pCollider = pBox;
      else if (pSphere)
        m_pCollider = pSphere;
      else if (pCapsule)
        m_pCollider = pCapsule;
      else
        return false;
    }

    if (!m_pCamera)
    {
      m_pCamera = GRAPHICS->GetCamera(m_nGamePadID);
      if (!m_pCamera)
        return false;
    }

    if (!m_pGamePad)
    {
      m_pGamePad = Input::GamePadEvent::GetGamePad(static_cast<unsigned>(m_nGamePadID));
      if (!m_pGamePad) // need to be able to still run with the keyboard
        return false;
    }

    return true;
  }

  void PlayerController::UpdateRotation(float2 const &rotation, float /*dt*/)
  {
    Core::Transform *pTransform = dynamic_cast<Core::Transform*>(m_pTransform);
    Physics::RigidBody* pBody = dynamic_cast<Physics::RigidBody*>(GetOwner()->GetComponent(eRigidBody));
    if (!pTransform || !pBody)
      return;

    if (Equals(rotation.x, 0.f) && Equals(rotation.y, 0.f))
      pTransform->SetForwardVector(m_PlayerForward);
    else
    {
      // take my current forward vector
      float4 currForward = m_DefaultForward;
      float currTheta = DEG_TO_RAD * m_fTheta * rotation.x;
      float currPhi = DEG_TO_RAD * m_fPhi * rotation.y;
      float tempThetaTotal = m_fCurrTheta;
      float tempPhiTotal = m_fCurrPhi;
      tempThetaTotal += currTheta;
      tempPhiTotal += currPhi;
    
      quaternion rotationAlongY(float4(0.f, 1.f, 0.f, 0.f), tempThetaTotal);
      quaternion rotationAlongX(pTransform->GetRightVector(), -tempPhiTotal);
      float4 newForward = rotationAlongY.RotateVector(currForward);

      m_PlayerForward = newForward;
      pTransform->SetForwardVector(m_PlayerForward);
    
      // we only want to rotate up or down for the camera
      // TODO: Add a bounds check for being able to rotate the camera forward too far down or up
      rotationAlongX.RotateVectorInPlace(newForward);
      // before we set this as our new forward, make sure it doesn't cross the vertical boundary
      float4 up(0.f, 1.f, 0.f, 0.f);
      float d = newForward.Dot(up);
      if (EqualsEp(d, 1.f, 0.01f) || EqualsEp(d, -1.f, 0.01f))
        return;
    
      m_CameraForward = newForward;
    
      m_fCurrTheta = tempThetaTotal;
      m_fCurrPhi = tempPhiTotal;
    }
  }

  void PlayerController::UpdateMovement(float4 &movement, float dt)
  {
    Physics::RigidBody *pBody = dynamic_cast<Physics::RigidBody*>(m_pBody);
    Core::Transform *pTransform = dynamic_cast<Core::Transform*>(m_pTransform);
    if (!pBody || !pTransform)
      return;

    // remove any movement in the world up axis
    movement -= movement.Project(m_WorldUp);
    // we'll normalize it, xinput is almost guarenteed to not give me something
    // normalized

    // To rotate movement vector with respect to playerForward, start by getting
    // the player's right vector (forward rotated by 90 degrees around y axis).
    float4 playerRight = float4(m_PlayerForward.z, 0.0f, -m_PlayerForward.x, 0.0f);

    // Then create a rotation matrix
    float4x4 rotMatrix = float4x4(playerRight, float4(), m_PlayerForward, float4());
    movement = rotMatrix * movement;

    float4 nullVel = float4(0.0f, 0.0f, 0.0f, 0.0f);
    nullVel.y = pBody->GetVelocity().y;
    pBody->SetVelocity(nullVel);
    float len = 0.f;
    movement.Normalize(&len);

    if (Equals(len, 0.f))
      return;
    pBody->AddForce(movement * m_fMaxMoveSpeed);
  }

  void PlayerController::Jump(void)
  {
    float4 impulse = m_WorldUp * m_fJumpSpeed;
    Physics::RigidBody *pBody = dynamic_cast<Physics::RigidBody*>(m_pBody);
    if (!pBody)
      return;
    if (!Audio::AUDIOMANAGER->GetAllMute())
    {
      Audio::SoundInstance instance;
      instance.SetVolume(1.0f);
      Audio::AUDIOMANAGER->PlayEvent("OutOfAmmo", &instance);
      instance.SetVolume(1.0f);
    }
    m_bJumping = true;
    pBody->ApplyLinearImpulse(impulse);
  }

  void PlayerController::JumpCancel(void)
  {
    Physics::RigidBody *pBody = dynamic_cast<Physics::RigidBody*>(m_pBody);
    float4 vel = pBody->GetVelocity();
    if (vel.y > 0)
    {
      vel.y -= vel.y / 10;
      pBody->SetVelocity(vel);
    }
  }

  void PlayerController::UpdateCamera(void)
  {
    if (!InGameEditor::IsEditorActive())
    {
      Core::Transform *pTransform = dynamic_cast<Core::Transform*>(m_pTransform);
      //m_pCamera->SetPosition(pTransform->GetTranslation() + pTransform->GetUpVector() + m_CameraForward * 5);
      //m_pCamera->SetLookAt(m_CameraForward);
      float3 pos = m_pCamera->GetPosition();
      m_CameraPosition = float4(pos.x, pos.y, pos.z, 0.f);
    }
  }

  void PlayerController::CreateBullet(float4 const &pos, float4 const &normal, Core::GameComponent *pHitCollider)
  {
    ObjectBuilder builder(space);
    Core::GameObject *pBullet = builder.CreateObject()
      .AddTransform(pos + normal)
      .AddPointLight(m_Color)
      .AddRigidBody()
      .Finish();

    // create a weld joint between the bullet and the thing it hit
    // calculate the local positions and rotations for each body
    Core::Transform *pHitTransform = pHitCollider->GetOwner()->GetComponent<Core::Transform>(eTransform);
    ErrorIf(!pHitTransform);
    if (!pHitTransform)
      return;

    Core::Transform *pBulletTransform = pBullet->GetComponent<Core::Transform>(eTransform);
    ErrorIf(!pBulletTransform);
    if (!pBulletTransform)
      return;

    // local position for the bullet should just be the origin
    //float4 localBulletPos;// = Physics::RotatePointIntoBasis(pBulletTransform->GetRotationMatrix(),
    //  //pBulletTransform->GetTranslation(), pos);
    //// can find local position for other collider with the transform
    //float4 localOtherPos = Physics::RotatePointIntoBasis(pHitTransform->GetRotationMatrix(), 
    //  pHitTransform->GetTranslation(), pBulletTransform->GetTranslation());
    //
    //// should only need the bullet's orientation, it's just a point so orientation is identity
    //quaternion localBulletRot;
    //
    //// create my weld joint
    //Physics::WeldJoint *pJoint = new Physics::WeldJoint(localBulletPos, 
    //  localBulletRot, localOtherPos, localBulletRot);
    //ErrorIf(!pJoint);
    //if (!pJoint)
    //  return;
    //
    //Physics::RigidBody *pBodyBullet = pBullet->GetComponent<Physics::RigidBody>(eRigidBody);
    //Physics::RigidBody *pBodyHit = pHitCollider->GetOwner()->GetComponent<Physics::RigidBody>(eRigidBody);
    //ErrorIf(!pBodyBullet || !pBodyHit);
    //if (!pBodyBullet || !pBodyHit)
    //  return;
    //
    //pJoint->SetBodyPtrs(pBodyBullet, pBodyHit);
    //
    //// TODO: Make sure to clean this up sometime
    //Physics::PhysicsSystem::AddJoint(pJoint);
  }

  void PlayerController::Shoot(BulletMgr &bulletMgr)
  {
    float4 pos = m_CameraPosition + (m_CameraForward*2);
    Core::Transform *pTransform = dynamic_cast<Core::Transform*>(m_pTransform);
    if (!pTransform)
      return;
    bulletMgr.AddBullet(m_Color, space, pos, m_CameraForward, GetOwner());
    ////shoot a ray down the camera forward vector, this is our shoot vector
    //Core::Transform *pTransform = dynamic_cast<Core::Transform*>(m_pTransform);
    //if (!pTransform)
    //  return;
    //
    //// cast the ray, be sure to ignore our own collider
    //Physics::Ray ray;
    //ray.m_Direction = m_CameraForward;
    //ray.m_Start = m_CameraPosition;
    //Physics::RayCastResult result;
    //std::vector<Core::GameComponent*> filterList;
    //filterList.push_back(m_pCollider);
    //Physics::PhysicsSystem::CastRayFiltered(GetOwner()->space, ray, result, filterList);
    //
    //float3 start, end;
    //start = float3(ray.m_Start.x, ray.m_Start.y, ray.m_Start.z);
    //end = start + float3(ray.m_Direction.x, ray.m_Direction.y, ray.m_Direction.z) * 5.f;
    ////GRAPHICS->GetDebugDrawer().DrawLine(start, end, float4(1.f, 0.f, 0.f, 1.f));
    //
    //// if it hit something
    //if (!result.m_FirstCollider)
    //  return;
    //
    //// for now draw a point on that spot
    //float3 pos = float3(result.m_IntersectionPoint.x, result.m_IntersectionPoint.y, result.m_IntersectionPoint.z);
    ////GRAPHICS->GetDebugDrawer().DrawSphere(pos, 0.1f, float4(1.f, 0.f, 0.f, 1.f));
    //
    //// create a bullet at that spot, aka a point light
    //bulletMgr.AddBullet(result, m_Color, space);
    //
    //// splatter paint at the collision point
    ////Paintable* paintable = reinterpret_cast<Paintable*>(result.m_FirstCollider->GetOwner()->GetComponent(ePaintable));
    ////if (paintable)
    ////{
    ////  paintable->SetCollisionColor(rand() % (paintable->GetPaintTextureAreaWidth() - 1), rand() % (paintable->GetPaintTextureAreaHeight() - 1), Skye::Color::s_Blue);
    ////}
    //// if it hit a player, register damage
    //PlayerController* enemyPlayer = (PlayerController*)result.m_FirstCollider->GetOwner()->GetComponent("PlayerController");
    //if (enemyPlayer && enemyPlayer->m_bGodMode == false)
    //{
    //  enemyPlayer->GetGamePad()->SetVibration(float2(0.5f, 0.5f));
    //  enemyPlayer->m_fCurBuzz = 0.0f;
    //  Stats* targetStats = reinterpret_cast<Stats*>(result.m_FirstCollider->GetOwner()->GetComponent("Stats"));
    //  if (m_bGodMode)
    //  {
    //    targetStats->m_health = 0;
    //  }
    //  else
    //  {
    //    targetStats->m_health -= 5;
    //    enemyPlayer->UpdatePlayerHealth();
    //  }
    //  if (targetStats->m_health == 0)
    //  {
    //    //JSBroadcaster broadcaster;
    //    //broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
    //    //broadcaster.AddArgument(enemyPlayer->m_nGamePadID + 1);
    //    //broadcaster.AddArgument(1);
    //    //broadcaster.InvokeJSMethod("togglePlayerDead");
    //
    //    Stats* myStats = reinterpret_cast<Stats*>(GetOwner()->GetComponent("Stats"));
    //    if (myStats->GetTimeOfLastKill() > 0.0f)
    //    {
    //      int killCheck = myStats->GetSuccessiveKills();
    //      if (killCheck > 0)
    //      {
    //        myStats->m_score += killCheck * 100;
    //      }
    //    }
    //    myStats->GotAKill();
    //    //UpdatePlayerScore();
        if (!Audio::AUDIOMANAGER->GetAllMute())
        {
          Audio::SoundInstance instance;
          instance.SetVolume(1.0f);
          Audio::AUDIOMANAGER->PlayEvent("Shoot1", &instance);
          instance.SetVolume(1.0f);
        }
    //  }
    //}
    Fade* pFade =  reinterpret_cast<Fade*>(GetOwner()->GetComponent("Fade"));
    pFade->SetOpacity(pFade->GetOpacity() + 0.25f);
    pFade->SetFadeOutTime(1.0f);
    ////UpdatePlayerAmmo();
  }

  void PlayerController::GetKeyboardMovement(float4 &movement)
  {
    if (Input::KeyboardEvent::IsKeyHeld(Input::KEY_W))
      movement.z += 1.f;
    if (Input::KeyboardEvent::IsKeyHeld(Input::KEY_S))
      movement.z -= 1.f;
    if (Input::KeyboardEvent::IsKeyHeld(Input::KEY_D))
      movement.x += 1.f;
    if (Input::KeyboardEvent::IsKeyHeld(Input::KEY_A))
      movement.x -= 1.f;
  }

  void PlayerController::GetKeyboardRotation(float2 &rotation)
  {
    rotation = Input::MouseEvent::GetMouseDelta();
    rotation.y = -rotation.y;
    rotation.Normalize();
  }

  void PlayerController::FixRotation()
  {
    Core::Transform* myTrans = reinterpret_cast<Core::Transform*>(GetOwner()->GetComponent("Transform"));
    Physics::FixedAngleJoint* joint = new Physics::FixedAngleJoint(myTrans->GetRotation(), myTrans->GetRotation());
    ErrorIf(!joint);
    if (!joint)
      return;

    Physics::PhysicsSystem::AddJoint(joint);
    m_rotationFixed = true;
  }

  void PlayerController::Update(float dt, BulletMgr &bulletMgr)
  {
    // check all pointers to see if they exist, if they don't grab them
    // and if they still don't exist we can't run this controller
    if (!CheckValidPtrs())
      return;
    if (!m_rotationFixed)
      FixRotation();

    std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
    UpdateCamera();
    Stats* myStats = reinterpret_cast<Stats*>(GetOwner()->GetComponent(eStats));

    if (myStats->GetAlive() == false)
    {
      m_active = false;
    }
    else
    {
      m_active = true;
    }
    for each(auto sp in Spaces)
    {
      Base::ObjectAllocator *pTimerArray = sp->GetComponents(eTimer);
      if (!pTimerArray)
        continue;

      for (unsigned nIdx = 0; nIdx < pTimerArray->Size(); ++nIdx)
      {
        Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pTimerArray)[nIdx]);
        if (!pComp)
          continue;

        GameLogic::Timer *pTimer = dynamic_cast<GameLogic::Timer*>(pComp);
        if (!pTimer)
          continue;

        if (pTimer->m_startTime > 0)
        {
          m_active = false;
        }
      }
    }
    if (m_active)
    {
      float4 movement;
      float2 rotation;

      if (m_fOverHeat >= m_fOverHeatUpdate)
      {
        m_bIsOverHeated = true;
        m_fOverHeat = m_fOverHeatUpdate - 0.01;
        if (!Audio::AUDIOMANAGER->GetAllMute())
        {
          Audio::SoundInstance instance;
          instance.SetVolume(1.0f);
          Audio::AUDIOMANAGER->PlayEvent("Reload", &instance);
          instance.SetVolume(1.0f);
        }
        UpdatePlayerAmmo();
      }

      if (m_fOverHeatTime > m_fTimeTillCooling)
      {
        //float lastFrame = m_fOverHeat;
        m_fOverHeat -= dt*10;
        if (m_fOverHeat < 0.0f)
        {
          m_fOverHeat = 0.0f;
        }
        if (m_fOverHeat == 0.0f)
        {
          m_bIsOverHeated = false;
        }
        UpdatePlayerAmmo();
        m_fOverHeatTime = 0;
      }

      if (m_pGamePad->connected)
      {
        float2 leftStick = m_pGamePad->LeftStick();
        float2 rightStick = m_pGamePad->RightStick();

        movement.x = leftStick.x;
        movement.z = leftStick.y;

        if (movement.x > -0.0001 && movement.x < 0.0001)
        {
          movement.x = 0.0f;
        }
        if (movement.z > -0.0001 && movement.z < 0.0001)
        {
          movement.z = 0.0f;
        }
      //if (movement.x != 0.0f || movement.z != 0.0f)
      //{
      //  Audio::SoundInstance instance;
      //  instance.SetVolume(1.0f);
      //  Audio::AUDIOMANAGER->PlayEvent("AVT/AVT_Run", &instance);
      //  instance.SetVolume(1.0f);
      //}


        rotation.x = rightStick.x;
        rotation.y = rightStick.y;
      }
      else if(Config::installer)// keyboard movement and rotation
      {
        GetKeyboardMovement(movement);
        GetKeyboardRotation(rotation);
      }

      // if we press a, jump
      if (m_pGamePad->connected)
      {
        if (m_pGamePad->ButtonJustPressed(XINPUT_GAMEPAD_A))
        {
          if (!m_bJumping)
          {
            
            Jump();
          }
        }

        if (!m_pGamePad->ButtonPressed(XINPUT_GAMEPAD_A))
        {
          if (m_bJumping)
            JumpCancel();
        }
      }
      else if(Config::installer) // keyboard movement
      {
        if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_SPACE))
        {
          Jump();
        }
          
        
        if (Input::KeyboardEvent::IsKeyReleased(Input::KEY_SPACE))
        {
          if (m_bJumping)
            JumpCancel();
        }
      }
      
      // reloading
      if (m_pGamePad->connected)
      {
        //if (m_pGamePad->ButtonPressed(XINPUT_GAMEPAD_X) && m_iAmmo < m_iAmmoMax && !m_pGamePad->RightTrigger() && m_bIsOverHeated == false)
        //{
        //  Audio::SoundInstance instance;
        //  instance.SetVolume(1.0f);
        //  Audio::AUDIOMANAGER->PlayEvent("Gun_Reload_ChamberSpin", &instance);
        //  instance.SetVolume(1.0f);
        //  m_iAmmo = m_iAmmoMax;
        //  m_fOverHeat += dt;
        //}
        //if (m_fOverHeat > m_fOverHeatUpdate)
        //{
        //  m_bIsOverHeated = true;
        //}
      }
      else if (Config::installer)
      {
        //if (Input::KeyboardEvent::IsKeyHeld(Input::KEY_R) && m_iAmmo < m_iAmmoMax && !Input::MouseEvent::IsLeftMouseButtonPressed() && m_bIsOverHeated == false)
        //{
        //  Audio::SoundInstance instance;
        //  instance.SetVolume(1.0f);
        //  Audio::AUDIOMANAGER->PlayEvent("Gun_Reload_ChamberSpin", &instance);
        //  instance.SetVolume(1.0f);
        //  m_iAmmo = m_iAmmoMax;
        //  UpdatePlayerAmmo();
        //  m_fOverHeat += dt;
        //}
        //if (m_fOverHeat > m_fOverHeatUpdate)
        //{
        //  m_bIsOverHeated = true;
        //}
      }

      if (m_pGamePad->connected)
      {
        if (m_pGamePad->RightTrigger() && m_fTimeToShoot < m_fLastShot && m_bIsOverHeated == true)
        {
          m_fLastShot = 0.0f;
          //m_fOverHeatTime = 0.0f;
        }
        else if (m_pGamePad->RightTrigger() && m_fTimeToShoot < m_fLastShot && m_bIsOverHeated == false)
        {
          m_fOverHeat += dt * 2.5;
          Shoot(bulletMgr);
          m_fLastShot = 0.0f;
          m_fOverHeatTime = 0.0f;
          UpdatePlayerAmmo();
        }


      }
      else if(Config::installer)// keyboard controls
      {
        if (Input::MouseEvent::IsLeftMouseButtonPressed() && m_fTimeToShoot < m_fLastShot && m_bIsOverHeated == true)
        {
          if (!Audio::AUDIOMANAGER->GetAllMute())
          {
            Audio::SoundInstance instance;
            instance.SetVolume(1.0f);
            Audio::AUDIOMANAGER->PlayEvent("Reload", &instance);
            instance.SetVolume(1.0f);
          }
          m_fLastShot = 0.0f;
        }
        else if (Input::MouseEvent::IsLeftMouseButtonPressed() && m_fTimeToShoot < m_fLastShot && m_bIsOverHeated == false)
        {
          //if (!Audio::AUDIOMANAGER->GetAllMute())
          //{
          //  Audio::SoundInstance instance;
          //  instance.SetVolume(1.0f);
          //  Audio::AUDIOMANAGER->PlayEvent("Shoot1", &instance);
          //  instance.SetVolume(1.0f);
          //}
          m_fOverHeat += dt * 2.5;
          Shoot(bulletMgr);
          m_fLastShot = 0.0f;
        }
      }
      

      if (m_bCheatsActive)
      {
        if (m_pGamePad->connected)
        {
          if (m_pGamePad->ButtonPressed(XINPUT_GAMEPAD_DPAD_UP))
          {
            myStats->m_score += 100;
            //UpdatePlayerScore();
          }
        }
        else if (Config::installer)
        {
          if (m_nGamePadID == 0)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_8))
            {
              myStats->m_score += 100;
              //UpdatePlayerScore();
            }
          }
          else if (m_nGamePadID == 1)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_U))
            {
              myStats->m_score += 100;
              //UpdatePlayerScore();
            }
          }
          else if (m_nGamePadID == 2)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_J))
            {
              myStats->m_score += 100;
              //UpdatePlayerScore();
            }
          }
          else if (m_nGamePadID == 3)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_M))
            {
              myStats->m_score += 100;
              //UpdatePlayerScore();
            }
          }
        }
        if (m_pGamePad->connected)
        {
          if (m_pGamePad->ButtonJustPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
          {
            if (myStats->m_health < 25)
            {
              myStats->m_health = 25;
            }
          }
        }
        else if (Config::installer)
        {
          if (m_nGamePadID == 0)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_9))
            {
              if (myStats->m_health < 25)
              {
                myStats->m_health = 25;
              }
            }
          }
          else if (m_nGamePadID == 1)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_I))
            {
              if (myStats->m_health < 25)
              {
                myStats->m_health = 25;
              }
            }
          }
          else if (m_nGamePadID == 2)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_K))
            {
              if (myStats->m_health < 25)
              {
                myStats->m_health = 25;
              }
            }
          }
          else if (m_nGamePadID == 3)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_COMMA))
            {
              if (myStats->m_health < 25)
              {
                myStats->m_health = 25;
              }
            }
          }
        }
        if (m_pGamePad->connected)
        {
          if (m_pGamePad->ButtonJustPressed(XINPUT_GAMEPAD_DPAD_DOWN))
          {
            if (m_bGodMode == false)
            {
              m_bGodMode = true;
            }
            else
            {
              m_bGodMode = false;
            }
          }
        }
        else if (Config::installer)
        {
          if (m_nGamePadID == 0)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_0))
            {
              if (m_bGodMode == false)
              {
                m_bGodMode = true;
              }
              else
              {
                m_bGodMode = false;
              }
            }
          }
          else if (m_nGamePadID == 1)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_O))
            {
              if (m_bGodMode == false)
              {
                m_bGodMode = true;
              }
              else
              {
                m_bGodMode = false;
              }
            }
          }
          else if (m_nGamePadID == 2)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_L))
            {
              if (m_bGodMode == false)
              {
                m_bGodMode = true;
              }
              else
              {
                m_bGodMode = false;
              }
            }
          }
          else if (m_nGamePadID == 3)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_PERIOD))
            {
              if (m_bGodMode == false)
              {
                m_bGodMode = true;
              }
              else
              {
                m_bGodMode = false;
              }
            }
          }
        }
        if (m_pGamePad->connected)
        {
          if (m_pGamePad->ButtonJustPressed(XINPUT_GAMEPAD_DPAD_LEFT))
          {
            std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
            for each(auto sp in Spaces)
            {
              Base::ObjectAllocator *pTimerArray = sp->GetComponents(eTimer);
              if (!pTimerArray)
                continue;

              for (unsigned nIdx = 0; nIdx < pTimerArray->Size(); ++nIdx)
              {
                Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pTimerArray)[nIdx]);
                if (!pComp)
                  continue;

                GameLogic::Timer *pTimer = dynamic_cast<GameLogic::Timer*>(pComp);
                if (!pTimer)
                  continue;

                pTimer->SetTime(0.0f);
              }
            }
          }
        }
        else if (Config::installer)
        {
          if (m_nGamePadID == 0)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_MINUS))
            {
              std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
              for each(auto sp in Spaces)
              {
                Base::ObjectAllocator *pTimerArray = sp->GetComponents(eTimer);
                if (!pTimerArray)
                  continue;

                for (unsigned nIdx = 0; nIdx < pTimerArray->Size(); ++nIdx)
                {
                  Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pTimerArray)[nIdx]);
                  if (!pComp)
                    continue;

                  GameLogic::Timer *pTimer = dynamic_cast<GameLogic::Timer*>(pComp);
                  if (!pTimer)
                    continue;

                  pTimer->SetTime(0.0f);
                }
              }
            }
          }
          else if (m_nGamePadID == 1)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_P))
            {
              std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
              for each(auto sp in Spaces)
              {
                Base::ObjectAllocator *pTimerArray = sp->GetComponents(eTimer);
                if (!pTimerArray)
                  continue;

                for (unsigned nIdx = 0; nIdx < pTimerArray->Size(); ++nIdx)
                {
                  Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pTimerArray)[nIdx]);
                  if (!pComp)
                    continue;

                  GameLogic::Timer *pTimer = dynamic_cast<GameLogic::Timer*>(pComp);
                  if (!pTimer)
                    continue;

                  pTimer->SetTime(0.0f);
                }
              }
            }
          }
          else if (m_nGamePadID == 2)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_SEMICOLON))
            {
              std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
              for each(auto sp in Spaces)
              {
                Base::ObjectAllocator *pTimerArray = sp->GetComponents(eTimer);
                if (!pTimerArray)
                  continue;

                for (unsigned nIdx = 0; nIdx < pTimerArray->Size(); ++nIdx)
                {
                  Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pTimerArray)[nIdx]);
                  if (!pComp)
                    continue;

                  GameLogic::Timer *pTimer = dynamic_cast<GameLogic::Timer*>(pComp);
                  if (!pTimer)
                    continue;

                  pTimer->SetTime(0.0f);
                }
              }
            }
          }
          else if (m_nGamePadID == 3)
          {
            if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_SLASH))
            {
              std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
              for each(auto sp in Spaces)
              {
                Base::ObjectAllocator *pTimerArray = sp->GetComponents(eTimer);
                if (!pTimerArray)
                  continue;

                for (unsigned nIdx = 0; nIdx < pTimerArray->Size(); ++nIdx)
                {
                  Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pTimerArray)[nIdx]);
                  if (!pComp)
                    continue;

                  GameLogic::Timer *pTimer = dynamic_cast<GameLogic::Timer*>(pComp);
                  if (!pTimer)
                    continue;

                  pTimer->SetTime(0.0f);
                }
              }
            }
          }
        }
      }
      if (m_pGamePad->connected)
      {
        if (m_pGamePad->ButtonJustPressed(XINPUT_GAMEPAD_BACK))
        {
          if (m_bCheatsActive == false)
          {
            m_bCheatsActive = true;
          }
          else
          {
            m_bCheatsActive = false;
          }
        }
      }
      else if (Config::installer)
      {
        if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_GRAVE_ACCENT))
        {
          if (m_bCheatsActive == false)
          {
            m_bCheatsActive = true;
          }
          else
          {
            m_bCheatsActive = false;
          }
        }
      }
      m_fLastShot += dt;
      m_fOverHeatTime += dt;
      UpdateMovement(movement, dt);
      UpdateRotation(rotation, dt);
    }
    m_fCurBuzz += dt;
    if (m_fCurBuzz > m_fBuzzTime)
    {
      GetGamePad()->SetVibration(0.0f, 0.0f);
    }
  }

  void PlayerController::SetGamePad(unsigned nIdx)
  {
    ErrorIf(nIdx > 3);
    if (nIdx > 3)
      return;

    m_nGamePadID = nIdx;
    // check valid ptrs will update this with the correct camera and gamepad
    m_pGamePad = nullptr;
    m_pCamera = nullptr;
  }

 void PlayerController::UpdatePlayerHealth(void)
 {
   Stats* myStats = reinterpret_cast<Stats*>(GetOwner()->GetComponent("Stats"));
 
      //char array buffer
   char buff[50] = { 0 };
    //get the game pad id in the buffer
   itoa((int)this->m_nGamePadID + 1, buff, 10);
 
      //start creating the message to updae UI
   std::string msg = std::string("JSMethod updatePlayerHealth ") + std::string(buff)+std::string(" ");
   itoa(myStats->m_health, buff, 10);
   msg += std::string(buff);
 
   Core::MessageUI uiM(msg);
 
   Core::ENGINE->SystemMessage(uiM);
 }

  void PlayerController::UpdatePlayerScore(int isRing)
  {
    Stats* myStats = reinterpret_cast<Stats*>(GetOwner()->GetComponent("Stats"));

    //char array buffer
    char buff[50] = { 0 }; 
    //get the game pad id in the buffer
    itoa((int)this->m_nGamePadID + 1, buff, 10);

    //start creating the message to updae UI
    std::string msg = std::string("JSMethod updatePlayerScore ") + std::string(buff) + std::string(" ");
    
    itoa(myStats->m_score, buff, 10);
    msg += std::string(buff) + std::string(" ");

    itoa(isRing, buff, 10);
    msg += std::string(buff);

    Core::MessageUI uiM(msg);

    Core::ENGINE->SystemMessage(uiM);
  }

  void PlayerController::UpdatePlayerAmmo(void)
  {
    Stats* myStats = reinterpret_cast<Stats*>(GetOwner()->GetComponent("Stats"));
    
    //char array buffer
    char buff[50] = { 0 };
    //get the game pad id in the buffer
    itoa((int)this->m_nGamePadID + 1, buff, 10);
    
    //start creating the message to updae UI
    std::string msg = std::string("JSMethod updatePlayerAmmo ") + std::string(buff) + std::string(" ");
    itoa(m_fOverHeat * 100, buff, 10);
    msg += std::string(buff);
    
    Core::MessageUI uiM(msg);
    
    Core::ENGINE->SystemMessage(uiM);
  }

  void PlayerController::UpdatePlayerStats(void)
  {
    Stats* myStats = reinterpret_cast<Stats*>(GetOwner()->GetComponent("Stats"));

    //char array buffer
    char buff[50] = { 0 };
    //get the game pad id in the buffer
    itoa((int)this->m_nGamePadID + 1, buff, 10);

    const std::string space(" ");

    //start creating the message to updae UI
    std::string msg = std::string("JSMethod updatePlayerStats ") + std::string(buff) + space;

    itoa(myStats->m_health, buff, 10);
    msg += std::string(buff) + space;

    itoa(myStats->m_score, buff, 10);
    msg += std::string(buff) + space;

    itoa(m_fOverHeat*10, buff, 10);
    msg += std::string(buff);

    Core::MessageUI uiM(msg);

    Core::ENGINE->SystemMessage(uiM);
  }

  void PlayerController::RecieveMessage(Core::CollisionEnterMessage& m)
  {
    std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      auto thisObj = sp->GetGameObject(m.body);
      if (GetOwner() == thisObj)
      {
        auto otherObj = sp->GetGameObject(m.other);
        if (!otherObj)
        {
          continue;
        }
        else if (otherObj->GetName() == "Light")
        {
          Flag* theFlag = reinterpret_cast<Flag*>(otherObj->GetComponent("Flag"));
          if (theFlag->m_bActive == false)
          {
            theFlag->m_hAttachedObject = owner;
            theFlag->m_bActive = true;
            m_bHasFlag = true;
            //Renderable* render = static_cast<Renderable*>(otherObj->GetComponent(eRenderable));
            //render->m_IsVisible = false;
            EmissiveComponent* pEmissive = reinterpret_cast<EmissiveComponent*>(otherObj->GetComponent(eEmissiveComponent));
            PointLight* pLight = reinterpret_cast<PointLight*>(otherObj->GetComponent(ePointLight));
            if (!pLight || !pEmissive)
              return;
            pEmissive->m_emittedColor = m_Color;
            pLight->m_LightColor = m_Color;

            char buff[50] = { 0 };
            //get the game pad id in the buffer
            itoa((int)this->m_nGamePadID + 1, buff, 10);

            const std::string space(" ");

            //start creating the message to updae UI
            std::string msg = std::string("JSMethod togglePlayerHasBall ") + std::string(buff);
            
            Core::MessageUI uiM(msg);

            Core::ENGINE->SystemMessage(uiM);
            if (!Audio::AUDIOMANAGER->GetAllMute())
            {
              Audio::SoundInstance instance;
              instance.SetVolume(1.0f);
              Audio::AUDIOMANAGER->PlayEvent("CollectItem", &instance);
              instance.SetVolume(1.0f);
            }

          }
        }
        else
        {
          Core::Transform* otherTrans = reinterpret_cast<Core::Transform*>(otherObj->GetComponent("Transform"));
          Core::Transform* myTrans = reinterpret_cast<Core::Transform*>(GetOwner()->GetComponent("Transform"));
          if (myTrans->GetTranslation().y - myTrans->GetScale().y / 2 > otherTrans->GetTranslation().y + otherTrans->GetScale().y / 2)
          {
            m_bJumping = false;
          }
        }
      }
    }
  }

  COMP_LUA_DEP(PlayerController);

}