/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: CollisionMgr.cpp 
 * Author: Jonah Dube
 * Author: Daniel Oliveira
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include <pch\precompiled.h>

#include <assert.h>

#include "physics/colliders/CollisionMgr.h"
#include "physics/colliders/BoxCollider.h"
#include "physics/colliders/CapsuleCollider.h"
#include "physics/colliders/SphereCollider.h"

#include "physics/dynamics/RigidBody.h"

#include "core/types/object/Object.h"
#include "core/components/transform/CTransform.h"
#include "core/types/space/Space.h"
#include "base/util/math/Float4x4.h"

namespace
{
  using namespace Physics;

  struct ClosestAxisData
  {
    float4 Center;
    float4 Axis;
    float4 ClosestPoint;
    float fLengthSq;
  };

  template <typename T>
  unsigned IndexSmallestElement(T *pData, unsigned nCount)
  {
    if (nCount < 1)
      return static_cast<unsigned>(-1);

    if (nCount == 1)
      return 0;

    unsigned smallest = 0;
    T start = pData[0];

    for (unsigned nIdx = 0; nIdx < nCount; ++nIdx)
    {
      if (pData[nIdx] < start)
      {
        smallest = nIdx;
        start = pData[nIdx];
      }
    }

    return smallest;
  }

  template <typename T>
  unsigned IndexLargestElement(T *pData, unsigned nCount)
  {
    if (nCount < 1)
      return static_cast<unsigned>(-1);

    if (nCount == 1)
      return 0;

    unsigned largest = 0;
    T start = pData[0];

    for (unsigned nIdx = 0; nIdx < nCount; ++nIdx)
    {
      if (pData[nIdx] > start)
      {
        largest = nIdx;
        start = pData[nIdx];
      }
    }

    return largest;
  }

  ClosestAxisData GetClosestAxis(float4 const &topPoint, float4 const &bottomPoint, float4 const &capsulePos, float4 const &boxExtents)
  {
    // 0 is the top sphere, 1 is the bottom sphere, 2 is the cylinder axis
    float4 OrigArray[3] = {topPoint, bottomPoint, capsulePos};
    float4 CenterArray[3] = {float4()};
    float4 AxisArray[3] = {float4()};
    float4 ClosestPointArray[3] = {float4()};
    float LengthSqArray[3] = {0.f};

    for (unsigned nIdx = 0; nIdx < 3; ++nIdx)
    {
      CenterArray[nIdx] = float4(
        Clamp(OrigArray[nIdx].x, -boxExtents.x, boxExtents.x),
        Clamp(OrigArray[nIdx].y, -boxExtents.y, boxExtents.y),
        Clamp(OrigArray[nIdx].z, -boxExtents.z, boxExtents.z), 0.f);

      ClosestPointArray[nIdx] = OrigArray[nIdx];
      // we actually do a line closest line test when it's the capsule center
      if (nIdx == 2)
      {
        Line line(topPoint, bottomPoint);
        ClosestPointArray[nIdx] = line.GetClosestPoint(CenterArray[nIdx]);
      }

      AxisArray[nIdx] = ClosestPointArray[nIdx] - CenterArray[nIdx];
      LengthSqArray[nIdx] = AxisArray[nIdx].LengthSq();
    }

    unsigned nIdx = IndexSmallestElement<float>(LengthSqArray, 3);

    return ClosestAxisData {CenterArray[nIdx], AxisArray[nIdx],
      ClosestPointArray[nIdx], LengthSqArray[nIdx]};
  }

  unsigned FindMinimumAxisOnBox(float4 const &spherePos)
  {
    float overlap = std::numeric_limits<float>::max();
    float4 axis[3] = {float4(1.f, 0.f, 0.f, 0.f),
      float4(0.f, 1.f, 0.f, 0.f),
      float4(0.f, 0.f, 1.f, 0.f)};
    unsigned nIdx = 0;
    for (unsigned i = 0; i < 3; ++i)
    {
      float dis = spherePos.Dot(axis[i]);
      if (dis < overlap)
      {
        overlap = dis;
        nIdx = i;
      }
    }

    // now check the negative values
    for (unsigned i = 0; i < 3; ++i)
    {
      float dis = spherePos.Dot(-axis[i]);
      if (dis < overlap)
      {
        overlap = dis;
        nIdx = i;
      }
    }

    return nIdx;
  }

  bool AnyParallelEdges(float4 *axis, unsigned nStart, unsigned nEnd)
  {
    for (unsigned i = nStart; i < nEnd; ++i)
    {
      // if the edges cross product are "zero" length, then they
      // are very close to parallel
      if (Equals(axis[i].LengthSq(), 0.f))
        return true;
    }

    return false;
  }

  unsigned FindIncidentFace(float4 const &normal, float4x4 const &matInc, float &negateInc)
  {
    // we are looking for the most negative value
    float overlap = FLT_MAX;
    unsigned index = 0;
    // run through all positive basis vectors
    for (unsigned i = 0; i < 3; ++i)
    {
      float dis = normal.Dot(GetColumn(matInc, i));
      // we found a new most negative value
      if (dis < overlap)
      {
        overlap = dis;
        index = i;
        negateInc = 1.f;
      }
    }
    // run through all negative basis vectors
    for (unsigned i = 0; i < 3; ++i)
    {
      float dis = normal.Dot(-GetColumn(matInc, i));
      if (dis < overlap)
      {
        overlap = dis;
        index = i;
        negateInc = -1.f;
      }
    }

    return index;
  }

  void GetIncidentPoints(std::vector<float4> &points, float4 const &pos, float4 *extents,
    unsigned nIdx, float fNegate)
  {
    float4 p0 = pos, p1 = pos, p2 = pos, p3 = pos;

    p0 += extents[nIdx] * fNegate + 
      extents[(nIdx + 1) % 3] * fNegate + 
      extents[(nIdx + 2) % 3];

    p1 += extents[nIdx] * fNegate +
      extents[(nIdx + 1) % 3] * fNegate -
      extents[(nIdx + 2) % 3];

    p2 += extents[nIdx] * fNegate -
      extents[(nIdx + 1) % 3] * fNegate -
      extents[(nIdx + 2) % 3];

    p3 += extents[nIdx] * fNegate -
      extents[(nIdx + 1) % 3] * fNegate +
      extents[(nIdx + 2) % 3];

    points.push_back(p0);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
  }

