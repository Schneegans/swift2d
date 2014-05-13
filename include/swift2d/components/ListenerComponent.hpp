////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LISTENER_COMPONENT_HPP
#define SWIFT2D_LISTENER_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/openal.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A screen core represents a rectangular, virtual screen. Combined with a    //
// view core it defines the viewing frustum.                                  //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ListenerComponent;
typedef std::shared_ptr<ListenerComponent>       ListenerComponentPtr;
typedef std::shared_ptr<const ListenerComponent> ConstListenerComponentPtr;

// -----------------------------------------------------------------------------
class ListenerComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  ListenerComponent();

  // ---------------------------------------------------------------- properties
  Float Volume;

  // ------------------------------------------------------------ public methods
  virtual void update(double time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  oalplus::Listener listener_;


};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_LISTENER_COMPONENT_HPP
