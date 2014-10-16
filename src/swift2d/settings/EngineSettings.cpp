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
  , DynamicLighting(true)
  , SubSampling(false)
  , LightSubSampling(false)
  , LensFlares(true)
  , HeatEffect(true)
  , Gamma(1.f) {}

////////////////////////////////////////////////////////////////////////////////

void EngineSettings::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("VSync", VSync);
  visitor.add_member("Fullscreen", Fullscreen);
  visitor.add_member("DynamicLighting", DynamicLighting);
  visitor.add_member("SubSampling", SubSampling);
  visitor.add_member("LightSubSampling", LightSubSampling);
  visitor.add_member("LensFlares", LensFlares);
  visitor.add_member("HeatEffect", HeatEffect);
  visitor.add_member("Gamma", Gamma);
}

////////////////////////////////////////////////////////////////////////////////

}
