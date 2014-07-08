////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATERIAL_HPP
#define SWIFT2D_MATERIAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/utils/SavableObject.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Material;
typedef std::shared_ptr<Material>       MaterialPtr;
typedef std::shared_ptr<const Material> ConstMaterialPtr;
typedef Property<MaterialPtr>           MaterialProperty;

// -----------------------------------------------------------------------------
class Material : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // uses the Material on the given context.
  virtual void draw_quad(RenderContext const& context,
                         math::mat3 const& object_transform,
                         float object_depth) = 0;

  template<typename T>
  void set_uniform(std::string name, T const& val) {

  }
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_MATERIAL_HPP
