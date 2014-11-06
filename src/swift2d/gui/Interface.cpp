////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/Interface.hpp>

#include <swift2d/utils/Logger.hpp>
#include <swift2d/utils/TextFile.hpp>
#include <swift2d/application/Paths.hpp>

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

#include <mutex>

// Awesomium bug in linux
#ifndef _WIN32
Awesomium::DataSource::~DataSource(){}
#endif

namespace swift {

namespace {

#include "GLSurface.ipp"
#include "GLSurfaceFactory.ipp"
#include "AweDataSource.ipp"

}

////////////////////////////////////////////////////////////////////////////////

void Interface::update() const {
  web_core_->Update();
}

////////////////////////////////////////////////////////////////////////////////

Interface::Interface()
  : LoadingProgress(1)
  , loading_state_(0)
  , max_state_(-1) {

  web_core_ = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
  web_core_->set_surface_factory(new GLSurfaceFactory());

  Awesomium::WebPreferences prefs;
  prefs.enable_smooth_scrolling = true;
  web_session_ = web_core_->CreateWebSession(Awesomium::WSLit(""), prefs);

  Awesomium::DataSource* data_source = new AweDataSource();
  web_session_->AddDataSource(Awesomium::WSLit("swift2d"), data_source);
}

////////////////////////////////////////////////////////////////////////////////

Interface::~Interface() {
  auto factory = static_cast<GLSurfaceFactory*>(web_core_->surface_factory());
  Awesomium::WebCore::Shutdown();
  delete factory;
  web_session_->Release();
}

////////////////////////////////////////////////////////////////////////////////

bool Interface::bind(Awesomium::WebView* view, RenderContext const& ctx, unsigned location) const {
  auto surface = static_cast<GLSurface*>(view->surface());
  return surface && surface->bind(ctx, location);
}

////////////////////////////////////////////////////////////////////////////////

Awesomium::WebView* Interface::create_webview(int width, int height) const {
  return web_core_->CreateWebView(width, height, web_session_,
                                  Awesomium::kWebViewType_Offscreen);
}

////////////////////////////////////////////////////////////////////////////////

void Interface::increase_loading_state() {
  ++loading_state_;

  if (loading_state_ > max_state_) {
    max_state_ = loading_state_;
  }

  if (max_state_ > 0) {
    LoadingProgress = 1.f - 1.f * loading_state_ / max_state_;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Interface::decrease_loading_state() {
  --loading_state_;

  if (loading_state_ == 0) {
    LoadingProgress = 1.f;
    max_state_ = -1;
  } else if (max_state_ > 0) {
    LoadingProgress = 1.f - 1.f * loading_state_ / max_state_;
  }
}

////////////////////////////////////////////////////////////////////////////////

}

