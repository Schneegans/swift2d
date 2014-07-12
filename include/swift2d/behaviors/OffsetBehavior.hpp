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
#include <swift2d/components/Component.hpp>
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
class OffsetBehavior : public Component {

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

  void set_transform_offset(math::vec2 const& offset) {
    TranslationOffsetX.set(offset.x());
    TranslationOffsetY.set(offset.y());
    TranslationOffsetX.set(0, 4.0);
    TranslationOffsetY.set(0, 4.0);
  }

  void set_rotation_offset(float offset) {
    RotationOffset.set(offset);
    RotationOffset.set(0, 4.0);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "OffsetBehavior"; }

  virtual void update(double time) {

    float x = TranslationOffsetX.get();
    float y = TranslationOffsetY.get();
    float r = RotationOffset.get();

    TranslationOffsetX.update(time);
    TranslationOffsetX.update(time);
    RotationOffset.update(time);

    x -= TranslationOffsetX.get();
    y -= TranslationOffsetY.get();
    r -= RotationOffset.get();

    auto mat = math::make_translation(x, y);
    math::rotate(mat, r);

    auto user_transform(get_user()->Transform.get());
    get_user()->Transform.set(mat * user_transform);
  }

};

}

#endif  // SWIFT2D_OFFSET_BEHAVIOR_HPP
