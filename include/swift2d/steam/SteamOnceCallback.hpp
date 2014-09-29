////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STEAM_ONCE_CALLBACK_HPP
#define SWIFT2D_STEAM_ONCE_CALLBACK_HPP

// includes  -------------------------------------------------------------------
#include <steam/steam_api.h>
#include <functional>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
template<typename T>
class SWIFT_DLL SteamOnceCallback {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static void set(SteamAPICall_t c, std::function<void(T*, bool)> f) {
    auto handler(new SteamOnceCallback<T>());
    handler->init(c, f);
  }

 private:
  void init(SteamAPICall_t c, std::function<void(T*, bool)> f) {
    callback.Set(c, this, &SteamOnceCallback<T>::run);
    f_ = f;
  }

  void run(T* result, bool fail) {
    f_(result, fail);
    delete this;
  }

  CCallResult<SteamOnceCallback<T>, T> callback;
  std::function<void(T*, bool)> f_;
};

}

#endif  // SWIFT2D_STEAM_ONCE_CALLBACK_HPP

