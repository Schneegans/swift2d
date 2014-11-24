////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PHYSICS_HPP
#define SWIFT2D_PHYSICS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/physics/DynamicBodyComponent.hpp>
#include <swift2d/physics/StaticBodyComponent.hpp>
#include <swift2d/physics/GravitySourceComponent.hpp>
#include <swift2d/physics/GravityMap.hpp>
#include <swift2d/utils/Singleton.hpp>

#include <unordered_set>

// forward declares ------------------------------------------------------------
class b2World;
class b2Body;

namespace swift {

class SwiftContactListener;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Physics : public Singleton<Physics> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void update(double time);

  b2Body* add(DynamicBodyComponent* body);
  b2Body* add(StaticBodyComponent* body);
  void    add(GravitySourceComponent* source);

  void    add_shock_wave(math::vec2 const& location, float strength);

  void    remove(b2Body* body);
  void    remove(GravitySourceComponent* source);

  void create_gravity_map(RenderContext const& ctx);
  void clear_gravity_map (RenderContext const& ctx);
  void update_gravity_map(ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void bind_gravity_map  (RenderContext const& ctx, int location);

  friend class Singleton<Physics>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Physics();
  ~Physics();

  std::unordered_set<GravitySourceComponent const*> gravity_sources_;
  b2World* world_;
  SwiftContactListener* contact_listener_;;

  GravityMap* gravity_map_;
  std::vector<math::vec3> shock_waves_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_PHYSICS_HPP
