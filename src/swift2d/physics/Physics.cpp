////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/Physics.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void Physics::update(double time) {

}

////////////////////////////////////////////////////////////////////////////////

void Physics::add_rigidbody(RigidbodyComponentPtr const& body) {
  rigid_bodies_.insert(&*body);
}

////////////////////////////////////////////////////////////////////////////////

void Physics::remove_rigidbody(RigidbodyComponentPtr const& body) {
  rigid_bodies_.erase(&*body);
}

////////////////////////////////////////////////////////////////////////////////

}
