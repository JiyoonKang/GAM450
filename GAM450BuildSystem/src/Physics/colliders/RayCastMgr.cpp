/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: RayCastMgr.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "physics/colliders/RayCastMgr.h"

#include "core/components/transform/CTransform.h"
#include "core/components/base/Component.h"

#include "core/engine/factory/Factory.h"
#include "core/engine/core/Engine.h"
#include "core/types/space/Space.h"

#include "physics/colliders/BoxCollider.h"
#include "Physics/colliders/CapsuleCollider.h"
#include "Physics/colliders/SphereCollider.h"

#include "core/types/object/Object.h"

namespace Physics
{
  bool RayCastMgr::RayToSphere(Ray &ray, Core::GameComponent *pCollider, float &t)
  {
    // grab the transform
    Core::Transform *pTransform = 
      pCollider->GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return false;

    SphereCollider *pSphere = dynamic_cast<SphereCollider*>(pCollider);
    if (!pSphere)
      return false;

    float4x4 Orientation = pTransform->GetRotationMatrix();
    float4 Position = pTransform->GetTranslation();

    // transform the ray
    Ray rayCopy(ray);
    rayCopy.m_Start = RotatePointIntoBasis(Orientation, Position, ray.m_Start);
    rayCopy.m_Direction = RotateVectorIntoBasis(Orientation, ray.m_Direction);

    // perform the test
    bool bResult = RayIntersectionTest(rayCopy, pSphere->GetShape());
    t = rayCopy.m_IntersectionTime;
    
    return bResult;
  }

  void RayCastMgr::FindSphereNormal(RayCastResult &result)
  {
    // need my transform
    Core::Transform *pTransform = result.m_FirstCollider->GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return;

    // the vector from my contact point to my center of my sphere is the normal
    float4 n = result.m_IntersectionPoint - pTransform->GetTranslation();
    result.m_Normal = n.Normal();
  }

  bool RayCastMgr::RayToBox(Ray &ray, Core::GameComponent *pCollider, float &t)
  {
    Core::Transform *pTransform =
      pCollider->GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return false;

    BoxCollider *pBox = dynamic_cast<BoxCollider*>(pCollider);
    if (!pBox)
      return false;

    float4x4 Orientation = pTransform->GetRotationMatrix();
    float4 Position = pTransform->GetTranslation();

    // transform the ray
    Ray rayCopy(ray);
    rayCopy.m_Start = RotatePointIntoBasis(Orientation, Position, ray.m_Start);
    rayCopy.m_Direction = RotateVectorIntoBasis(Orientation, ray.m_Direction);

    // perform the test
    bool bResult = RayIntersectionTest(rayCopy, pBox->GetShape());
    t = rayCopy.m_IntersectionTime;

    return bResult;
  }

  void RayCastMgr::FindBoxNormal(RayCastResult &result)
  {
    // need the rotations and half extents of this box
    Core::Transform *pTransform = result.m_FirstCollider->GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return;

    BoxCollider *pBox = dynamic_cast<BoxCollider*>(result.m_FirstCollider);
    if (!pBox)
      return;

    float4x4 basis = pTransform->GetRotationMatrix();
    float4 pos = pTransform->GetTranslation();
    float4 half = pBox->GetHalfExtents();

    for (unsigned nIdx = 0; nIdx < 3; ++nIdx)
    {
      // check the positive plane
      float4 n = GetColumn(basis, nIdx);
      float4 p = pos + n * half[nIdx];

      // make a plane from this equations
      // n is the normal, n.Dot(p) is the distance from the origin
      Plane plane(n, n.Dot(p));
      // if the point is on the plane, this is our normal
      if (plane.PointOnPlane(result.m_IntersectionPoint))
      {
        result.m_Normal = n;
        return;
      }

      // now check the negative plane
      n = -n;
      p = pos + n * half[nIdx];
      plane.SetNormal(n);
      plane.SetDistance(n.Dot(p));
      if (plane.PointOnPlane(result.m_IntersectionPoint))
      {
        result.m_Normal = n;
        return;
      }
    }

    // if we got here then something weird happened
    //assert(false);
  }

  bool RayCastMgr::RayToCapsule(Ray &ray, Core::GameComponent *pCollider, float &t)
  {
    Core::Transform *pTransform =
      pCollider->GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return false;

    CapsuleCollider *pCapsule = dynamic_cast<CapsuleCollider*>(pCollider);
    if (!pCapsule)
      return false;

    float4x4 Orientation = pTransform->GetRotationMatrix();
    float4 Position = pTransform->GetTranslation();

    // transform the ray
    Ray rayCopy(ray);
    rayCopy.m_Start = RotatePointIntoBasis(Orientation, Position, ray.m_Start);
    rayCopy.m_Direction = RotateVectorIntoBasis(Orientation, ray.m_Direction);

    // perform the test
    bool bResult = RayIntersectionTest(rayCopy, pCapsule->GetShape());
    t = rayCopy.m_IntersectionTime;

    return bResult;
  }

