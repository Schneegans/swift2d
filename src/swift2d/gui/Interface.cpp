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

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

#include <fstream>

// Awesomium bug in linux
Awesomium::DataSource::~DataSource(){}

namespace swift {

namespace {

////////////////////////////////////////////////////////////////////////////////

class GLSurface : public Awesomium::Surface {

 public:

  //////////////////////////////////////////////////////////////////////////////

  GLSurface(int width, int height)
    : tex_(nullptr)
    , buffer_(new unsigned char[width * height * 4])
    , needs_update_(false)
    , width_(width)
    , height_(height) {}

  //////////////////////////////////////////////////////////////////////////////

  ~GLSurface() {
    delete   tex_;
    delete[] buffer_;
  }

  //////////////////////////////////////////////////////////////////////////////

  void init(RenderContext const& ctx) {
    tex_ = new oglplus::Texture();
    oglplus::Texture::Active(0);
    ctx.gl.Bound(oglplus::Texture::Target::_2D, *tex_)
      .MinFilter(oglplus::TextureMinFilter::Linear)
      .MagFilter(oglplus::TextureMagFilter::Linear)
      .WrapS(oglplus::TextureWrap::ClampToEdge)
      .WrapT(oglplus::TextureWrap::ClampToEdge)
      .Image2D(0, oglplus::PixelDataInternalFormat::RGBA, width_, height_,
        0, oglplus::PixelDataFormat::BGRA,
        oglplus::PixelDataType::UnsignedByte, buffer_
      );
  }

  //////////////////////////////////////////////////////////////////////////////

  void bind(RenderContext const& ctx, unsigned location) {

    if (!tex_) {
      init(ctx);
    }

    tex_->Active(location);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *tex_);

    if (needs_update_) {
      tex_->SubImage2D(
        oglplus::Texture::Target::_2D, 0, 0, 0, width_, height_,
        oglplus::PixelDataFormat::BGRA, oglplus::PixelDataType::UnsignedByte,
        buffer_
      );

      needs_update_ = false;
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  void Paint(unsigned char* src_buffer, int src_row_span,
             Awesomium::Rect const& src_rect,
             Awesomium::Rect const& dest_rect) {

    for (int row = 0; row < dest_rect.height; row++)
      memcpy(buffer_ + (row + dest_rect.y) * width_*4 + (dest_rect.x * 4),
             src_buffer + (row + src_rect.y) * src_row_span + (src_rect.x * 4),
             dest_rect.width * 4);

    needs_update_ = true;
  }

  //////////////////////////////////////////////////////////////////////////////

  void Scroll(int dx, int dy, Awesomium::Rect const& clip_rect) {

    if (abs(dx) >= clip_rect.width || abs(dy) >= clip_rect.height)
      return;

      if (dx < 0 && dy == 0) {
      // Area shifted left by dx
      unsigned char* tempBuffer = new unsigned char[(clip_rect.width + dx) * 4];

      for (int i = 0; i < clip_rect.height; i++) {
        memcpy(tempBuffer, buffer_ + (i + clip_rect.y) * width_*4 +
               (clip_rect.x - dx) * 4, (clip_rect.width + dx) * 4);
        memcpy(buffer_ + (i + clip_rect.y) * width_*4 + (clip_rect.x) * 4,
               tempBuffer, (clip_rect.width + dx) * 4);
      }

      delete[] tempBuffer;
    } else if (dx > 0 && dy == 0) {
      // Area shifted right by dx
      unsigned char* tempBuffer = new unsigned char[(clip_rect.width - dx) * 4];

      for (int i = 0; i < clip_rect.height; i++) {
        memcpy(tempBuffer, buffer_ + (i + clip_rect.y) * width_*4 +
               (clip_rect.x) * 4, (clip_rect.width - dx) * 4);
        memcpy(buffer_ + (i + clip_rect.y) * width_*4 + (clip_rect.x + dx) * 4,
               tempBuffer, (clip_rect.width - dx) * 4);
      }

      delete[] tempBuffer;
    } else if (dy < 0 && dx == 0) {
      // Area shifted down by dy
      for (int i = 0; i < clip_rect.height + dy ; i++)
        memcpy(buffer_ + (i + clip_rect.y) * width_*4 + (clip_rect.x * 4),
               buffer_ + (i + clip_rect.y - dy) * width_*4 + (clip_rect.x * 4),
               clip_rect.width * 4);
    } else if (dy > 0 && dx == 0) {
      // Area shifted up by dy
      for (int i = clip_rect.height - 1; i >= dy; i--)
        memcpy(buffer_ + (i + clip_rect.y) * width_*4 + (clip_rect.x * 4),
               buffer_ + (i + clip_rect.y - dy) * width_*4 + (clip_rect.x * 4),
               clip_rect.width * 4);
    }

    needs_update_ = true;
  }

 private:
  oglplus::Texture* tex_;
  unsigned char*    buffer_;
  bool              needs_update_;
  int               width_;
  int               height_;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class GLSurfaceFactory : public Awesomium::SurfaceFactory {

 public:
  GLSurfaceFactory() {}
  ~GLSurfaceFactory() {}

  //////////////////////////////////////////////////////////////////////////////

  Awesomium::Surface* CreateSurface(Awesomium::WebView* view,
                                    int width, int height) {
    return new GLSurface(width, height);
  }

  //////////////////////////////////////////////////////////////////////////////

  void DestroySurface(Awesomium::Surface* surface) {
    delete static_cast<GLSurface*>(surface);
  }

  //////////////////////////////////////////////////////////////////////////////
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SwiftDataSource : public Awesomium::DataSource {

 public:

  void OnRequest(int request_id, Awesomium::ResourceRequest const& request,
                 Awesomium::WebString const& path) {

    std::string html_str("<h1>Failed to load resource.</h1>");
    std::ifstream ifs(Awesomium::ToString(path), std::ios::in | std::ios::ate);

    if (ifs) {
      std::ifstream::pos_type fileSize = ifs.tellg();
      ifs.seekg(0, std::ios::beg);
      std::vector<char> bytes(fileSize);
      ifs.read(&bytes[0], fileSize);
      html_str = std::string(&bytes[0], fileSize);
    } else {
      Logger::LOG_WARNING << "Failed to load resource \"" << path << "\": File not found!" << std::endl;
    }

    SendResponse(request_id, html_str.size(), (unsigned char*)html_str.c_str(),
                 Awesomium::WSLit("text/html"));
  }

};

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Interface::update() const {
  web_core_->Update();
}

////////////////////////////////////////////////////////////////////////////////

Interface::Interface() {
  web_core_ = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
  web_core_->set_surface_factory(new GLSurfaceFactory());

  Awesomium::WebPreferences prefs;
  prefs.enable_smooth_scrolling = true;
  web_session_ = web_core_->CreateWebSession(Awesomium::WSLit(""), prefs);

  Awesomium::DataSource* data_source = new SwiftDataSource();
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
  if (surface) {
    surface->bind(ctx, location);
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////

Awesomium::WebView* Interface::create_webview(int width, int height) const {
  return web_core_->CreateWebView(width, height, web_session_,
                                  Awesomium::kWebViewType_Offscreen);
}

////////////////////////////////////////////////////////////////////////////////

}

