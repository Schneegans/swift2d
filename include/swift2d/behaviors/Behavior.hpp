////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_BEHAVIOR_HPP
#define SWIFT2D_BEHAVIOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
// class Behavior;
// typedef std::shared_ptr<Behavior>       BehaviorPtr;
// typedef std::shared_ptr<const Behavior> ConstBehaviorPtr;

// -----------------------------------------------------------------------------
template<typename UserType>
class Behavior : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties

  // ------------------------------------------------------------ public methods
  virtual void update(double time) = 0;

  void set_user(UserType u) {
    Component::set_user(u);
    user_ = u;
  }

  UserType get_user() const {
    return user_;
  }

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:
  Behavior() {}

  UserType user_;
};

}

#endif  // SWIFT2D_BEHAVIOR_HPP
