////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_SIGNAL_HPP
#define SWIFT2D_EVENTS_SIGNAL_HPP

// includes  ------------------------------------------------------------------
#include <swift2d/utils/platform.hpp>

#include <functional>
#include <map>

namespace swift {

// -----------------------------------------------------------------------------
template <typename... Parameters>
class SWIFT_DLL Signal {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  Signal() : current_id_(0) {}

  // ------------------------------------------------------------ public methods
  template <typename F, typename... Args>
  int connect_member(F&& f, Args&& ... a) const {
    callbacks_.insert(std::make_pair(++current_id_, std::bind(f, a...)));
    return current_id_;
  }

  int connect(std::function<void(Parameters...)> const& callback) const {
    callbacks_.insert(std::make_pair(++current_id_, callback));
    return current_id_;
  }

  void disconnect(int id) const { callbacks_.erase(id); }

  void disconnect_all() const { callbacks_.clear(); }

  void emit(Parameters... p) {
    for (auto& callback : callbacks_) {
      callback.second(p...);
    }
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  mutable std::map<int, std::function<void(Parameters...)>> callbacks_;
  mutable int current_id_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_SIGNAL_HPP */
