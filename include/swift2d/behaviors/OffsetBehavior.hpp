////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_OFFSET_BEHAVIOR_HPP
#define SWIFT2D_OFFSET_BEHAVIOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/behaviors/Behavior.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class OffsetBehavior;
typedef std::shared_ptr<OffsetBehavior>       OffsetBehaviorPtr;
typedef std::shared_ptr<const OffsetBehavior> ConstOffsetBehaviorPtr;

// -----------------------------------------------------------------------------
class OffsetBehavior : public Behavior<SceneObject*> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  AnimatedFloat TranslationOffsetX;
  AnimatedFloat TranslationOffsetY;
  AnimatedFloat RotationOffset;

  // ----------------------------------------------------- constrution interface
  OffsetBehavior()
    : TranslationOffsetX()
    , TranslationOffsetY()
    , RotationOffset() {

    TranslationOffsetX.set(0);
    TranslationOffsetY.set(0);
    RotationOffset.set(0);
  }

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static OffsetBehaviorPtr create(Args&& ... a) {
    return std::make_shared<OffsetBehavior>(a...);
  }

  // creates a copy from this
  OffsetBehaviorPtr create_copy() const {
    return std::make_shared<OffsetBehavior>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual void update(double time) {

    TranslationOffsetX.update(time);
    TranslationOffsetX.update(time);
    RotationOffset.update(time);

    auto user_transform(get_user()->Transform.get());

    math::set_translate(user_transform, math::get_translate(user_transform) + math::vec2(TranslationOffsetX.get(), TranslationOffsetY.get()));
    math::set_rotation(user_transform, math::get_rotation(user_transform) + RotationOffset.get());

    get_user()->Transform.set(user_transform);
  }

};

}

#endif  // SWIFT2D_OFFSET_BEHAVIOR_HPP
