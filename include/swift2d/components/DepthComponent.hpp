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

#ifndef SWIFT2D_DEPTH_COMPONENT_HPP
#define SWIFT2D_DEPTH_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/math.hpp>

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DepthComponent;
typedef std::shared_ptr<DepthComponent>       DepthComponentPtr;
typedef std::shared_ptr<const DepthComponent> ConstDepthComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL DepthComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  DepthComponent();

  // ---------------------------------------------------------------- properties
  Float Depth;
  Float WorldDepth;

  // ------------------------------------------------------------ public methods
  virtual void update(double time, SceneObject* user);
  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif  // SWIFT2D_DEPTH_COMPONENT_HPP
