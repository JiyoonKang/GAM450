/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: RayCastMgr.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_RAY_CAST_MGR
#define H_PAINT_PHYSICS_RAY_CAST_MGR

#include "Physics/util/Geometry.h"
#include "Core/components/base/Component.h"
#include "base/modules/memory/ObjectAllocator.h"

#include "base/modules/handle/Handle.h"

#include <unordered_map>

namespace Physics
{
  typedef std::pair<float, Core::GameComponent*> TColliderPair;
  struct RayCastResult
  {
    // the collider that was struck first with the ray
    Core::GameComponent *m_FirstCollider;
    float m_FirstT;
    // every collider that was possibly hit by the ray
    std::vector<TColliderPair> m_HitColliders;
    // the original ray
    Ray m_Ray;
    // the intersection point
    float4 m_IntersectionPoint;
    // the face normal at intersection
    float4 m_Normal;

    RayCastResult(void)
      : m_FirstCollider(nullptr)
      , m_FirstT(0.f)
      , m_HitColliders()
      , m_Ray()
      , m_IntersectionPoint()
      , m_Normal()
    {

    }

    // did this ray hit anything at all?
    bool Hit(void)
    {
      return m_FirstCollider != nullptr;
    }
  };

  class RayCastMgr
  {
  private:
    // all the raycasts that occurred this last frame, if we call a raycast
    // function and it doesn't hit anything, don't insert into this vector
    std::vector<RayCastResult> m_Results;

    typedef bool(*RayCastFunc)(Ray &ray, Core::GameComponent *pCollider, float &t);
    typedef std::unordered_map<Base::EComponent, RayCastFunc> TRayCastHelperMap;
    TRayCastHelperMap m_RayCastHelpers;
    typedef void(*FindNormal)(RayCastResult &result);
    typedef std::unordered_map<Base::EComponent, FindNormal> TNormalHelperMap;
    TNormalHelperMap m_RayCastNormalHelpers;

    static bool RayToSphere(Ray &ray, Core::GameComponent *pCollider, float &t);
    static bool RayToBox(Ray &ray, Core::GameComponent *pCollider, float &t);
    static bool RayToCapsule(Ray &ray, Core::GameComponent *pCollider, float &t);

    static void FindSphereNormal(RayCastResult &result);
    static void FindBoxNormal(RayCastResult &result);
    static void FindCapsuleNormal(RayCastResult &result);
  public:
    RayCastMgr(void);

    // each frame we clear the vector of all positive raycasts from last frame
    void ClearResults(void);
    void DrawRay(Ray const &ray);

    RayCastResult const &GetResult(unsigned nIdx) const;
    RayCastResult &GetResult(unsigned nIdx);

    // cast a ray into the scene and collects all positive ray collisions
    // against all colliders that are in the "checkList"
    // as raycast result object, ignores any colliders in the ignore list
    // we also take a list of all colliders to check against,
    // will be implicitely passed from the physics system
    bool CastRay(Ray &ray, std::vector<Core::GameComponent *> &checkList,
      RayCastResult &result, float fMaxDistance = FLT_MAX,
      std::vector<Core::GameComponent*> const &ignoreList = std::vector<Core::GameComponent*>());

    // casts a ray and specifically tests against pCollider only
    bool CastRay(Ray &ray, Core::GameComponent *pCollider, RayCastResult &result,
      float fMaxDistance = FLT_MAX);
  };
}

#endif