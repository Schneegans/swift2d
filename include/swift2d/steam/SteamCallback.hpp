////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STEAM_CALLBACK_HPP
#define SWIFT2D_STEAM_CALLBACK_HPP

// includes  -------------------------------------------------------------------
#include <steam/steam_api.h>
#include <functional>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
template<typename T>
class SWIFT_DLL SteamCallback {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  SteamCallback(std::function<void(T*)> f)
    : member_(this, &SteamCallback<T>::run)
    , f_(f) {}

 private:

  STEAM_CALLBACK( SteamCallback<T>, run, T, member_);

  std::function<void(T*)> f_;
};

template<typename T>
void SteamCallback<T>::run(T* result) {
  f_(result);
}

}

#endif  // SWIFT2D_STEAM_CALLBACK_HPP

