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
#include <swift2d/physics/RigidbodyComponent.hpp>
#include <swift2d/utils/Singleton.hpp>

#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Physics : public Singleton<Physics> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void update(double time);


  void add_rigidbody(RigidbodyComponentPtr const& body);
  void remove_rigidbody(RigidbodyComponentPtr const& body);

  friend class Singleton<Physics>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Physics() {};
  ~Physics() {};

  std::unordered_set<RigidbodyComponent*> rigid_bodies_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_PHYSICS_HPP
