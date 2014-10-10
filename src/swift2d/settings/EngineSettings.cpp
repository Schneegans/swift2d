////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/settings/EngineSettings.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

EngineSettings::EngineSettings()
  : VSync(true)
  , Fullscreen(false)
  , ShadingQuality(5)
  , SubSampling(false)
  , LightSubSampling(false)
  , Gamma(1.f) {}

////////////////////////////////////////////////////////////////////////////////

void EngineSettings::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("VSync", VSync);
  visitor.add_member("Fullscreen", Fullscreen);
  visitor.add_member("ShadingQuality", ShadingQuality);
  visitor.add_member("SubSampling", SubSampling);
  visitor.add_member("LightSubSampling", LightSubSampling);
  visitor.add_member("Gamma", Gamma);
}

////////////////////////////////////////////////////////////////////////////////

}
