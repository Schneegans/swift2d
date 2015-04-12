////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class GLSurfaceFactory : public Awesomium::SurfaceFactory {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  Awesomium::Surface* CreateSurface(Awesomium::WebView* view,
                                    int width, int height) {
    return new GLSurface(width, height);
  }

  void DestroySurface(Awesomium::Surface* surface) {
    delete static_cast<GLSurface*>(surface);
  }

};
