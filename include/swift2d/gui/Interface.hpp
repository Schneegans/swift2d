////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_INTERFACE_HPP
#define SWIFT2D_INTERFACE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/graphics/Window.hpp>

// forward declares ------------------------------------------------------------
namespace Awesomium {
  class WebCore;
  class WebView;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Interface : public Singleton<Interface> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void update() const;
  bool bind(RenderContext const& ctx, unsigned location) const;

  void set_window(WindowPtr& window) const;

  friend class Singleton<Interface>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Interface();
  ~Interface();

  Awesomium::WebCore* web_core_;
  Awesomium::WebView* view_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_INTERFACE_HPP
