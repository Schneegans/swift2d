////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DRAWABLE_COMPONENT_HPP
#define SWIFT2D_DRAWABLE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A screen core represents a rectangular, virtual screen. Combined with a    //
// view core it defines the viewing frustum.                                  //
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class RenderContext;

// shared pointer type definition ----------------------------------------------
class DrawableComponent;
typedef std::shared_ptr<DrawableComponent>       DrawableComponentPtr;
typedef std::shared_ptr<const DrawableComponent> ConstDrawableComponentPtr;

// -----------------------------------------------------------------------------
class DrawableComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  virtual void draw(RenderContext const& ctx) = 0;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_DRAWABLE_COMPONENT_HPP