  void ClipPoints(float xHalf, float4 const &start,
    float4 const &end, float x0, float x1,
    std::vector<float4> &points, bool negate)
  {
    // grab the difference between the half width and the point's x value
    float x0Dif = x0 - xHalf;
    float x1Dif = x1 - xHalf;

    if (negate)
    {
      x0Dif = -x0Dif;
      x1Dif = -x1Dif;
    }

    std::vector<float4> newPoints;

    // if x0Dif is negative, start is inside
    // if x1Dif is negative, end is inside
    if (x0Dif <= 0.f && x1Dif > 0.f)
    {
      // find intersection = i and add it
      float t = (xHalf - x0) / (x1 - x0);
      float4 i;
      // if between 0 and 1
      if (t >= 0.f && t <= 1.f)
      {
        i = start + (end - start) * t;
        points.push_back(i);
      }
    }
    else if (x0Dif > 0.f && x1Dif <= 0.f)
    {
      // find intersection = i and add it
      float t = (xHalf - x0) / (x1 - x0);
      float4 i;

      if (t >= 0.f && t <= 1.f)
      {
        i = start + (end - start) * t;
        points.push_back(i);
        points.push_back(end);
      }
    }
    else if (x0Dif <= 0.f && x1Dif <= 0.f)
    {
      // just add the end point
      points.push_back(end);
    }
  }

  // TODO: rework this function, it's disgusting
  void ClipToReferenceFace(float xHalf, float yHalf,
    std::vector<float4> &points)
  {
    // run through all the points in the vector start with x,
    // positive and negative and clip, run through positive x
    std::vector<float4> clippedPoints;

    for (unsigned i = 0; i < points.size(); ++i)
    {
      // if it runs past the end, it will wrap back to 0
      unsigned j = (i + 1) % points.size();

      // grab points at these indexes
      float4 start = points[i];
      float4 end = points[j];

      // now clip these two points
      ClipPoints(xHalf, start, end, start.x, end.x, clippedPoints, false);
    }

    // now my new points are my clipped points
    points = clippedPoints;
    clippedPoints.clear();

    // run through negative x
    for (unsigned i = 0; i < points.size(); ++i)
    {
      unsigned j = (i + 1) % points.size();

      // grab the correct points
      float4 start = points[i];
      float4 end = points[j];

      ClipPoints(-xHalf, start, end, start.x, end.x, clippedPoints, true);
    }

    points = clippedPoints;
    clippedPoints.clear();

    // then with y, positive and negative clip points run through y
    for (unsigned i = 0; i < points.size(); ++i)
    {
      unsigned j = (i + 1) % points.size();

      float4 start = points[i];
      float4 end = points[j];

      ClipPoints(yHalf, start, end, start.y, end.y, clippedPoints, false);
    }

    points = clippedPoints;
    clippedPoints.clear();

    for (unsigned i = 0; i < points.size(); ++i)
    {
      unsigned j = (i + 1) % points.size();

      float4 start = points[i];
      float4 end = points[j];

      ClipPoints(-yHalf, start, end, start.y, end.y, clippedPoints, true);
    }

    points = clippedPoints;
  }

  void CullPoints(std::vector<float4> &points,
    float4 const &normal, float4 const &refFace,
    std::vector<float> &penetration)
  {
    // create a vector from the reference faces center to each point in my vector
    // dot with the normal, if positive, then throw it away it is not underneath our plane
    float4 v;
    std::vector<float4> newPoints;
    for (auto it = points.begin(); it != points.end(); ++it)
    {
      v = (*it) - refFace;
      float dist = v.Dot(normal);
      // if the dot product is negative, keep it
      if (dist < 0.f)
      {
        newPoints.push_back(v + refFace);
        penetration.push_back(std::abs(dist));
      }
    }

    points = newPoints;
  }

  // will go through and if there are more than 4 points we will trim it
  // so we only have 4 points, take from Dirk Gregarious GDC Lecture 2015
  void ConsolidatePoints(std::vector<float4> &/*points*/)
  {

  }

  Line GetEdge(float4 const &half,float4 const &pos, float4 const &normal,
    float4x4 const &mat)
  {
    // half are the half extensts of the box, s is the support vector
    float4 p0;
    float4 p1;
    Line l;
    float4 n = mat.Transpose() * normal;

    // add or subtract the half extents based on if the support vecotr is negative
    if (n.x < 0.f)
      p0.x = -half.x;
    else
      p0.x = +half.x;

    if (n.y < 0.f)
      p0.y = -half.y;
    else
      p0.y = +half.y;

    if (n.z < 0.f)
      p0.z = -half.z;
    else
      p0.z = +half.z;

    p1 = p0;
    if (Equals(n.x, 0.f))
      p1.x = -p1.x;
    else if (Equals(n.y, 0.f))
      p1.y = -p1.y;
    else if (Equals(n.z, 0.f))
      p1.z = -p1.z;

    l.m_P0 = RotatePointOutOfBasis(mat, pos, p0);
    l.m_P1 = RotatePointOutOfBasis(mat, pos, p1);

    return l;
  }
}

namespace Physics
{

  CollisionMessageMgr::CollisionMessageMgr(int maxAge) : m_MaxAge(maxAge)
  {
  }

  void CollisionMessageMgr::NoteCollision(RigidBody * a, RigidBody * b, 
    std::vector<MessageHold>& messages)
  {
    // Check if A is in MessageMap
    auto iter = m_MessageMap.find(a);
    if (iter == m_MessageMap.end())
    {
      // If not then enter collision and push_back a new entry
      messages.push_back({ MessageType::ENTER, a, b });
      m_MessageMap[a].push_back(std::make_pair(b, 0));
    }
    else
    {
      // If A is in there, look for B
      auto& vec = iter->second;
      for (int i = 0; i < vec.size(); ++i)
      {
        if (vec[i].first == b)
        {
          vec[i].second = 0;
          return;
        }
      }

      // B not found
      messages.push_back({ MessageType::ENTER, a, b });
      vec.push_back(std::make_pair(b, 0));
      return;
    }
  }

