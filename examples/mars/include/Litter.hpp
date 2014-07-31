////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_LITTER_HPP
#define MARS_LITTER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

using namespace swift;

class Litter: public SceneObject {
 public:

  Litter(math::vec2 const& pos, math::vec2 const& dir);

  virtual void update(double time);

 private:
  bool initialized_;
  static bool assets_loaded_;
  static PointParticleSystemComponentPtr smoke_;

  math::vec2 dir_;
};

// -----------------------------------------------------------------------------

#endif // MARS_LITTER_HPP
