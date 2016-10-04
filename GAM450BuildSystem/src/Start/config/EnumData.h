/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EnumData.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/*****************************************************************
This is the enumeration manifest file where special enumerations
are processed by the engine. Essentially this is what happens to
every single enumeration declared here.

(Where *** is what is inside E_NAME() )
(Where $$$ is what is inside E_ENTRY() )

- Creates a C++ enumeration E*** 
  - Each Entry inside of the enumeration is e$$$
- Creates an instance of the Enum class named ENUM***

The Enum class is cool because it contains a hash map of $$$
to the corresponding C++ enumeration index
*****************************************************************/

E_NAME(Generics)
E_ENTRY(GameComponent)
E_ENTRY(GameObject)
E_ENTRY(Abstract)
E_END(Generics)

E_NAME(Component)
// Physics
E_ENTRY(RigidBody)
E_ENTRY(BoxCollider)
E_ENTRY(SphereCollider)
E_ENTRY(CapsuleCollider)
E_ENTRY(GravityEffect)
E_ENTRY(ConstantEffect)
E_ENTRY(DragEffect)

E_ENTRY(Transform)

//Graphics
E_ENTRY(Renderable)
E_ENTRY(Paintable)
E_ENTRY(PointLight)
E_ENTRY(EmissiveComponent)
E_ENTRY(Fade)
E_ENTRY(Levitate)
E_ENTRY(Flicker)
E_ENTRY(LightDeath)
E_ENTRY(ParticleSystem)

// Audio
E_ENTRY(SoundEmitter)

// Gameplay
//E_ENTRY(GamePad)
  E_ENTRY(PlayerController)
  E_ENTRY(Timer)
  E_ENTRY(Stats)
  E_ENTRY(Bullet)
  E_ENTRY(Flag)

E_ENTRY(LuaProxy)
E_ENTRY(LuaComponent)
E_ENTRY(countComponents) //keep this at the end
E_END(Component)
