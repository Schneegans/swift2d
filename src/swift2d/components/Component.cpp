////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Component::Component()
  : Enabled(true)
  , user_(nullptr)
  , remove_flag_(false) {}

////////////////////////////////////////////////////////////////////////////////

void Component::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("Enabled", Enabled);
  visitor.add_member("Label", Label);
}

////////////////////////////////////////////////////////////////////////////////

}

