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
class Physics : public Singleton<Physics> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void update(double time);


  b2Body* add(DynamicBodyComponent* body);
  b2Body* add(StaticBodyComponent* body);
  void    add(GravitySourceComponent* source);

  void    remove(b2Body* body);
  void    remove(GravitySourceComponent* source);

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

};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_PHYSICS_HPP
