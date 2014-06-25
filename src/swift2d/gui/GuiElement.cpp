
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
  , parent_(parent) {

  view_ = Interface::instance()->create_webview(parent_->Size().x(), parent_->Size().y());
  view_->SetTransparent(true);
  view_->Focus();
  view_->set_view_listener(new AweViewListener());
  view_->set_load_listener(new AweLoadListener(parent_));
  view_->set_process_listener(new AweProcessListener());
  view_->set_js_method_handler(new AweJSMethodHandler(parent_));

  auto window = WindowManager::instance()->get_default();

  callbacks_[0] = window->on_mouse_move.connect([&](math::vec2 const& pos) {
    auto size(WindowManager::instance()->get_default()->get_context().size);

    math::vec2 corner(
      (size.x() - parent_->Size().x() + parent_->Anchor().x() * (size.x() - parent_->Size().x()))*0.5 + parent_->Offset().x(),
      (size.y() - parent_->Size().y() + parent_->Anchor().y() * (size.y() - parent_->Size().y()))*0.5 + parent_->Offset().y()
    );

    view_->InjectMouseMove(pos.x() - corner.x(), pos.y() - corner.y());
  });

  callbacks_[1] = window->on_scroll.connect([&](math::vec2 const& dir) {
    view_->InjectMouseWheel(dir.y()*10, dir.x()*10);
  });

  callbacks_[2] = window->on_button_press.connect([&](Button button, int action, int mods) {
    if (action == 0) {
      view_->InjectMouseUp(static_cast<Awesomium::MouseButton>(button));
    } else {
      view_->InjectMouseDown(static_cast<Awesomium::MouseButton>(button));
    }
  });

  callbacks_[3] = window->on_char.connect([&](unsigned c) {
    view_->InjectKeyboardEvent(AweKeyEvent(c));
  });

  callbacks_[4] = window->on_key_press.connect([&](Key key, int scancode, int action, int mods) {
    view_->InjectKeyboardEvent(AweKeyEvent(key, scancode, action, mods));
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

  auto window = WindowManager::instance()->get_default();
  window->on_mouse_move.disconnect(callbacks_[0]);
  window->on_scroll.disconnect(callbacks_[1]);
  window->on_button_press.disconnect(callbacks_[2]);
  window->on_char.disconnect(callbacks_[3]);
  window->on_key_press.disconnect(callbacks_[4]);
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::reload() {
  view_->Reload(true);
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::call_javascript(std::string const& method, std::string const& arg) {
  Awesomium::JSValue window = view_->ExecuteJavascriptWithResult(
    Awesomium::WSLit("window"), Awesomium::WSLit("")
  );

  if (window.IsObject()) {
    Awesomium::JSArray args;
    args.Push(Awesomium::JSValue(Awesomium::ToWebString(arg)));

    window.ToObject().Invoke(Awesomium::ToWebString(method), args);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::add_javascript_callback(std::string const& callback) {
  Awesomium::JSValue o = view_->ExecuteJavascriptWithResult(
    Awesomium::WSLit("Swift2D"), Awesomium::WSLit("")
  );

  if (o.IsObject()) {
    o.ToObject().SetCustomMethod(Awesomium::ToWebString(callback), false);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiElement::draw(RenderContext const& ctx) {

  if (Interface::instance()->bind(view_, ctx, 0)) {
    GuiShader::instance()->use(ctx);

    math::vec2 size(
      1.0 * parent_->Size().x() / ctx.size.x(),
      1.0 * parent_->Size().y() / ctx.size.y()
    );

    math::vec2 offset(
      (2.0 * parent_->Offset().x() + parent_->Anchor().x() * (ctx.size.x() - parent_->Size().x()))/ctx.size.x(),
      (2.0 * parent_->Offset().y() + parent_->Anchor().y() * (ctx.size.y() - parent_->Size().y()))/ctx.size.y()
    );

    GuiShader::instance()->set_uniform("size", size);
    GuiShader::instance()->set_uniform("offset", offset);
    GuiShader::instance()->set_uniform("diffuse", 0);
    Quad::instance()->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

}

