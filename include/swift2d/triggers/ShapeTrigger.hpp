////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SHAPER_TRIGGER_HPP
#define SWIFT2D_SHAPER_TRIGGER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/events.hpp>
#include <swift2d/shapes.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL ShapeTrigger {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------- signals
  Signal<> on_leave;
  Signal<> on_enter;

  // ----------------------------------------------------- contruction interface
  ~ShapeTrigger();

  // ------------------------------------------------------------ public methods
  void set_shapes(CircularShapePtr const& a, CircularShapePtr const& b);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void check();
  bool callback(math::mat3 const& v);

  CircularShapePtr a_, b_;
  int a_callback_, b_callback_;
  bool intersects_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SHAPER_TRIGGER_HPP
