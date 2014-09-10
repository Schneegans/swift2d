////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GUI_ELEMENT_HPP
#define SWIFT2D_GUI_ELEMENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/gui/Interface.hpp>

// forward declares ------------------------------------------------------------
namespace Awesomium {
  class WebView;
  class JSValue;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class GuiComponent;

// shared pointer type definition ----------------------------------------------
class GuiElement;
typedef std::shared_ptr<GuiElement>       GuiElementPtr;
typedef std::shared_ptr<const GuiElement> ConstGuiElementPtr;
typedef Property<GuiElementPtr>           GuiElementProperty;

// -----------------------------------------------------------------------------
class GuiElement {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  GuiElement(GuiComponent* parent);
  ~GuiElement();

  static GuiElementPtr create(GuiComponent* parent) {
    return std::make_shared<GuiElement>(parent);
  }

  // ------------------------------------------------------------ public methods
  void reload();

  void focus();
  void set_interactive(bool interactive);

  void update_mouse_position(math::vec2 const& pos) const;

  void call_javascript(std::string const& method, std::vector<std::string> const& args) const;
  void add_javascript_callback(std::string const& callback, bool with_result);

  void draw(RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Awesomium::WebView* view_;
  Awesomium::JSValue* js_window_;
  std::vector<int>    callbacks_;
  GuiComponent*       parent_;
  bool                interactive_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_GUI_ELEMENT_HPP