  void CollisionMessageMgr::AgeCollisions(std::vector<MessageHold>& messages)
  {
    // For each element in the array, increment the age and either continue the
    // collision with callback, or exit it with exitCallback if it's too old
    std::vector<RigidBody*> keysToErase; // Keys to erase from unordered_map

    for (auto i = m_MessageMap.begin(); i != m_MessageMap.end(); ++i)
    {
      auto& vec = i->second;
      for (auto j = vec.begin(); j != vec.end();)
      {
        ++(j->second);
        if (j->second > m_MaxAge)
        {
          // Shift iterator j back when deleting entry
          messages.push_back({ MessageType::EXIT, i->first, j->first });
          j = vec.erase(j);
          continue;
        }
        else
          messages.push_back({ MessageType::CONTINUE, i->first, j->first });

        ++j;
      }
      if (vec.size() == 0) // Vector is empty after updating
        keysToErase.push_back(i->first);
    }

    for (int i = 0; i < keysToErase.size(); ++i)
      m_MessageMap.erase(keysToErase[i]);
  }

  void CollisionMessageMgr::ClearBodies()
  {
    m_MessageMap.clear();
  }

  void CollisionMessageMgr::RemoveBody(RigidBody* body)
  {
	  m_MessageMap.erase(body);

    std::vector<RigidBody*> keysToErase; // Keys to erase from unordered_map

    for (auto i = m_MessageMap.begin(); i != m_MessageMap.end(); ++i)
    {
      auto& vec = i->second;
      for (auto j = vec.begin(); j != vec.end();)
      {
        if (j->first == body)
        {
          j = vec.erase(j);
          continue;
        }

        ++j;
      }
      if (vec.size() == 0) // Vector is empty after updating
        keysToErase.push_back(i->first);
    }

    for (int i = 0; i < keysToErase.size(); ++i)
      m_MessageMap.erase(keysToErase[i]);
  }

	CollisionMgr::CollisionMgr(void) : m_MessageMgr(1)
  {
    // initialize the collision functions that will be used for collision detection
    auto &boxPickers = m_CollisionPicker[eBoxCollider];
    boxPickers[eBoxCollider] = CollisionMgr::BoxToBox;
    boxPickers[eSphereCollider] = CollisionMgr::BoxToSphere;
    boxPickers[eCapsuleCollider] = CollisionMgr::BoxToCapsule;

    auto &spherePickers = m_CollisionPicker[eSphereCollider];
    spherePickers[eSphereCollider] = CollisionMgr::SphereToSphere;
    spherePickers[eBoxCollider] = CollisionMgr::SphereToBox;
    spherePickers[eCapsuleCollider] = CollisionMgr::SphereToCapsule;

    auto &capsulePickers = m_CollisionPicker[eCapsuleCollider];
    capsulePickers[eCapsuleCollider] = CollisionMgr::CapsuleToCapsule;
    capsulePickers[eBoxCollider] = CollisionMgr::CapsuleToBox;
    capsulePickers[eSphereCollider] = CollisionMgr::CapsuleToSphere;

    m_EnterCallback = [](RigidBody*, RigidBody*)->void { return; };
    m_Callback = [](RigidBody*, RigidBody*)->void { return; };
    m_ExitCallback = [](RigidBody*, RigidBody*)->void { return; };
  }

  bool CollisionMgr::Collide(Core::GameComponent *a, Core::GameComponent *b)
  {
    Manifold m;

    if (m_CollisionPicker[a->typeID][b->typeID](a, b, m))
    {
      // Mix material properties like friction and restitution
      const Material& matA = dynamic_cast<Collider*>(a)->GetMaterial();
      const Material& matB = dynamic_cast<Collider*>(b)->GetMaterial();
      m.MixMaterials(matA, matB);
      
      m_Contacts.push_back(m);
      return true;
    }

    return false;
  }

  void CollisionMgr::ClearContacts(void)
  {
    m_Contacts.clear();
  }

  void CollisionMgr::CacheContacts(void)
  {
    // clear all cached contacts, only hold this frames
    m_CachedContacts.clear();
    for (auto it = m_Contacts.begin(); it != m_Contacts.end(); ++it)
    {
      // how did this get here???
      //assert(it->m_ContactPoints.size() != 0);
      if (it->m_ContactPoints.size() == 0)
        continue;

      m_CachedContacts[it->m_pBodyA][it->m_pBodyB] = *it;
    }
  }

  void CollisionMgr::SetCollisionCallbacks(const TCollisionCallback& enterCallback, 
    const TCollisionCallback& callback, const TCollisionCallback& exitCallback)
  {
    m_EnterCallback = enterCallback;
    m_Callback = callback;
    m_ExitCallback = exitCallback;
  }

  void CollisionMgr::ManageCollisionMessages()
  {
    std::vector<MessageHold> messages;
    for (int i = 0; i < m_Contacts.size(); ++i)
    {
      RigidBody* a = m_Contacts[i].m_pBodyA;
      RigidBody* b = m_Contacts[i].m_pBodyB;
      m_MessageMgr.NoteCollision(a, b, messages);
      m_MessageMgr.NoteCollision(b, a, messages);
    }

    m_MessageMgr.AgeCollisions(messages);

    for (int i = 0; i < messages.size(); ++i)
    {
      MessageHold m = messages[i];
      switch (m.type)
      {
        case MessageType::ENTER:
          m_EnterCallback(m.a, m.b);
          break;
        case MessageType::CONTINUE:
          m_Callback(m.a, m.b);
          break;
        case MessageType::EXIT:
          m_ExitCallback(m.a, m.b);
          break;
      }
    }
  }

  void CollisionMgr::RemoveRigidBodyMessages(RigidBody* pRigidBody)
  {
    m_MessageMgr.RemoveBody(pRigidBody);
  }

  void CollisionMgr::ClearRigidBodyMessages()
  {
    m_MessageMgr.ClearBodies();
  }