  void RayCastMgr::FindCapsuleNormal(RayCastResult &result)
  {
    // need to make a line from the two points of my capsule in world space
    Core::Transform *pTransform = result.m_FirstCollider->GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return;

    // need my capsule
    CapsuleCollider *pCapsule = dynamic_cast<CapsuleCollider*>(result.m_FirstCollider);

    float4 pos = pTransform->GetTranslation();
    float4x4 basis = pTransform->GetRotationMatrix();
    // get my local top and bottom
    // rotate them into world space, so out of the basis of the capsule
    float4 top = RotatePointOutOfBasis(basis, pos, pCapsule->GetTopPoint());
    float4 bottom = RotatePointOutOfBasis(basis, pos, pCapsule->GetBottomPoint());

    // find the closest point of the line to the intersection point
    Line line(bottom, top);
    float4 closestPoint = line.GetClosestPoint(result.m_IntersectionPoint);

    // intersection - closest point is our normal
    float4 n = result.m_IntersectionPoint - closestPoint;
    result.m_Normal = n.Normal();
  }

  RayCastMgr::RayCastMgr(void)
    : m_Results()
    , m_RayCastHelpers()
  {
    m_RayCastHelpers[eBoxCollider] = RayToBox;
    m_RayCastHelpers[eSphereCollider] = RayToSphere;
    m_RayCastHelpers[eCapsuleCollider] = RayToCapsule;

    m_RayCastNormalHelpers[eBoxCollider] = FindBoxNormal;
    m_RayCastNormalHelpers[eSphereCollider] = FindSphereNormal;
    m_RayCastNormalHelpers[eCapsuleCollider] = FindCapsuleNormal;
  }

  void RayCastMgr::ClearResults(void)
  {
    m_Results.clear();
  }

  void RayCastMgr::DrawRay(Ray const &ray)
  {

  }

  RayCastResult const &RayCastMgr::GetResult(unsigned nIdx) const
  {
    return m_Results[nIdx];
  }

  RayCastResult &RayCastMgr::GetResult(unsigned nIdx)
  {
    return m_Results[nIdx];
  }

  bool RayCastMgr::CastRay(Ray &ray, std::vector<Core::GameComponent*> &checkList, 
    RayCastResult &result, float fMaxDistance, 
    std::vector<Core::GameComponent*> const &ignoreList)
  {
    RayCastResult StoredResult;
    for each(auto pCurrCollider in checkList)
    {
      // if this collider is in the ignore list, skip over
      auto it = std::find_if(ignoreList.begin(), ignoreList.end(), [&](Core::GameComponent *pIgnored) {
        if (pIgnored == pCurrCollider)
          return true;
        else
          return false;
      });

      // if it found anything, just skip over this one
      if (it != ignoreList.end())
        continue;

      // perform the actual test
      float t = 0.f;
      if (m_RayCastHelpers[pCurrCollider->typeID](ray, pCurrCollider, t) && t <= fMaxDistance)
        result.m_HitColliders.push_back(std::make_pair(t, pCurrCollider));
    }

    // if we didn't hit anything, just return false
    if (result.m_HitColliders.size() == 0)
      return false;

    // once we are done testing, find the minimum t value this is the first
    // collider, and the values we fill in the raycast result
    float tMin = FLT_MAX;
    unsigned nColliderIdx = static_cast<unsigned>(-1);
    for (unsigned nIdx = 0; 
      nIdx < static_cast<unsigned>(result.m_HitColliders.size()); ++nIdx)
    {
      if (result.m_HitColliders[nIdx].first < tMin)
      {
        tMin = result.m_HitColliders[nIdx].first;
        nColliderIdx = nIdx;
      }
    }

    // there wasn't a minumum? something bad happened
    assert(nColliderIdx != static_cast<unsigned>(-1));
    if (nColliderIdx == static_cast<unsigned>(-1))
      return false;

    result.m_FirstCollider = result.m_HitColliders[nColliderIdx].second;
    result.m_FirstT = tMin;
    result.m_IntersectionPoint = ray.m_Start + ray.m_Direction * tMin;
    result.m_Ray = ray;

    ray.m_IntersectionTime = tMin;

    // TODO: Compute the normal
    
	m_RayCastNormalHelpers[result.m_FirstCollider->typeID](result);

    m_Results.push_back(result);
    return true;
  }

  bool RayCastMgr::CastRay(Ray &ray, Core::GameComponent *pCollider, 
    RayCastResult &result, float fMaxDistance)
  {
    float t = 0.f;
    if (m_RayCastHelpers[pCollider->typeID](ray, pCollider, t) && t <= fMaxDistance)
    {
      result.m_HitColliders.push_back(std::make_pair(t, pCollider));
      result.m_FirstCollider = pCollider;
      result.m_FirstT = t;
      result.m_IntersectionPoint = ray.m_Start + ray.m_Direction * t;
      result.m_Ray = ray;

      ray.m_IntersectionTime = t;

      m_RayCastNormalHelpers[result.m_FirstCollider->typeID](result);

      m_Results.push_back(result);

      return true;
    }
    else
      return false;
  }
}