
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

////////////////////////////////////////////////////////////////////////////////

const int KEYS_TO_AWE_KEYS[] = {
  0x1B, // GLFW_KEY_ESCAPE
  0x0D, // GLFW_KEY_ENTER
  0x09, // GLFW_KEY_TAB
  0x08, // GLFW_KEY_BACKSPACE
  0x2D, // GLFW_KEY_INSERT
  0x2E, // GLFE_KEY_DELETE
  0x27, // GLFW_KEY_RIGHT
  0x25, // GLFW_KEY_LEFT
  0x28, // GLFW_KEY_DOWN
  0x26, // GLFW_KEY_UP
  0x21, // GLFW_KEY_PAGE_UP
  0x22, // GLFW_KEY_PAGE_DOWN
  0x24, // GLFW_KEY_HOME
  0x23, // GLFW_KEY_END

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x14, // GLFW_KEY_CAPS_LOCK
  0x91, // GLFW_KEY_SCROLL_LOCK
  0x90, // GLFW_KEY_NUM_LOCK
  0x2A, // GLFW_KEY_PRINT_SCREEN
  0x13, // GLFW_KEY_PAUSE

  0x00, 0x00, 0x00, 0x00, 0x00,

  0x70, // GLFW_KEY_F1
  0x71, // GLFW_KEY_F2
  0x72, // GLFW_KEY_F3
  0x73, // GLFW_KEY_F4
  0x74, // GLFW_KEY_F5
  0x75, // GLFW_KEY_F6
  0x76, // GLFW_KEY_F7
  0x77, // GLFW_KEY_F8
  0x78, // GLFW_KEY_F9
  0x79, // GLFW_KEY_F10
  0x7A, // GLFW_KEY_F11
  0x7B, // GLFW_KEY_F12
  0x7C, // GLFW_KEY_F13
  0x7D, // GLFW_KEY_F14
  0x7E, // GLFW_KEY_F15
  0x7F, // GLFW_KEY_F16
  0x80, // GLFW_KEY_F17
  0x81, // GLFW_KEY_F18
  0x82, // GLFW_KEY_F19
  0x83, // GLFW_KEY_F20
  0x84, // GLFW_KEY_F21
  0x85, // GLFW_KEY_F22
  0x86, // GLFW_KEY_F23
  0x87, // GLFW_KEY_F24
  0x00, // GLFW_KEY_F25 -- No awe equivalent

  0x00, 0x00, 0x00, 0x00, 0x00,

  0x60, // GLFW_KEY_KP_0
  0x61, // GLFW_KEY_KP_1
  0x62, // GLFW_KEY_KP_2
  0x63, // GLFW_KEY_KP_3
  0x64, // GLFW_KEY_KP_4
  0x65, // GLFW_KEY_KP_5
  0x66, // GLFW_KEY_KP_6
  0x67, // GLFW_KEY_KP_7
  0x68, // GLFW_KEY_KP_8
  0x69, // GLFW_KEY_KP_9
  0x6E, // GLFW_KEY_KP_DECIMAL
  0x6F, // GLFW_KEY_KP_DIVIDE
  0x6A, // GLFW_KEY_KP_MULTIPLY
  0x6D, // GLFW_KEY_KP_SUBTRACT
  0x6B, // GLFW_KEY_KP_ADD
  0x6C, // GLFW_KEY_KP_ENTER
  0x00, // GLFW_KEY_KP_EQUAL -- No awe equivalent

  0x00, 0x00, 0x00,

  0xA0, // GLFW_KEY_LEFT_SHIFT
  0xA2, // GLFW_KEY_LEFT_CONTROL
  0xA4, // GLFW_KEY_LEFT_ALT
  0x5B, // GLFW_KEY_LEFT_SUPER
  0xA1, // GLFW_KEY_RIGHT_SHIFT
  0xA3, // GLFW_KEY_RIGHT_CONTROL
  0xA5, // GLFW_KEY_RIGHT_ALT
  0x5C, // GLFW_KEY_RIGHT_SUPER
  0x12 // GLFW_KEY_MENU
};

