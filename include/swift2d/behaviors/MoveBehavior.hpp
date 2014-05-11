////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MOVE_BEHAVIOR_HPP
#define SWIFT2D_MOVE_BEHAVIOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/behaviors/Behavior.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class MoveBehavior;
typedef std::shared_ptr<MoveBehavior>       MoveBehaviorPtr;
typedef std::shared_ptr<const MoveBehavior> ConstMoveBehaviorPtr;

// -----------------------------------------------------------------------------
class MoveBehavior : public Behavior<SceneObject*> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Vec2 pSpeed;

  // ------------------------------------------------------------ public methods
  virtual void update(double time) {
    get_user()->pTransform.set(math::translated(get_user()->pTransform.get(), pSpeed.get() * time));
  }

};

}

#endif  // SWIFT2D_MOVE_BEHAVIOR_HPP
