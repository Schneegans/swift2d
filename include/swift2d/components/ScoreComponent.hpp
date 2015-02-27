////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SCORE_COMPONENT_HPP
#define SWIFT2D_SCORE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ScoreComponent;
typedef std::shared_ptr<ScoreComponent>       ScoreComponentPtr;
typedef std::shared_ptr<const ScoreComponent> ConstScoreComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL ScoreComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Int32 Score;

  // ----------------------------------------------------- contruction interface
  ScoreComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static ScoreComponentPtr create(Args&& ... a) {
    return std::make_shared<ScoreComponent>(a...);
  }

  // creates a copy from this
  ScoreComponentPtr create_copy() const {
    return std::make_shared<ScoreComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ScoreComponent"; }

  void increase(int amount=1);
  void decrease(int amount=1);

  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif  // SWIFT2D_SCORE_COMPONENT_HPP
