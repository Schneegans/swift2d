////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GUI_COMPONENT_HPP
#define SWIFT2D_GUI_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/gui/GuiElement.hpp>
#include <swift2d/gui/Interface.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class GuiComponent;
typedef std::shared_ptr<GuiComponent>       GuiComponentPtr;
typedef std::shared_ptr<const GuiComponent> ConstGuiComponentPtr;

// -----------------------------------------------------------------------------
class GuiComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float  Depth;
  String Resource;
  Vec2i  Size;
  Vec2   Anchor;
  Vec2   Offset;
  Bool   Active;

  // ------------------------------------------------------------------- signals
  Signal<std::string> on_javascript_callback;
  Signal<>            on_loaded;

  // ---------------------------------------------------- construction interface
  GuiComponent();

  static GuiComponentPtr create() {
    return std::make_shared<GuiComponent>();
  }

  // creates a copy from this
  GuiComponentPtr create_copy() const {
    return std::make_shared<GuiComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "GuiComponent"; }

  void reload();

  void call_javascript(std::string const& method, std::string const& arg);
  void call_javascript(std::string const& method, std::vector<std::string> const& args);
  void add_javascript_callback(std::string const& callback);

  void draw(RenderContext const& ctx);

  void serialize(SerializedScenePtr& scene) const;

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  GuiElementPtr gui_element_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_GUI_COMPONENT_HPP