	void CollisionMgr::PrestepCollisionResolver(void)
  {
    // perform the presteps needed on the contacts and joints
    for (auto it = m_Contacts.begin();
      it != m_Contacts.end(); ++it)
    {
      if (it->m_pBodyA->m_bGhost || it->m_pBodyB->m_bGhost)
        continue;

      it->Step();

      // if this is a cached contact, we might be able to warm start it
      auto jt = m_CachedContacts.find(it->m_pBodyA);
      if (jt != m_CachedContacts.end())
      {
        auto kt = jt->second.find(it->m_pBodyB);
        // this contact existed, if the contact points are close enough together
        // we can actually perform a warm start
        if (kt != jt->second.end())
        {
          for (unsigned nIdx = 0; nIdx < it->m_ContactPoints.size(); ++nIdx)
            if (CompareContactPoints(kt->second, *it, nIdx))
              WarmStart(kt->second, *it, nIdx);
        }
      }
    }

    // for each joint
    // step
  }

  void CollisionMgr::ResolveCollisions(float dt, unsigned nIterations)
  {
    for (unsigned nIdx = 0; nIdx < nIterations; ++nIdx)
      for (auto it = m_Contacts.begin(); it != m_Contacts.end(); ++it)
        if (!it->m_pBodyA->m_bGhost && !it->m_pBodyB->m_bGhost) // Ignore collisions for kinematic objs
          it->ApplyNormalImpulse(dt);

    for (unsigned nIdx = 0; nIdx < nIterations; ++nIdx)
      for (auto it = m_Contacts.begin(); it != m_Contacts.end(); ++it)
        if (!it->m_pBodyA->m_bGhost && !it->m_pBodyB->m_bGhost) // Ignore collisions for kinematic objs
          it->ApplyTangentImpulse();

    // for(nIdx < m_nIterations)
    // for each joint
    // apply impulse
  }

  bool CollisionMgr::CompareContactPoints(Manifold &cachedContact, 
    Manifold &contact, unsigned nIdx)
  {
    if (!contact.m_pBodyA || !contact.m_pBodyB)
      return false;

    assert(cachedContact.m_ContactPoints.size() != 0 ||
      contact.m_ContactPoints.size() != 0);

    // if this new contact has more points than our cached contact treat it as a new contact
    if (nIdx >= cachedContact.m_ContactPoints.size())
      return false;

    Core::Transform *pTransform = 
      contact.m_pBodyA->GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return false;

    float4 ContactPosition = pTransform->GetTranslation();
    float4x4 ContactOrientation = pTransform->GetRotationMatrix();

    float4 LocalCached = cachedContact.m_ContactPoints[nIdx];
    LocalCached = RotatePointIntoBasis(ContactOrientation, ContactPosition, LocalCached);
    float4 LocalNew = contact.m_ContactPoints[nIdx];
    LocalNew = RotatePointIntoBasis(ContactOrientation, ContactPosition, LocalNew);

    // if their difference is very small we use the data from the cached contact
    if ((LocalNew - LocalCached).LengthSq() < EPSILON)
      return true;
    else
      return false;
  }

  void CollisionMgr::WarmStart(Manifold &cachedContact, Manifold &contact, 
    unsigned nIdx)
  {
    if (!contact.m_pBodyA || !contact.m_pBodyB)
      return;

    Core::Transform *pTransformA = 
      contact.m_pBodyA->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformB = 
      contact.m_pBodyB->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformA || !pTransformB)
      return;

    float4 PositionA = pTransformA->GetTranslation();
    float4 PositionB = pTransformB->GetTranslation();

    float4 rA = contact.m_ContactPoints[nIdx] - PositionA;
    float4 rB = contact.m_ContactPoints[nIdx] - PositionB;

    // create the "velocity vector"
    VelocityVector Velocity =
    {contact.m_pBodyA->GetVelocity(), contact.m_pBodyA->GetAngularVelocity(),
    contact.m_pBodyB->GetVelocity(), contact.m_pBodyB->GetAngularVelocity()};

    // and the "mass matrix"
    MassMatrix Masses =
    {contact.m_pBodyA->GetInverseMass(), contact.m_pBodyA->GetInverseInertiaTensorW(),
    contact.m_pBodyB->GetInverseMass(), contact.m_pBodyB->GetInverseInertiaTensorW()};

    // the old lambda
    float fCachedLambda = cachedContact.m_NormalImpulse[nIdx];
    contact.m_NormalImpulse[nIdx] = fCachedLambda;

    // create the contact normal jacobian
    Jacobian NormalJacobian =
    {-contact.m_ContactNormal, -(rA.Cross(contact.m_ContactNormal)),
    contact.m_ContactNormal, rB.Cross(contact.m_ContactNormal)};

    // find the impulse
    Jacobian Impulse = Masses.Solve(NormalJacobian) * fCachedLambda;
    // add to our velocity
    Velocity += Impulse;

    // apply the friction for this point
    // use two tangent vector orthogonal to our contact normal
    fCachedLambda = cachedContact.m_TangentImpulseA[nIdx];
    contact.m_TangentImpulseA[nIdx] = fCachedLambda;

    Jacobian TangentJacobian =
    {-contact.m_TangentA, -(rA.Cross(contact.m_TangentA)),
    contact.m_TangentA, rB.Cross(contact.m_TangentA)};

    // find and apply the impulse
    Impulse = Masses.Solve(TangentJacobian) * fCachedLambda;
    Velocity += Impulse;

    // find the second tangent impulse
    fCachedLambda = cachedContact.m_TangentImpulseB[nIdx];
    contact.m_TangentImpulseB[nIdx] = fCachedLambda;

    TangentJacobian =
    {-contact.m_TangentB, -(rA.Cross(contact.m_TangentB)),
    contact.m_TangentB, rB.Cross(contact.m_TangentB)};

    // find and apply the impulse
    Impulse = Masses.Solve(TangentJacobian) * fCachedLambda;
    Velocity += Impulse;

