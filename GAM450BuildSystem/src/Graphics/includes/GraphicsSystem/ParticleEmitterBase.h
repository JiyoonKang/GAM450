#pragma  once

#include<vector>
#include <Base/util/math/VectorMath.h>

//forward declarations
struct Particle;

class ParticleEmitterBase
{
public:
  ParticleEmitterBase();
  ~ParticleEmitterBase();

  //properties
  float3 m_diffFromOwner;

  //Methods
  virtual void UpdateParticles(std::vector<Particle>& particles) = 0;

private:
  virtual void ResetParticle(Particle& p) = 0;

};