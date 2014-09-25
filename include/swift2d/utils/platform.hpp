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
#define SWIFT_DLL __declspec(dllexport)
#else // _WIN32
#define SWIFT_DLL
#endif // else _WIN32

}

#endif // SWIFT2D_PLATFORM_HPP
