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

#ifndef SWIFT2D_OFFSET_COMPONENT_HPP
#define SWIFT2D_OFFSET_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class OffsetComponent;
typedef std::shared_ptr<OffsetComponent>       OffsetComponentPtr;
typedef std::shared_ptr<const OffsetComponent> ConstOffsetComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL OffsetComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  AnimatedFloat TranslationOffsetX;
  AnimatedFloat TranslationOffsetY;
  AnimatedFloat RotationOffset;

  // ----------------------------------------------------- constrution interface
  OffsetComponent()
    : TranslationOffsetX()
    , TranslationOffsetY()
    , RotationOffset() {

    TranslationOffsetX.set(0);
    TranslationOffsetY.set(0);
    RotationOffset.set(0);
  }

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static OffsetComponentPtr create(Args&& ... a) {
    return std::make_shared<OffsetComponent>(a...);
  }

  // creates a copy from this
  OffsetComponentPtr create_copy() const {
    return std::make_shared<OffsetComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
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
  static  std::string get_type_name_static() { return "OffsetComponent"; }

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

#endif  // SWIFT2D_OFFSET_COMPONENT_HPP
