////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PLATFORM_HPP
#define SWIFT2D_PLATFORM_HPP

namespace swift {

#if defined( _WIN32 )
  #if defined( SWIFT_MAKE_LIBRARY )
    #define SWIFT_DLL __declspec(dllexport)
  #else
    #define SWIFT_DLL __declspec(dllimport)
  #endif
#else
  #define SWIFT_DLL
#endif

}

#endif // SWIFT2D_PLATFORM_HPP
