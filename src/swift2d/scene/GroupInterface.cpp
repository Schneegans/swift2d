////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/GroupInterface.hpp>
#include <swift2d/scene/Node.hpp>

namespace swift {

namespace cores {

////////////////////////////////////////////////////////////////////////////////

void GroupInterface::add_node(NodePtr const& child) {
  child->add_parent(this);
}

////////////////////////////////////////////////////////////////////////////////

void GroupInterface::remove_node(NodePtr const& child) {
  child->remove_parent(this);
}


}

}
