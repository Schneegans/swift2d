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

#ifndef SWIFT2D_BUFFER_HPP
#define SWIFT2D_BUFFER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/openal.hpp>

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class AudioBuffer;
typedef std::shared_ptr<AudioBuffer>       AudioBufferPtr;
typedef std::shared_ptr<const AudioBuffer> ConstAudioBufferPtr;
typedef Property<AudioBufferPtr>           AudioBufferProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL AudioBuffer : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  virtual void load(oalplus::Source* source) = 0;
  virtual void unload(oalplus::Source* source) = 0;
  virtual void update(oalplus::Source* source, double time) {};
};

}

#endif // SWIFT2D_BUFFER_HPP
