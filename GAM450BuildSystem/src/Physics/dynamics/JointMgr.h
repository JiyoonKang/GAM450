/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: JointMgr.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_DYNAMICS_JOINT_MGR
#define H_PAINT_PHYSICS_DYNAMICS_JOINT_MGR

#include <vector>

namespace Physics
{
  class Joint;

  class JointMgr
  {
  private:
    // we own these joints, delete them on exit or removal
    // find a better way to store these? removal causes all of indices
    // that are given back from add joint invalid
    std::vector<Joint*> m_Joints;

  public:
    JointMgr(void);

    void ClearJoints(void);
    // returned the index of the added joint so they can grab the joint later on
    unsigned AddJoint(Joint *pJoint);

    Joint const *GetJoint(unsigned nIdx) const;
    Joint *GetJoint(unsigned nIdx);

    // returns true if removed successfully, false otherwise
    bool RemoveJoint(unsigned nIdx);

    void Step(float dt);
    // runs through all joints and applies the proper impulse to them
    void ApplyImpulse(unsigned nIterations);
    void Draw(void);
  };
}

#endif