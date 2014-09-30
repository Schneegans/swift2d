////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SORTED_OBJECT_CONTAINER_HPP
#define SWIFT2D_SORTED_OBJECT_CONTAINER_HPP

#include <swift2d/utils/platform.hpp>
#include <swift2d/math.hpp>

#include <memory>
#include <vector>
#include <map>

namespace swift {

// forward declares ------------------------------------------------------------
class DrawableComponent;
typedef std::shared_ptr<DrawableComponent> DrawableComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL SortedObjectContainer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void add_object(DrawableComponentPtr const& object);
  void add_instanced_object(void* key, DrawableComponentPtr const& object);

  std::map<void*, std::pair<DrawableComponentPtr, std::vector<math::mat3>>> const& get_instanced_objects() const;
  std::vector<DrawableComponentPtr> const& get_objects() const;

 private:
  std::map<void*, std::pair<DrawableComponentPtr, std::vector<math::mat3>>> instanced_objects_;
  std::vector<DrawableComponentPtr> objects_;
};

}

#endif  // SWIFT2D_SORTED_OBJECT_CONTAINER_HPP
