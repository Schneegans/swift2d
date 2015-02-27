////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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
  class WebSession;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Interface : public Singleton<Interface> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------- signals
  Signal<Cursor> on_cursor_change;

  // ---------------------------------------------------------------- properties
  Float LoadingProgress;

  // ------------------------------------------------------------ public methods
  void update() const;

  friend class GuiComponent;
  friend class Singleton<Interface>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Interface();
  ~Interface();

  bool bind(Awesomium::WebView* view, RenderContext const& ctx, unsigned location) const;
  Awesomium::WebView* create_webview(int width, int height) const;

  void increase_loading_state();
  void decrease_loading_state();

  int loading_state_;
  int max_state_;

  Awesomium::WebCore* web_core_;
  Awesomium::WebSession* web_session_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_INTERFACE_HPP
