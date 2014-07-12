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
#include <swift2d/utils/Singleton.hpp>

#include <unordered_set>

// forward declares ------------------------------------------------------------
class b2World;
class b2Body;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Physics : public Singleton<Physics> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void update(double time);


  b2Body* add(DynamicBodyComponent const* body);
  b2Body* add(StaticBodyComponent const* body);
  // void remove_rigidbody(RigidbodyComponentPtr const& body);

  friend class Singleton<Physics>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Physics();
  ~Physics();

  // std::unordered_set<RigidbodyComponent*> rigid_bodies_;
  b2World* world_;


};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_PHYSICS_HPP