int key_to_awe_key(Key key) {
  switch(key) {
    case Key::SPACE:          return 0x20;
    case Key::APOSTROPHE:     return 0xBA;
    case Key::COMMA:          return 0xBC;
    case Key::MINUS:          return 0xBD;
    case Key::PERIOD:         return 0xBE;
    case Key::SLASH:          return 0xBF;
    case Key::KEY_0:
    case Key::KEY_1:
    case Key::KEY_2:
    case Key::KEY_3:
    case Key::KEY_4:
    case Key::KEY_5:
    case Key::KEY_6:
    case Key::KEY_7:
    case Key::KEY_8:
    case Key::KEY_9:          return static_cast<int>(key);
    case Key::SEMICOLON:      return 0x00;
    case Key::EQUAL:          return 0xBB;
    case Key::A:
    case Key::B:
    case Key::C:
    case Key::D:
    case Key::E:
    case Key::F:
    case Key::G:
    case Key::H:
    case Key::I:
    case Key::J:
    case Key::K:
    case Key::L:
    case Key::M:
    case Key::N:
    case Key::O:
    case Key::P:
    case Key::Q:
    case Key::R:
    case Key::S:
    case Key::T:
    case Key::U:
    case Key::V:
    case Key::W:
    case Key::X:
    case Key::Y:
    case Key::Z:              return static_cast<int>(key);
    case Key::LEFT_BRACKET:   return 0x00;
    case Key::BACKSLASH:      return 0x00;
    case Key::RIGHT_BRACKET:  return 0x00;
    case Key::GRAVE_ACCENT:   return 0x00;
    case Key::WORLD_1:        return 0x00;
    case Key::WORLD_2:        return 0x00;
    default: return KEYS_TO_AWE_KEYS[static_cast<int>(key) - static_cast<int>(Key::ESCAPE)];
  }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class AweKeyEvent : public Awesomium::WebKeyboardEvent {

 public:
  AweKeyEvent(unsigned c) {
    type = Awesomium::WebKeyboardEvent::kTypeChar;
    modifiers = 0;
    virtual_key_code = static_cast<int>(c);
    native_key_code = 0;
    text[0] = static_cast<wchar16>(c);
  }

  AweKeyEvent(Key key, int scancode, int action, int mods) {
    int key_awe = key_to_awe_key(key);

    auto awe_type = Awesomium::WebKeyboardEvent::kTypeKeyUp;
    auto awe_mods = mods;

    if (action != 0) {
      awe_type = Awesomium::WebKeyboardEvent::kTypeKeyDown;
    }

    if (action == 2) {
      awe_mods |= Awesomium::WebKeyboardEvent::kModIsAutorepeat;
    }

    type = awe_type;
    modifiers = awe_mods;
    virtual_key_code = key_awe;
    native_key_code = static_cast<int>(key);
  }
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SwiftViewListener : public Awesomium::WebViewListener::View {

 public:
  void OnChangeTitle(
    Awesomium::WebView* caller,
    const Awesomium::WebString& title) {}

  void OnChangeAddressBar(
    Awesomium::WebView* caller,
    const Awesomium::WebURL& url) {}

  void OnChangeTooltip(
    Awesomium::WebView* caller,
    const Awesomium::WebString& tooltip) {}

  void OnChangeTargetURL(
    Awesomium::WebView* caller,
    const Awesomium::WebURL& url) {}

  void OnChangeCursor(
    Awesomium::WebView* caller,
    Awesomium::Cursor cursor) {}

  void OnChangeFocus(
    Awesomium::WebView* caller,
    Awesomium::FocusedElementType focused_type) {}

  void OnAddConsoleMessage(
    Awesomium::WebView* caller, const Awesomium::WebString& message,
    int line_number, const Awesomium::WebString& source) {

    Logger::LOG_MESSAGE << message << " (" << source << ":"
                        << line_number << ")" << std::endl;
  }

  void OnShowCreatedWebView(
    Awesomium::WebView* caller, Awesomium::WebView* new_view,
    const Awesomium::WebURL& opener_url, const Awesomium::WebURL& target_url,
    const Awesomium::Rect& initial_pos, bool is_popup) {}

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SwiftLoadListener : public Awesomium::WebViewListener::Load {

 public:
  void OnBeginLoadingFrame(
    Awesomium::WebView* caller, int64 frame_id,
    bool is_main_frame, const Awesomium::WebURL& url,
    bool is_error_page) {

    Logger::LOG_DEBUG << "OnBeginLoadingFrame" << std::endl;
  }

  void OnFailLoadingFrame(
    Awesomium::WebView* caller, int64 frame_id,
    bool is_main_frame, const Awesomium::WebURL& url,
    int error_code, const Awesomium::WebString& error_desc) {

    Logger::LOG_DEBUG << "OnFailLoadingFrame" << std::endl;
  }

  void OnFinishLoadingFrame(
    Awesomium::WebView* caller, int64 frame_id,
    bool is_main_frame, const Awesomium::WebURL& url) {

    Logger::LOG_DEBUG << "OnFinishLoadingFrame" << std::endl;
  }

  void OnDocumentReady(
    Awesomium::WebView* caller, const Awesomium::WebURL& url) {

    Logger::LOG_DEBUG << "OnDocumentReady" << std::endl;
  }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SwiftProcessListener : public Awesomium::WebViewListener::Process {

 public:
  void OnUnresponsive(Awesomium::WebView* caller) {
    Logger::LOG_WARNING << "OnUnresponsive" << std::endl;
  }

  void OnResponsive(Awesomium::WebView* caller) {
    Logger::LOG_WARNING << "OnResponsive" << std::endl;
  }

  void OnCrashed(Awesomium::WebView* caller,
                         Awesomium::TerminationStatus status) {
    Logger::LOG_WARNING << "OnCrashed" << std::endl;
  }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

GuiElement::GuiElement(GuiComponent* parent)
  : view_(nullptr)
  , callbacks_(5)
  , parent_(parent) {

  view_ = Interface::instance()->create_webview(parent_->Size().x(), parent_->Size().y());
  view_->SetTransparent(true);
  view_->Focus();
  view_->set_view_listener(new SwiftViewListener());
  view_->set_load_listener(new SwiftLoadListener());
  view_->set_process_listener(new SwiftProcessListener());

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
    // Awesomium::WebURL url(Awesomium::WSLit("http://www.google.de"));
    Awesomium::WebURL url(Awesomium::WSLit(("asset://swift2d/" + val).c_str()));
    view_->LoadURL(url);
  });

  parent_->Size.on_change().connect([&](math::vec2i const& val) {
    view_->Resize(val.x(), val.y());
  });
}

////////////////////////////////////////////////////////////////////////////////

GuiElement::~GuiElement() {
  delete static_cast<SwiftViewListener*>(view_->view_listener());
  delete static_cast<SwiftLoadListener*>(view_->load_listener());
  delete static_cast<SwiftProcessListener*>(view_->process_listener());
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
////////////////////////////////////////////////////////////////////////////////

}

