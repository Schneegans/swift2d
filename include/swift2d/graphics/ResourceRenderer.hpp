////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RESOURCE_RENDERER_HPP
#define SWIFT2D_RESOURCE_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/platform.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

namespace swift {

class RenderContext;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL ResourceRendererBase {
 public:
  virtual bool predraw(float& min_depth) = 0;
  virtual bool draw(RenderContext const& ctx, float max_depth, float& next_depth) = 0;
};

// -----------------------------------------------------------------------------
template<typename T>
class SWIFT_DLL ResourceRenderer : public ResourceRendererBase {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  bool predraw(float& min_depth) {
    if (objects.size() == 0) {
      return false;
    }

    std::sort(objects.begin(), objects.end());
    cur_index_ = 0;
    if (objects[0].Depth < min_depth) {
      min_depth = objects[0].Depth;
    }

    return true;
  }

  // ---------------------------------------------------------------------------
  bool draw(RenderContext const& ctx, float max_depth, float& next_depth) {
    unsigned start(cur_index_);

    while(cur_index_ < objects.size() && objects[cur_index_].Depth <= max_depth) {
      ++cur_index_;
    }

    if (start != cur_index_) {
      draw_impl(ctx, start, cur_index_);
    }

    if (cur_index_ < objects.size()) {
      if (max_depth == next_depth || objects[cur_index_].Depth < next_depth) {
        next_depth = objects[cur_index_].Depth;
      }
      return true;
    }

    return false;
  };

  // ---------------------------------------------------------------------------
  void add(typename T::Serialized && object) {
    objects.push_back(object);
  }

  // ---------------------------------------------------------------------------
  bool empty() const {
    return objects.empty();
  }

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:
  virtual void draw_impl(RenderContext const& ctx, int start, int end) = 0;

  std::vector<typename T::Serialized> objects;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  unsigned cur_index_;
};

}

#endif  // SWIFT2D_RESOURCE_RENDERER_HPP
