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
  virtual bool predraw(RenderContext const& ctx, float& min_depth) = 0;
  virtual bool draw(RenderContext const& ctx, float max_depth, float& next_depth) = 0;
};

// -----------------------------------------------------------------------------
template<typename T>
class SWIFT_DLL ResourceRenderer : public ResourceRendererBase {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  bool predraw(RenderContext const& ctx, float& min_depth) {
    if (objects.size() == 0) {
      draw_no_objects(ctx);
      return false;
    }

    std::sort(objects.begin(), objects.end());
    cur_index_ = 0;
    if (objects[0].Depth < min_depth) {
      min_depth = objects[0].Depth;
    }

    predraw(ctx);

    return true;
  }

  // ---------------------------------------------------------------------------
  bool draw(RenderContext const& ctx, float max_depth, float& next_depth) {
    unsigned start(cur_index_);

    while(cur_index_ < objects.size() && objects[cur_index_].Depth <= max_depth) {
      ++cur_index_;
    }

    if (start != cur_index_) {
      draw(ctx, start, cur_index_);
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
  virtual void draw(RenderContext const& ctx, int start, int end) = 0;
  virtual void draw_no_objects(RenderContext const& ctx) {};
  virtual void predraw(RenderContext const& ctx) {};

  std::vector<typename T::Serialized> objects;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  unsigned cur_index_;
};

}

#endif  // SWIFT2D_RESOURCE_RENDERER_HPP
