////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PHYSICS_HPP
#define SWIFT2D_PHYSICS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/physics/PhysicsBodyComponent.hpp>
#include <swift2d/physics/GravitySourceComponent.hpp>
#include <swift2d/physics/GravityMap.hpp>
#include <swift2d/utils/Singleton.hpp>

#include <unordered_set>

// forward declares ------------------------------------------------------------
class b2World;
class b2Body;
class b2Joint;

namespace swift {

class SwiftContactListener;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Physics : public Singleton<Physics> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  struct ShockWave {
    math::uint64 damage_source;
    math::vec2   location;
    float        damage;
    float        radius;
    float        strength;
  };

  Vec2 Gravity;

  void update(double time);

  b2Body* add(PhysicsBodyComponent* body);
  void    add(GravitySourceComponent* source);

  b2Joint* add_constraint(b2Body* a, b2Body* b, math::vec2 const& world_pos);
  void    remove_constraint(b2Joint* constraint);

  void    add_shock_wave(math::uint64 damage_source, math::vec2 const& location, float damage, float radius, float strength);

  void    remove(b2Body* body);
  void    remove(GravitySourceComponent* source);

  void create_gravity_map(RenderContext const& ctx);
  void clear_gravity_map (RenderContext const& ctx);
  void update_gravity_map(ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void bind_gravity_map  (RenderContext const& ctx, int location);

  bool ray_cast(math::vec2 const& start, math::vec2 const& end,
                std::vector<PhysicsBodyComponent*>& hits,
                std::vector<math::vec2>& hit_points,
                std::vector<math::vec2>& hit_normals) const;

  friend class Singleton<Physics>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Physics();
  ~Physics();

  std::unordered_set<GravitySourceComponent const*> gravity_sources_;
  b2World* world_;
  SwiftContactListener* contact_listener_;

  GravityMap* gravity_map_;
  std::vector<ShockWave> shock_waves_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_PHYSICS_HPP
