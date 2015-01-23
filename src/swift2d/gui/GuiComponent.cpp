////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/GuiComponent.hpp>

#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/gui/GuiShader.hpp>
#include <swift2d/graphics/RendererPool.hpp>

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

GuiComponent::GuiComponent()
  : Opacity(1.f)
  , Resource("")
  , Size(math::vec2i(10,10))
  , Anchor(math::vec2i(0,0))
  , Offset(math::vec2i(0,0))
  , Interactive(true)
  , view_(nullptr)
  , js_window_(nullptr)
  , callbacks_(5)
  , interactive_(true) {

  Interface::get().increase_loading_state();

  on_loaded.connect([this]() {
    js_window_ = new Awesomium::JSValue();
    *js_window_ = view_->ExecuteJavascriptWithResult(
      Awesomium::WSLit("window"), Awesomium::WSLit("")
    );

    if (!js_window_->IsObject()) {
      LOG_WARNING << "Failed to initialize GuiComponent!" << std::endl;
    }

    Interface::get().decrease_loading_state();

    return false;
  });

  view_ = Interface::get().create_webview(Size().x(), Size().y());
  view_->SetTransparent(true);
  view_->Focus();
  view_->set_view_listener(new AweViewListener());
  view_->set_load_listener(new AweLoadListener(this));
  view_->set_process_listener(new AweProcessListener());
  view_->set_js_method_handler(new AweJSMethodHandler(this));

  auto window = WindowManager::get().current();

  callbacks_[0] = window->on_mouse_move.connect([&](math::vec2 const& pos) {
    update_mouse_position(pos);
    return true;
  });

  callbacks_[1] = window->on_mouse_scroll.connect([&](math::vec2 const& dir) {
    if (interactive_) {
      view_->InjectMouseWheel(dir.y()*10, dir.x()*10);
    }
    return true;
  });

  callbacks_[2] = window->on_mouse_button_press.connect([&](Button button, int action, int mods) {
    if (interactive_) {
      if (action == 0) {
        view_->InjectMouseUp(static_cast<Awesomium::MouseButton>(button));
      } else {
        view_->InjectMouseDown(static_cast<Awesomium::MouseButton>(button));
      }
    }
    return true;
  });

  callbacks_[3] = window->on_char.connect([&](unsigned c) {
    if (interactive_) {
      view_->InjectKeyboardEvent(AweKeyEvent(c));
    }
    return true;
  });

  callbacks_[4] = window->on_key_press.connect([&](Key key, int scancode, int action, int mods) {
    if (interactive_) {
      view_->InjectKeyboardEvent(AweKeyEvent(key, scancode, action, mods));
    }
    return true;
  });

  Resource.on_change().connect([&](std::string const& val) {
    Awesomium::WebURL url(Awesomium::WSLit(("asset://swift2d/" + val).c_str()));
    view_->LoadURL(url);
    return true;
  });

  Size.on_change().connect([&](math::vec2i const& val) {
    view_->Resize(val.x(), val.y());
    return true;
  });

  Interactive.on_change().connect([this](bool val){
    interactive_ = val;

    if (interactive_) {
      auto pos = WindowManager::get().current()->get_cursor_pos();
      update_mouse_position(pos);
    }
    return true;
  });

  Offset.on_change().connect([this](math::vec2 const& val) {
    auto pos = WindowManager::get().current()->get_cursor_pos();
    update_mouse_position(pos);
    return true;
  });

  Anchor.on_change().connect([this](math::vec2 const& val) {
    auto pos = WindowManager::get().current()->get_cursor_pos();
    update_mouse_position(pos);
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

GuiComponent::~GuiComponent() {
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

  if (js_window_) {
    delete js_window_;
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::update(double time) {
  Component::update(time);
  DepthComponent::update(time, get_user());
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    if (Interface::get().bind(o.Self->view_, ctx, 0)) {
      GuiShader::get().use(ctx);

      math::vec2 size(
        1.0 * o.Size.x() / ctx.window_size.x(),
        1.0 * o.Size.y() / ctx.window_size.y()
      );

      math::vec3 offset(
        (2.0 * o.Offset.x() + o.Anchor.x() * (ctx.window_size.x() - o.Size.x()))/ctx.window_size.x(),
        (2.0 * o.Offset.y() + o.Anchor.y() * (ctx.window_size.y() - o.Size.y()))/ctx.window_size.y(),
        0
      );

      GuiShader::get().size.Set(size);
      GuiShader::get().opacity.Set(o.Opacity);
      GuiShader::get().offset_rot.Set(offset);
      GuiShader::get().diffuse.Set(0);
      Quad::get().draw(ctx);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::reload() {
  view_->Reload(true);
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::focus() {
  view_->Focus();
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::call_javascript_impl(std::string const& method, std::vector<std::string> const& args) const {

  if (!js_window_) {
    return;
  }

  Awesomium::JSArray j_args;
  for (auto const& arg: args) {
    j_args.Push(Awesomium::JSValue(Awesomium::ToWebString(arg)));
  }
  js_window_->ToObject().Invoke(Awesomium::ToWebString(method), j_args);
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::add_javascript_callback(std::string const& name) {
  add_javascript_callback(name, false);
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::add_javascript_getter(std::string const& name, std::function<std::string()> callback) {
  add_javascript_callback(name, true);
  result_callbacks_[name] = callback;
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth   = WorldDepth();
  s.Opacity = Opacity();
  s.Size    = Size();
  s.Anchor  = Anchor();
  s.Offset  = Offset();
  s.Self    = shared_from_this();
  scene->renderers().gui_elements.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  DepthComponent::accept(visitor);
  visitor.add_member("Opacity",     Opacity);
  visitor.add_member("Resource",    Resource);
  visitor.add_member("Size",        Size);
  visitor.add_member("Anchor",      Anchor);
  visitor.add_member("Offset",      Offset);
  visitor.add_member("Interactive", Interactive);
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::update_mouse_position(math::vec2 const& pos) const {
  if (interactive_) {
    auto size(WindowManager::get().current()->get_context().window_size);

    math::vec2 corner(
      (size.x() - Size().x() + Anchor().x() * (size.x() - Size().x()))*0.5 + Offset().x(),
      (size.y() - Size().y() - Anchor().y() * (size.y() - Size().y()))*0.5 - Offset().y()
    );

    view_->InjectMouseMove(pos.x() - corner.x(), size.y() - pos.y() - corner.y());
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::add_javascript_callback(std::string const& callback, bool with_result) {
  Awesomium::JSValue o = view_->ExecuteJavascriptWithResult(
    Awesomium::WSLit("Swift2D"), Awesomium::WSLit("")
  );

  if (o.IsObject()) {
    o.ToObject().SetCustomMethod(Awesomium::ToWebString(callback), with_result);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
