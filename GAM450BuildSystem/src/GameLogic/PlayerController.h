/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PlayerController.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_ORBIT_GAME_PLAYER_CONTROLLER
#define H_ORBIT_GAME_PLAYER_CONTROLLER

#include "core/components/base/Component.h"
#include "core/systems/input/Gamepad.h"
#include "graphics/includes/GraphicsSystem/SkyeGraphics.h"

#include "base/util/math/Float4.h"
#include "GameLogic/BulletMgr.h"

namespace GameLogic
{
  class PlayerController : public Core::GameComponent
  {
  private:
    Input::GamePad *m_pGamePad;

    // we need these each frame, might as well cache them
    Core::GameComponent *m_pBody;
    Core::GameComponent *m_pCollider;
    Core::GameComponent *m_pTransform;
    // the current viewport we are drawing too, 
    Skye::Camera *m_pCamera;

  public:
    //UI functions
    void UpdatePlayerStats(void);
    void UpdatePlayerHealth(void);
    void UpdatePlayerScore(int isRing);
    void UpdatePlayerAmmo(void);
    void RecieveMessage(Core::CollisionEnterMessage& msg);

    // used to determine what surfaces can be walked on
    float4 m_WorldUp;

    // determines if I am actually on the ground or not
    bool m_bGrounded;
    // determines if I am currenly in a jump state
    bool m_bJumping;

    float m_fJumpSpeed;
    float m_fJumpCancel;
    float m_fMaxMoveSpeed;

    float4 m_DefaultForward;
    float4 m_PlayerForward;
    float4 m_CameraForward;
    float4 m_CameraPosition;
    float m_fTheta;
    float m_fCurrTheta;
    float m_fPhi;
    float m_fCurrPhi;

    float m_fLastShot;
    float m_fTimeToShoot;
    float m_fOverHeatTime;
    float m_fTimeTillCooling;

    bool m_bHasFired;
    bool m_bCheatsActive;
    bool m_bGodMode;
    float4 m_Color;

    float m_fOverHeat;
    float m_fOverHeatUpdate;
    bool m_bIsOverHeated;
    bool m_bHasFlag;
    // 0 - 3 for game pad id, if it goes above 4, we assert
    int m_nGamePadID;

  private:
    bool CheckValidPtrs(void);
    void Jump(void);
    void JumpCancel(void);
    void UpdateMovement(float4 &movement, float dt);
    void UpdateRotation(float2 const &rotation, float dt);
    void UpdateCamera(void);
    void Shoot(BulletMgr &bulletMgr);
    void CreateBullet(float4 const &pos, float4 const &normal, Core::GameComponent *pCollider);
    void GetKeyboardMovement(float4 &movement);
    void GetKeyboardRotation(float2 &rotation);
    void FixRotation();

  public:
    PlayerController(void);

    //int m_iAmmo;
    //int m_iAmmoMax;

    //float m_fLastShot;
    //float m_fTimeOf;

    float m_fBuzzTime;
    float m_fCurBuzz;

    Input::GamePad const *GetGamePad(void) const;
    Input::GamePad *GetGamePad(void);

    Skye::Camera const *GetCamera(void) const;
    Skye::Camera *GetCamera(void);

    // takes in an unsigned 0 - 3 specifying which game pad will apply to this controller
    void SetGamePad(unsigned nIdx);

    // checks for user input from it's game pad, if any input is true
    // will call the appropriate functions on it's siblings to mimic the
    // desired behavior. ie, when we hit the A button, add a force to the rigidbody
    // in the up direction
    void Update(float dt, BulletMgr &bulletMgr);

    float4 GetColor(void) const
    {
      return m_Color;
    }
    void SetColor(float4 const &color)
    {
      m_Color = color;
    }
    bool m_active;
    bool m_rotationFixed;

    COMP_LUA_USABLE;
    //static void Serialize(File &file, Variable var);
    //static void Deserialize(File &file, Variable var);
  };
}

#endif