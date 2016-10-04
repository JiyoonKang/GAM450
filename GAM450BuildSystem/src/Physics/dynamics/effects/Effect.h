/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Effect.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_DYNAMICS_EFFECTS
#define H_PAINT_PHYSICS_DYNAMICS_EFFECTS

#include <Base\util\math\float4.h>
#include <string>

namespace Physics
{
  class IEffect
  {
  public:
    virtual ~IEffect(void) = 0 {}

    virtual float4 const &GetDirection(void) const = 0;
    virtual void SetDirection(float4 const &Direction) = 0;

    virtual float GetMagnitude(void) const = 0;
    virtual void SetMagnitude(float fMagnitude) = 0;

    virtual float4 GetTotalForce(void) const = 0;

    virtual bool IsActive(void) const = 0;
    virtual void SetActive(bool bActive) = 0;
  };

  class Effect : public IEffect
  {
  public:
    float4 m_Direction;
    float m_fMagnitude;

    bool m_bActive;

  protected:
    // the guid that represents this class
    std::string m_szGuid;
  public:
    Effect(void);
    Effect(float4 const &Direction, float fMagnitude);
    virtual ~Effect(void) override;

    virtual float4 const &GetDirection(void) const final;
    virtual void SetDirection(float4 const &Direction) final;

    virtual float GetMagnitude(void) const final;
    virtual void SetMagnitude(float fMagnitude) final;

    virtual float4 GetTotalForce(void) const override;

    virtual bool IsActive(void) const final;
    virtual void SetActive(bool bActive) final;
  };
}

#endif