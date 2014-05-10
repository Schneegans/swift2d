////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GROUP_INTERFACE_CORE_HPP
#define SWIFT2D_GROUP_INTERFACE_CORE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class GroupInterface;
typedef std::shared_ptr<GroupInterface>       GroupInterfacePtr;
typedef std::shared_ptr<const GroupInterface> ConstGroupInterfacePtr;

// -----------------------------------------------------------------------------
class GroupInterface : public Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

    virtual void add_node(NodePtr const& child);
    virtual void remove_node(NodePtr const& child);
};

}}

#endif  // SWIFT2D_GROUP_INTERFACE_CORE_HPP
