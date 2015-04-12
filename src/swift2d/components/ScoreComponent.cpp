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

#include <swift2d/components/ScoreComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ScoreComponent::ScoreComponent()
  : Score(0) {}

////////////////////////////////////////////////////////////////////////////////

void ScoreComponent::increase(int amount) {
  Score = Score() + amount;
}

////////////////////////////////////////////////////////////////////////////////

void ScoreComponent::decrease(int amount) {
  Score = std::max(0, Score() - amount);
}

////////////////////////////////////////////////////////////////////////////////

void ScoreComponent::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("Score", Score);
}

////////////////////////////////////////////////////////////////////////////////

}
