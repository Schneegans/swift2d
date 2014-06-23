////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_INTERFACE_HPP
#define SWIFT2D_INTERFACE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>

// forward declares ------------------------------------------------------------
namespace Awesomium {
  class WebCore;
  class WebView;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Interface : public Singleton<Interface> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void update() const;

  friend class Singleton<Interface>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Interface();
  ~Interface();

  Awesomium::WebCore* web_core_;
  Awesomium::WebView* view_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_INTERFACE_HPP
