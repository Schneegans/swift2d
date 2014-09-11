////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATERIAL_BASE_HPP
#define SWIFT2D_MATERIAL_BASE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/SavableObject.hpp>

namespace swift {

class RenderContext;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class MaterialBase;
typedef std::shared_ptr<MaterialBase>       MaterialBasePtr;
typedef std::shared_ptr<const MaterialBase> ConstMaterialBasePtr;
typedef Property<MaterialBasePtr>           MaterialBaseProperty;

// -----------------------------------------------------------------------------
class MaterialBase : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // uses the Material on the given context.
  virtual void draw_quad(RenderContext const& ctx, math::mat3 const& object_transform,
                         float object_depth, float time = 0.f) = 0;

  virtual void draw_fullscreen_quad(RenderContext const& ctx, float time = 0.f) = 0;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_MATERIAL_BASE_HPP
