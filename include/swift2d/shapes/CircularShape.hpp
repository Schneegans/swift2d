////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_CIRCULAR_SHAPE_HPP
#define SWIFT2D_CIRCULAR_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/shapes/Shape.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A screen core represents a rectangular, virtual screen. Combined with a    //
// view core it defines the viewing frustum.                                  //
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class RectangularShape;
typedef std::shared_ptr<RectangularShape>       RectangularShapePtr;

// shared pointer type definition ----------------------------------------------
class CircularShape;
typedef std::shared_ptr<CircularShape>       CircularShapePtr;
typedef std::shared_ptr<const CircularShape> ConstCircularShapePtr;

// -----------------------------------------------------------------------------
class CircularShape : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {
    return "CircularShape";
  }

  /*virtual*/ void update(double time) {
    TransformableComponent::update(time);
    center_ = (WorldTransform() * math::vec3(0, 0, 1)).xy();
    auto diameter((WorldTransform() * math::vec3(1, 0, 0)).xy());
    square_radius_ = diameter.x()*diameter.x() + diameter.y()*diameter.y();
  }

  /*virtual*/ bool contains(CircularShapePtr const& other) {
    auto dist(center_ - other->center_);
    return dist.x()*dist.x() + dist.y()*dist.y() < other->square_radius_ - square_radius_;
  };

  /*virtual*/ bool intersects(CircularShapePtr const& other) {
    auto dist(center_ - other->center_);
    return dist.x()*dist.x() + dist.y()*dist.y() < square_radius_ + other->square_radius_;
  };

  /*virtual*/ bool contains(RectangularShapePtr const& other) {

  };

  /*virtual*/ bool intersects(RectangularShapePtr const& other) {

  };

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  math::vec2  center_;
  float       square_radius_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_CIRCULAR_SHAPE_HPP
