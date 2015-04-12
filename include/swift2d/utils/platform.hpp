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

#ifndef SWIFT2D_PLATFORM_HPP
#define SWIFT2D_PLATFORM_HPP

namespace swift {

#if defined( _WIN32 )
  #if defined( SWIFT_MAKE_LIBRARY )
    #define SWIFT_DLL //__declspec(dllexport)
  #else
   #define SWIFT_DLL //__declspec(dllexport) 
  #endif
#else
  #define SWIFT_DLL
#endif

}

#endif // SWIFT2D_PLATFORM_HPP
