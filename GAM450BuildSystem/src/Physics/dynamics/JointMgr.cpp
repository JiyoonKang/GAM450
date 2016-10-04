/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: JointMgr.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "physics/dynamics/JointMgr.h"
#include "physics/dynamics/Joint.h"

namespace Physics
{
  JointMgr::JointMgr(void)
    : m_Joints()
  {

  }

  void JointMgr::ClearJoints(void)
  {
    for (auto it = m_Joints.begin(); it != m_Joints.end(); ++it)
      delete *it;

    m_Joints.clear();
  }

  unsigned JointMgr::AddJoint(Joint *pJoint)
  {
    if (!pJoint)
      return static_cast<unsigned>(-1);

    unsigned nResult = static_cast<unsigned>(m_Joints.size());
    m_Joints.push_back(pJoint);
    return nResult;
  }

  Joint const *JointMgr::GetJoint(unsigned nIdx) const
  {
    if (nIdx >= static_cast<unsigned>(m_Joints.size()))
      return nullptr;

    return m_Joints[nIdx];
  }

  Joint *JointMgr::GetJoint(unsigned nIdx)
  {
    if (nIdx >= static_cast<unsigned>(m_Joints.size()))
      return nullptr;

    return m_Joints[nIdx];
  }

  bool JointMgr::RemoveJoint(unsigned nIdx)
  {
    if (nIdx >= static_cast<unsigned>(m_Joints.size()))
      return false;

    std::swap(m_Joints[nIdx], m_Joints.back());
    m_Joints.pop_back();
    return true;
  }

  void JointMgr::Step(float dt)
  {
    for (auto it = m_Joints.begin(); it != m_Joints.end(); ++it)
      (*it)->Step(dt);
  }

  void JointMgr::ApplyImpulse(unsigned nIterations)
  {
    for (unsigned nIdx = 0; nIdx < nIterations; ++nIdx)
      for (auto it = m_Joints.begin(); it != m_Joints.end(); ++it)
        (*it)->ApplyImpulse();
  }

  void JointMgr::Draw(void)
  {
    for (auto it = m_Joints.begin(); it != m_Joints.end(); ++it)
      (*it)->Draw();
  }
}