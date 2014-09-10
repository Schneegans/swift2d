
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/GuiElement.hpp>

#include <swift2d/geometries/Quad.hpp>
#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/gui/GuiShader.hpp>
#include <swift2d/gui/GuiComponent.hpp>

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

namespace swift {

namespace {

#include "AweKeyEvent.ipp"
#include "AweViewListener.ipp"
#include "AweLoadListener.ipp"
#include "AweProcessListener.ipp"
#include "AweJSMethodHandler.ipp"

}

////////////////////////////////////////////////////////////////////////////////

GuiElement::GuiElement(GuiComponent* parent)
  : view_(nullptr)
  , callbacks_(5)
  , parent_(parent)
  , interactive_(true) {

  view_ = Interface::get().create_webview(parent_->Size().x(), parent_->Size().y());
  view_->SetTransparent(true);
  view_->Focus();
  view_->set_view_listener(new AweViewListener());
  view_->set_load_listener(new AweLoadListener(parent_));
  view_->set_process_listener(new AweProcessListener());
  view_->set_js_method_handler(new AweJSMethodHandler(parent_));

  js_window_ = new Awesomium::JSValue();
  *js_window_ = view_->ExecuteJavascriptWithResult(
    Awesomium::WSLit("window"), Awesomium::WSLit("")
  );

  auto window = WindowManager::get().current();

  callbacks_[0] = window->on_mouse_move.connect([&](math::vec2 const& pos) {
    update_mouse_position(pos);
  });

  callbacks_[1] = window->on_mouse_scroll.connect([&](math::vec2 const& dir) {
    if (interactive_) {
      view_->InjectMouseWheel(dir.y()*10, dir.x()*10);
    }
  });

  callbacks_[2] = window->on_mouse_button_press.connect([&](Button button, int action, int mods) {
    if (interactive_) {
      if (action == 0) {
        view_->InjectMouseUp(static_cast<Awesomium::MouseButton>(button));
      } else {
        view_->InjectMouseDown(static_cast<Awesomium::MouseButton>(button));
      }
    }
  });

  callbacks_[3] = window->on_char.connect([&](unsigned c) {
    if (interactive_) {
      view_->InjectKeyboardEvent(AweKeyEvent(c));
    }
  });

  callbacks_[4] = window->on_key_press.connect([&](Key key, int scancode, int action, int mods) {
    if (interactive_) {
      view_->InjectKeyboardEvent(AweKeyEvent(key, scancode, action, mods));
    }
  });

  parent_->Resource.on_change().connect([&](std::string const& val) {
    Awesomium::WebURL url(Awesomium::WSLit(("asset://swift2d/" + val).c_str()));
    view_->LoadURL(url);
  });

  parent_->Size.on_change().connect([&](math::vec2i const& val) {
    view_->Resize(val.x(), val.y());
  });
}

////////////////////////////////////////////////////////////////////////////////

GuiElement::~GuiElement() {
  delete static_cast<AweViewListener*>(view_->view_listener());
  delete static_cast<AweLoadListener*>(view_->load_listener());
  delete static_cast<AweProcessListener*>(view_->process_listener());
  delete static_cast<AweJSMethodHandler*>(view_->js_method_handler());
  view_->Destroy();

  auto window = WindowManager::get().current();
  window->on_mouse_move.disconnect(callbacks_[0]);
  window->on_mouse_scroll.disconnect(callbacks_[1]);
  window->on_mouse_button_press.disconnect(callbacks_[2]);
  window->on_char.disconnect(callbacks_[3]);
  window->on_key_press.disconnect(callbacks_[4]);

  delete js_window_;
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::reload() {
  view_->Reload(true);
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::focus() {
  view_->Focus();
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::set_interactive(bool interactive) {
  interactive_ = interactive;

  if (interactive_) {
    auto pos = WindowManager::get().current()->get_cursor_pos();
    update_mouse_position(pos);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::update_mouse_position(math::vec2 const& pos) const {
  if (interactive_) {
    auto size(WindowManager::get().current()->get_context().window_size);

    math::vec2 corner(
      (size.x() - parent_->Size().x() + parent_->Anchor().x() * (size.x() - parent_->Size().x()))*0.5 + parent_->Offset().x(),
      (size.y() - parent_->Size().y() - parent_->Anchor().y() * (size.y() - parent_->Size().y()))*0.5 - parent_->Offset().y()
    );

    view_->InjectMouseMove(pos.x() - corner.x(), size.y() - pos.y() - corner.y());
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::call_javascript(std::string const& method, std::vector<std::string> const& args) const {
  Awesomium::JSArray j_args;
  for (auto const& arg: args) {
    j_args.Push(Awesomium::JSValue(Awesomium::ToWebString(arg)));
  }

  js_window_->ToObject().Invoke(Awesomium::ToWebString(method), j_args);
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::add_javascript_callback(std::string const& callback, bool with_result) {
  Awesomium::JSValue o = view_->ExecuteJavascriptWithResult(
    Awesomium::WSLit("Swift2D"), Awesomium::WSLit("")
  );

  if (o.IsObject()) {
    o.ToObject().SetCustomMethod(Awesomium::ToWebString(callback), with_result);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::draw(RenderContext const& ctx) {

  if (Interface::get().bind(view_, ctx, 0)) {
    GuiShader::get().use(ctx);

    math::vec2 size(
      1.0 * parent_->Size().x() / ctx.window_size.x(),
      1.0 * parent_->Size().y() / ctx.window_size.y()
    );

    math::vec2 offset(
      (2.0 * parent_->Offset().x() + parent_->Anchor().x() * (ctx.window_size.x() - parent_->Size().x()))/ctx.window_size.x(),
      (2.0 * parent_->Offset().y() + parent_->Anchor().y() * (ctx.window_size.y() - parent_->Size().y()))/ctx.window_size.y()
    );

    GuiShader::get().size.Set(size);
    GuiShader::get().offset.Set(offset);
    GuiShader::get().diffuse.Set(0);
    Quad::get().draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

}

