////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RECTANGULAR_SHAPE_HPP
#define SWIFT2D_RECTANGULAR_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/shapes/Shape.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A screen core represents a rectangular, virtual screen. Combined with a    //
// view core it defines the viewing frustum.                                  //
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class CircularShape;
typedef std::shared_ptr<CircularShape>       CircularShapePtr;

// shared pointer type definition ----------------------------------------------
class RectangularShape;
typedef std::shared_ptr<RectangularShape>       RectangularShapePtr;
typedef std::shared_ptr<const RectangularShape> ConstRectangularShapePtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL RectangularShape : public Shape {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "RectangularShape"; }

  /*virtual*/ bool contains(CircularShapePtr const& other) {
    return false;
  };

  /*virtual*/ bool contains(RectangularShapePtr const& other) {
    return false;
  };
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_RECTANGULAR_SHAPE_HPP