    // apply our new velocity to our bodies
    contact.m_pBodyA->SetVelocity(Velocity.m_LinA);
    contact.m_pBodyA->SetAngularVelocity(Velocity.m_AngA);
    contact.m_pBodyB->SetVelocity(Velocity.m_LinB);
    contact.m_pBodyB->SetAngularVelocity(Velocity.m_AngB);
  }

  bool CollisionMgr::BoxToBox(Core::GameComponent *a, Core::GameComponent *b, Manifold &m)
  {
    Collider *colA = dynamic_cast<Collider*>(a);
    Collider *colB = dynamic_cast<Collider*>(b);

    BoxCollider *boxA = static_cast<BoxCollider*>(colA);
    BoxCollider *boxB = static_cast<BoxCollider*>(colB);

    Core::Transform *pTransformA = 
      a->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformB =
      b->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformA || !pTransformB)
      return false;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    float4 posA = pTransformA->GetTranslation();
    float4 posB = pTransformB->GetTranslation();
    float4x4 matA = pTransformA->GetRotationMatrix();
    float4x4 matB = pTransformB->GetRotationMatrix();
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    float4 halfA = boxA->GetHalfExtents();
    float4 halfB = boxB->GetHalfExtents();

    float4 basisHalfAX = GetColumn(matA, 0) * halfA.x;
    float4 basisHalfBX = GetColumn(matB, 0) * halfB.x;
    float4 basisHalfAY = GetColumn(matA, 1) * halfA.y;
    float4 basisHalfBY = GetColumn(matB, 1) * halfB.y;
    float4 basisHalfAZ = GetColumn(matA, 2) * halfA.z;
    float4 basisHalfBZ = GetColumn(matB, 2) * halfB.z;

    // for this algorithm both objects are in world space
    // the 15 axis that we check, faces of a, faces of b and the cross produce of their edges
    float4 axis[15] = {float4()};
    axis[0] = GetColumn(matA, 0);
    axis[1] = GetColumn(matA, 1);
    axis[2] = GetColumn(matA, 2);
    
    axis[3] = GetColumn(matB, 0);
    axis[4] = GetColumn(matB, 1);
    axis[5] = GetColumn(matB, 2);

    axis[6] = GetColumn(matA, 0).Cross(GetColumn(matB, 0));
    axis[7] = GetColumn(matA, 0).Cross(GetColumn(matB, 1));
    axis[8] = GetColumn(matA, 0).Cross(GetColumn(matB, 2));

    axis[9]  = GetColumn(matA, 1).Cross(GetColumn(matB, 0));
    axis[10] = GetColumn(matA, 1).Cross(GetColumn(matB, 1));
    axis[11] = GetColumn(matA, 1).Cross(GetColumn(matB, 2));

    axis[12] = GetColumn(matA, 2).Cross(GetColumn(matB, 0));
    axis[13] = GetColumn(matA, 2).Cross(GetColumn(matB, 1));
    axis[14] = GetColumn(matA, 2).Cross(GetColumn(matB, 2));

    // if there are any parallel axis, then don't bother checking separation checks
    bool bParallelEdges = AnyParallelEdges(axis, 6, 15);
    if (!bParallelEdges)
    {
      for (unsigned nIdx = 6; nIdx < 15; ++nIdx)
        axis[nIdx].Normalize();
    }

    float overlap = -FLT_MAX;
    unsigned smallestAxisIdx = static_cast<unsigned>(-1);
    float ra = 0.f, rb = 0.f, tValue = 0.f;
    float4 t = posB - posA;

    // check my axis for the least penetration
    for (unsigned nIdx = 0; nIdx < 15; ++nIdx)
    {
      // there were parallel edges, we don't have to check them
      if (bParallelEdges && nIdx > 5)
        break;

      // distance from the axis from the center
      tValue = std::abs(t.Dot(axis[nIdx]));
      ra = std::abs(basisHalfAX.Dot(axis[nIdx])) + std::abs(basisHalfAY.Dot(axis[nIdx])) + std::abs(basisHalfAZ.Dot(axis[nIdx]));
      rb = std::abs(basisHalfBX.Dot(axis[nIdx])) + std::abs(basisHalfBY.Dot(axis[nIdx])) + std::abs(basisHalfBZ.Dot(axis[nIdx]));

      // we found a seperating axis, we can stop
      if (tValue > (ra + rb))
        return false;
      else
      {
        // this is an overlapping axis, might be the smallest axis
        float diff = tValue - (ra + rb);
        if (diff <= 0.f && diff > overlap)
        {
          // this is the new minimum axis
          overlap = diff;
          smallestAxisIdx = nIdx;
        }
      }
    }

    // we collided!, now we have to figure out how
    m.m_ContactNormal = axis[smallestAxisIdx];

    // this was a something-face collision
    if (smallestAxisIdx < 6)
    {
      BoxCollider *boxRef = nullptr;
      BoxCollider *boxInc = nullptr;

      float4 halfRef;
      float4 halfInc;

      float4x4 matRef;
      float4x4 matInc;
      float4 posRef;
      float4 posInc;

      // if 0 - 2, a is the reference and b is the incident
      if (smallestAxisIdx < 3)
      {
        boxRef = boxA;
        boxInc = boxB;

        halfRef = halfA;
        halfInc = halfB;

        matRef = matA;
        matInc = matB;
        posRef = posA;
        posInc = posB;
      }
      else
      {
        boxRef = boxB;
        boxInc = boxA;

        halfRef = halfB;
        halfInc = halfA;

        matRef = matB;
        matInc = matA;
        posRef = posB;
        posInc = posA;
      }

      float4 normal = m.m_ContactNormal;
      if (normal.Dot(posInc - posRef) < 0.f)
        normal = -normal;

      float negateInc = 1.f;
      unsigned faceInc = FindIncidentFace(normal, matInc, negateInc);

      // construct our change of basis, z is the normal
      float4x4 basisRef(
        GetColumn(matRef, (smallestAxisIdx + 1) % 3), 
        GetColumn(matRef, (smallestAxisIdx + 2) % 3), 
        normal, 
        float4(0.f, 0.f, 0.f, 1.f));

      // find the points in the incident face to clip later in the algorithm
      std::vector<float4> incidentPoints;
      float4 extentX = GetColumn(matInc, 0) * halfInc.v[0];
      float4 extentY = GetColumn(matInc, 1) * halfInc.v[1];
      float4 extentZ = GetColumn(matInc, 2) * halfInc.v[2];

      float4 extents[3] = {extentX, extentY, extentZ};

      GetIncidentPoints(incidentPoints, posInc, extents, faceInc, negateInc);

      // the incident points are currently in world space
      // transform them to be in the space of the basis
      for (auto it = incidentPoints.begin(); it != incidentPoints.end(); ++it)
         *it = RotatePointIntoBasis(basisRef, posRef, *it);

      // get the new half widths
      float halfX = halfRef[(smallestAxisIdx + 1) % 3];
      float halfY = halfRef[(smallestAxisIdx + 2) % 3];

      ClipToReferenceFace(halfX, halfY, incidentPoints);

      // now transform back to world space
      for (auto it = incidentPoints.begin(); it != incidentPoints.end(); ++it)
        *it = RotatePointOutOfBasis(basisRef, posRef, *it);

      // move the point to the center of the reference face to cull
      float4 facePointRef = posRef + normal * halfRef[smallestAxisIdx % 3];

      CullPoints(incidentPoints, normal, facePointRef, m.m_Penetration);

      ConsolidatePoints(incidentPoints);

      if (m.m_ContactNormal.Dot(posB - posA) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      m.m_ContactPoints = incidentPoints;

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }
    // otherwise it was edge-edge collision
    else
    {
      // should we negate our normal for the negative side of the box
      if (m.m_ContactNormal.Dot(posB - posA) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      Line edgeA = GetEdge(halfA, posA, m.m_ContactNormal, matA);
      Line edgeB = GetEdge(halfB, posB, -m.m_ContactNormal, matB);

      Line closestPoints = edgeA.GetClosestPoints(edgeB);

      m.m_ContactPoints.push_back(closestPoints.m_P0);
      m.m_ContactPoints.push_back(closestPoints.m_P1);

      // penetration is the length between the points
      float4 diff = closestPoints.m_P1 - closestPoints.m_P0;
      float len = diff.Length();
      m.m_Penetration.push_back(len);
      m.m_Penetration.push_back(len);

      if (m.m_ContactNormal.Dot(posB - posA) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    return false;
  }

  bool CollisionMgr::BoxToSphere(Core::GameComponent *a, Core::GameComponent *b, Manifold &m)
  {
    Collider *colA = dynamic_cast<Collider*>(a);
    Collider *colB = dynamic_cast<Collider*>(b);

    BoxCollider *pBox = static_cast<BoxCollider*>(colA);
    SphereCollider *pSphere = static_cast<SphereCollider*>(colB);

    Core::Transform *pTransformBox =
      a->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformSphere =
      b->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformBox || !pTransformSphere)
      return false;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    float4 posBox = pTransformBox->GetTranslation();
    float4 posSphere = pTransformSphere->GetTranslation();
    float4x4 matBox = pTransformBox->GetRotationMatrix();
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    // transform the sphere into the basis of the box
    // the box is now the origin and it is axis aligned
    posSphere = RotatePointIntoBasis(matBox, posBox, posSphere);
    float4 halfExtents = pBox->GetHalfExtents();

    // we clamp the spheres center to the minimum and maximum extents to find
    // the minimum axis between the two shapes
    float4 newCenter(Clamp(posSphere.x, -halfExtents.x, halfExtents.x),
      Clamp(posSphere.y, -halfExtents.y, halfExtents.y),
      Clamp(posSphere.z, -halfExtents.z, halfExtents.z), 0.f);

    // the sphere is perfectly inside the box, won't usually happen but can
    if (Equals(newCenter.Dot(newCenter), 0.f))
    {
      m.m_Penetration.push_back(pSphere->GetRadius());
      m.m_ContactNormal = matBox * float4(0.f, 1.f, 0.f, 0.f);
      
      posSphere = RotatePointOutOfBasis(matBox, posBox, posSphere);
      m.m_ContactPoints.push_back(posSphere - m.m_ContactNormal * pSphere->GetRadius());

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    // otherwise we take the vector between the shapes and do a distance check
    float4 c(newCenter - posSphere);
    float lenSq = c.LengthSq();

    // the center of the sphere is inside the box
    if (Equals(lenSq, 0.f))
    {
      unsigned nIdx = FindMinimumAxisOnBox(posSphere);
      float4 n; n[nIdx] = posSphere[nIdx];

      m.m_Penetration.push_back(pSphere->GetRadius() + (halfExtents[nIdx] - posSphere[nIdx]));
      m.m_ContactNormal = matBox * n.Normal();

      posSphere = RotatePointOutOfBasis(matBox, posBox, posSphere);
      m.m_ContactPoints.push_back(posSphere - m.m_ContactNormal * pSphere->GetRadius());
      
      if (m.m_ContactNormal.Dot(posSphere - posBox) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    // otherwise the center is outside and it's a shallow penetration
    if (lenSq < pSphere->GetRadius() * pSphere->GetRadius())
    {
      m.m_Penetration.push_back(pSphere->GetRadius() - std::sqrt(lenSq));
      m.m_ContactNormal = matBox * c.Normal();

      posSphere = RotatePointOutOfBasis(matBox, posBox, posSphere);
      m.m_ContactPoints.push_back(posSphere - m.m_ContactNormal * pSphere->GetRadius());
      
      if (m.m_ContactNormal.Dot(posSphere - posBox) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    return false;
  }

  bool CollisionMgr::BoxToCapsule(Core::GameComponent *b, Core::GameComponent *a, Manifold &m)
  {
    //bool bResult = CapsuleToBox(b, a, m);
    //m.m_ContactNormal = -m.m_ContactNormal;
    //return bResult;

    Collider *colA = dynamic_cast<Collider*>(a);
    Collider *colB = dynamic_cast<Collider*>(b);

    CapsuleCollider *pCapsule = static_cast<CapsuleCollider*>(colA);
    BoxCollider *pBox = static_cast<BoxCollider*>(colB);

    Core::Transform *pTransformCapsule =
      a->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformBox =
      b->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformCapsule || !pTransformBox)
      return false;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    float4 posCapsule = pTransformCapsule->GetTranslation();
    float4 posBox = pTransformBox->GetTranslation();
    float4x4 matCapsule = pTransformCapsule->GetRotationMatrix();
    float4x4 matBox = pTransformBox->GetRotationMatrix();
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    float4 topPoint = posCapsule + matCapsule * pCapsule->GetTopPoint();
    float4 botPoint = posCapsule + matCapsule * pCapsule->GetBottomPoint();

    // rotate the capsule into the basis of the box, the box is now the origin and axis aligned
    posCapsule = RotatePointIntoBasis(matBox, posBox, posCapsule);
    matCapsule = matBox.Transpose() * matCapsule;

    topPoint = RotatePointIntoBasis(matBox, posBox, topPoint);
    botPoint = RotatePointIntoBasis(matBox, posBox, botPoint);
    Line axis(topPoint, botPoint);

    // clamp the capsule center to the box
    float4 halfExtents = pBox->GetHalfExtents();

    ClosestAxisData data = GetClosestAxis(topPoint, botPoint, posCapsule, halfExtents);

    if (Equals(data.fLengthSq, 0.f))
    {
      // the capsule is deep within the box, find the closets point on the
      // capsule to the center of the box, which happens to be the origin
      data.ClosestPoint = axis.GetClosestPoint(float4());

      float4 n;
      unsigned nIdx = 0;
      // if this is also zero then we are directly in the center
      if (Equals(data.ClosestPoint.LengthSq(), 0.f))
      {
        n.x = 0.f; n.y = 1.f; n.z = 0.f; n.w = 0.f;
        nIdx = 1;
      }
      else
      {
        nIdx = IndexLargestElement(data.ClosestPoint.v, 4);
        n[nIdx] = data.ClosestPoint[nIdx];
      }

      m.m_ContactNormal = matBox * n.Normal();
      m.m_Penetration.push_back(halfExtents[nIdx] - std::abs(n[nIdx]) + pCapsule->GetRadius());

      data.ClosestPoint = RotatePointOutOfBasis(matBox, posBox, data.ClosestPoint);
      m.m_ContactPoints.push_back(data.ClosestPoint - m.m_ContactNormal * pCapsule->GetRadius());

      if (m.m_ContactNormal.Dot(posBox - posCapsule) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    // can perform the proper distance check now
    if (data.fLengthSq < pCapsule->GetRadius() * pCapsule->GetRadius())
    {
      float len = std::abs(pCapsule->GetRadius() - std::sqrt(data.fLengthSq));
      m.m_Penetration.push_back(len);
      m.m_ContactNormal = matBox * data.Axis.Normal();
      data.ClosestPoint = RotatePointOutOfBasis(matBox, posBox, data.ClosestPoint);
      m.m_ContactPoints.push_back(data.ClosestPoint - m.m_ContactNormal * pCapsule->GetRadius());

      if (m.m_ContactNormal.Dot(posBox - posCapsule) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    return false;
  }

  bool CollisionMgr::SphereToSphere(Core::GameComponent *a, Core::GameComponent *b, Manifold &m)
  {
    Collider *colA = dynamic_cast<Collider*>(a);
    Collider *colB = dynamic_cast<Collider*>(b);

    SphereCollider *sphereA = static_cast<SphereCollider*>(colA);
    SphereCollider *sphereB = static_cast<SphereCollider*>(colB);

    Core::Transform *pTransformA =
      a->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformB =
      b->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformA || !pTransformB)
      return false;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    float4 posA = pTransformA->GetTranslation();
    float4 posB = pTransformB->GetTranslation();
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    float addedRaddi = sphereA->GetRadius() + sphereB->GetRadius();
    float4 vectorBetween = posB - posA;

    if (addedRaddi * addedRaddi > vectorBetween.LengthSq())
    {
      m.m_Penetration.push_back(addedRaddi - vectorBetween.Length());
      m.m_ContactNormal = vectorBetween.Normal();

      m.m_ContactPoints.push_back(posA + m.m_ContactNormal * sphereA->GetRadius());

      if (m.m_ContactNormal.Dot(posB - posA) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      // grab the rigidbody
      m.m_pBodyA = sphereA->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = sphereB->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    return false;
  }

  bool CollisionMgr::SphereToCapsule(Core::GameComponent *b, Core::GameComponent *a, Manifold &m)
  {
    //bool bResult = CapsuleToSphere(b, a, m);
    //m.m_ContactNormal = -m.m_ContactNormal;
    //return bResult;

    Collider *colA = dynamic_cast<Collider*>(a);
    Collider *colB = dynamic_cast<Collider*>(b);

    CapsuleCollider *pCapsule = static_cast<CapsuleCollider*>(colA);
    SphereCollider *pSphere = static_cast<SphereCollider*>(colB);

    Core::Transform *pTransformCapsule =
      a->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformSphere =
      b->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformCapsule || !pTransformSphere)
      return false;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    float4 posCapsule = pTransformCapsule->GetTranslation();
    float4 posSphere = pTransformSphere->GetTranslation();
    float4x4 matCapsule = pTransformCapsule->GetRotationMatrix();
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    // transform the sphere into the basis of the capsule
    // the sphere has no rotation, so no transform of that must be done
    posSphere = RotatePointIntoBasis(matCapsule, posCapsule, posSphere);

    float4 topPoint = pCapsule->GetTopPoint();
    float4 botPoint = pCapsule->GetBottomPoint();

    // do a closest point check between center of sphere and axis of the capsule
    Line line(topPoint, botPoint);
    float4 closestPointOnLine = line.GetClosestPoint(posSphere);

    float radiusCapsule = pCapsule->GetRadius();
    float radiusSphere = pSphere->GetRadius();

    float4 axis = posSphere - closestPointOnLine;
    float lenSq = axis.LengthSq();
    float addedRadii = radiusCapsule + radiusSphere;

    float diff = lenSq - (addedRadii * addedRadii);
    if (diff < 0.f)
    {
      float len = 0.f;
      axis.Normalize(&len);
      float dist = std::abs(addedRadii - len);

      m.m_ContactNormal = matCapsule * axis;
      m.m_Penetration.push_back(dist);

      float4 closestPoint = closestPointOnLine + axis * radiusCapsule;
      closestPoint = RotatePointOutOfBasis(matCapsule, posCapsule, closestPoint);

      m.m_ContactPoints.push_back(closestPoint);

      posSphere = RotatePointOutOfBasis(matCapsule, posCapsule, posSphere);
      if (m.m_ContactNormal.Dot(posSphere - posCapsule) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    return false;
  }

  bool CollisionMgr::SphereToBox(Core::GameComponent *a, Core::GameComponent *b, Manifold &m)
  {
    bool bResult = BoxToSphere(b, a, m);
    m.m_ContactNormal = -m.m_ContactNormal;
    return bResult;
  }

  bool CollisionMgr::CapsuleToCapsule(Core::GameComponent *a, Core::GameComponent *b, Manifold &m)
  {
    Collider *colA = dynamic_cast<Collider*>(a);
    Collider *colB = dynamic_cast<Collider*>(b);

    CapsuleCollider *capsuleA = static_cast<CapsuleCollider*>(colA);
    CapsuleCollider *capsuleB = static_cast<CapsuleCollider*>(colB);

    Core::Transform *pTransformA =
      a->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformB =
      b->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformA || !pTransformB)
      return false;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    float4 posA = pTransformA->GetTranslation();
    float4 posB = pTransformB->GetTranslation();
    float4x4 matA = pTransformA->GetRotationMatrix();
    float4x4 matB = pTransformB->GetRotationMatrix();
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    float4 topB = posB + matB * capsuleB->GetTopPoint();
    float4 botB = posB + matB * capsuleB->GetBottomPoint();

    // transform the capsuleB into capsuleA's space
    posB = RotatePointIntoBasis(matA, posA, posB);

    // capsuleA is now the origin and is axis aligned
    matB = matA.Transpose() * matB;

    float4 topA = capsuleA->GetTopPoint();
    float4 botA = capsuleA->GetBottomPoint();

    topB = RotatePointIntoBasis(matA, posA, topB);
    botB = RotatePointIntoBasis(matA, posA, botB);

    // do a simple line to line intersection test
    Line lineA(topA, botA);
    Line lineB(topB, botB);

    // find the closest point between the two line segment
    Line lineClosest = lineA.GetClosestPoints(lineB);

    float4 axis = lineClosest.GetAxis();

    // take the distance from the axis, if greater than added radii, no collision
    float lenSq = axis.LengthSq();
    float radiusA = capsuleA->GetRadius();
    float radiusB = capsuleB->GetRadius();
    float addedRadii = radiusA + radiusB;

    float diff = 0.f;
    diff = lenSq - (addedRadii * addedRadii);

    if (diff < 0.f)
    {
      float4 n = axis.Normal();
      float4 p0 = lineClosest.m_P0 + n * radiusA;
      float4 p1 = lineClosest.m_P1 + (-n) * radiusB;
      float len = (p1 - p0).Length();
      p0 = RotatePointOutOfBasis(matA, posA, p0);
      p1 = RotatePointOutOfBasis(matA, posA, p1);

      m.m_ContactPoints.push_back(p0);
      m.m_ContactPoints.push_back(p1);
      m.m_Penetration.push_back(len);
      m.m_Penetration.push_back(len);

      m.m_ContactNormal = matA * n;

      if (m.m_ContactNormal.Dot(posB - posA) < 0.f)
        m.m_ContactNormal = -m.m_ContactNormal;

      // grab the rigidbody
      m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
      m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);

      return true;
    }

    return false;
  }

  bool CollisionMgr::CapsuleToBox(Core::GameComponent *b, Core::GameComponent *a, Manifold &m)
  {
    bool bResult = BoxToCapsule(b, a, m);
    m.m_ContactNormal = -m.m_ContactNormal;
    return bResult;
  }

  bool CollisionMgr::CapsuleToSphere(Core::GameComponent *a, Core::GameComponent *b, Manifold &m)
  {
    bool bResult = SphereToCapsule(b, a, m);
    m.m_ContactNormal = -m.m_ContactNormal;
    return bResult;
    //Collider *colA = dynamic_cast<Collider*>(a);
    //Collider *colB = dynamic_cast<Collider*>(b);
    //
    //CapsuleCollider *pCapsule = static_cast<CapsuleCollider*>(colA);
    //SphereCollider *pSphere = static_cast<SphereCollider*>(colB);
    //
    //Core::Transform *pTransformCapsule =
    //  a->GetOwner()->GetComponent<Core::Transform>(eTransform);
    //Core::Transform *pTransformSphere =
    //  b->GetOwner()->GetComponent<Core::Transform>(eTransform);
    //
    //if (!pTransformCapsule || !pTransformSphere)
    //  return false;
    //
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    //float4 posCapsule = pTransformCapsule->GetTranslation();
    //float4 posSphere = pTransformSphere->GetTranslation();
    //float4x4 matCapsule = pTransformCapsule->GetRotationMatrix();
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    //
    //// transform the sphere into the basis of the capsule
    //// the sphere has no rotation, so no transform of that must be done
    //posSphere = RotatePointIntoBasis(matCapsule, posCapsule, posSphere);
    //
    //float4 topPoint = pCapsule->GetTopPoint();
    //float4 botPoint = pCapsule->GetBottomPoint();
    //
    //// do a closest point check between center of sphere and axis of the capsule
    //Line line(topPoint, botPoint);
    //float4 closestPointOnLine = line.GetClosestPoint(posSphere);
    //
    //float radiusCapsule = pCapsule->GetRadius();
    //float radiusSphere = pSphere->GetRadius();
    //
    //float4 axis = posSphere - closestPointOnLine;
    //float lenSq = axis.LengthSq();
    //float addedRadii = radiusCapsule + radiusSphere;
    //
    //float diff = lenSq - (addedRadii * addedRadii);
    //if (diff < 0.f)
    //{
    //  float len = 0.f;
    //  axis.Normalize(&len);
    //  float dist = std::abs(addedRadii - len);
    //
    //  m.m_ContactNormal = matCapsule * axis;
    //  m.m_Penetration.push_back(dist);
    //
    //  float4 closestPoint = closestPointOnLine + axis * radiusCapsule;
    //  closestPoint = RotatePointOutOfBasis(matCapsule, posCapsule, closestPoint);
    //
    //  m.m_ContactPoints.push_back(closestPoint);
    //
    //  posSphere = RotatePointOutOfBasis(matCapsule, posCapsule, posSphere);
    //  if (m.m_ContactNormal.Dot(posSphere - posCapsule) < 0.f)
    //    m.m_ContactNormal = -m.m_ContactNormal;
    //
    //  // grab the rigidbody
    //  m.m_pBodyA = a->GetOwner()->GetComponent<RigidBody>(eRigidBody);
    //  m.m_pBodyB = b->GetOwner()->GetComponent<RigidBody>(eRigidBody);
    //
    //  return true;
    //}
    //
    //return false;
  }

}