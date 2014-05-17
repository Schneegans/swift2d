////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SHAPE_HPP
#define SWIFT2D_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Shape;
typedef std::shared_ptr<Shape>       ShapePtr;
typedef std::shared_ptr<const Shape> ConstShapePtr;

// -----------------------------------------------------------------------------
class Shape : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  virtual bool contains(ShapePtr const& other) = 0;
  virtual bool intersects(ShapePtr const& other) = 0;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SHAPE_HPP
