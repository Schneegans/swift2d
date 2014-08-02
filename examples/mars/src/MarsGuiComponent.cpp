////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/MarsGuiComponent.hpp"

////////////////////////////////////////////////////////////////////////////////

MarsGuiComponent::MarsGuiComponent(
  std::unordered_map<std::string, std::function<void()>> const& callbacks,
  bool hide_top, int overlap)
  : top_(-1.f, 0.f, 0.f, swift::AnimatedFloat::IN_OUT, swift::AnimatedFloat::NONE, 1.5f)
  , hide_top_(hide_top)
  , visible_(false)
  , overlap_(overlap) {

  Enabled = false;
  Interactive = false;

  on_loaded.connect([&](){
    for (auto c: callbacks) {
      add_javascript_callback(c.first);
    }
  });

  on_javascript_callback.connect([&](std::string const& method) {
    auto c(callbacks.find(method));
    if (c != callbacks.end()) {
      c->second();
    } else {
      swift::Logger::LOG_WARNING << "Failed to execute callback \"" << method
                                 << "\": No such callback defined!" << std::endl;
    }
  });

  top_.on_finish().connect([&](){
    if (!visible_) {
      Enabled = false;
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

void MarsGuiComponent::show() {
  Offset = swift::math::vec2i(Offset().x(), Size().y());
  Enabled = true;

  if (top_() == -1.f) {
    top_.set(Size().y());
  }

  top_.set(overlap_, 0.75f);
  visible_ = true;
}

////////////////////////////////////////////////////////////////////////////////

void MarsGuiComponent::hide() {
  top_.set(Size().y(), 0.75);
  visible_ = false;
}

////////////////////////////////////////////////////////////////////////////////

bool MarsGuiComponent::is_visible() const {
  return visible_;
}

////////////////////////////////////////////////////////////////////////////////

void MarsGuiComponent::update(double time) {
  if (Enabled() && visible_ && Interactive()) {
    swift::GuiComponent::update(time);
  }

  top_.update(time);

  if (hide_top_) {
    Offset = swift::math::vec2(Offset().x(), top_());
  } else {
    Offset = swift::math::vec2(Offset().x(), -top_());
  }

}

////////////////////////////////////////////////////////////////////////////////



