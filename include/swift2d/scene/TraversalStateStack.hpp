////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TRAVERSAL_STATE_STACK_HPP
#define SWIFT2D_TRAVERSAL_STATE_STACK_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <stack>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class TraversalStateMap {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  template <typename T>
    std::shared_ptr<T> get() {
      auto it(map_.find(typeid(T)));
      if (it == map_.end()) {
        return nullptr;
      }

      return std::dynamic_pointer_cast<T>(it->second);
    }

    template <typename T>
    void set(std::shared_ptr<T> const& item) {
      map_[typeid(T)] = item;
    }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  std::unordered_map<std::type_index, std::shared_ptr<cores::Core::TraversalState>> map_;
};



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class TraversalStateStack {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void save();
  void restore();

  TraversalStateMap get_all() {
    if (state_.empty()) {
      return TraversalStateMap();
    }

    return state_.top();
  }

  template <typename T>
  std::shared_ptr<T> get() {
    if (state_.empty()) {
      return nullptr;
    }

    return state_.top().get<T>();
  }

  template <typename T>
  void set(std::shared_ptr<T> const& item) {
    if (state_.empty()) {
      save();
    }
    state_.top().set(item);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  std::stack<TraversalStateMap> state_;

};

}

#endif  // SWIFT2D_TRAVERSAL_STATE_STACK_HPP
