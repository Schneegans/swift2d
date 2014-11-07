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
#include <mutex>
#include <map>

namespace swift {

// -----------------------------------------------------------------------------
template <typename... Parameters>
class Signal {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  Signal() : current_id_(0) {}

  // copy creates new signal
  Signal(Signal const& other) : current_id_(0) {}

  // ------------------------------------------------------------ public methods
  template <typename F, typename... Args>
  int connect_member(F&& f, Args&& ... a) const {
    std::unique_lock<std::mutex> lock(mutex_);
    callbacks_.insert(std::make_pair(++current_id_, std::bind(f, a...)));
    return current_id_;
  }

  int connect(std::function<bool(Parameters...)> const& callback) const {
    std::unique_lock<std::mutex> lock(mutex_);
    callbacks_.insert(std::make_pair(++current_id_, callback));
    return current_id_;
  }

  void disconnect(int id) const {
    std::unique_lock<std::mutex> lock(mutex_);
    callbacks_.erase(id);
  }

  void disconnect_all() const {
    std::unique_lock<std::mutex> lock(mutex_);
    callbacks_.clear();
    current_id_ = 0;
  }

  void emit(Parameters... p) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto it(callbacks_.begin());
    while(it != callbacks_.end()) {
      if (it->second(p...)) {
        ++it;
      } else {
        it = callbacks_.erase(it);
      }
    }
  }

  Signal& operator=(Signal const& other) {
    disconnect_all();
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  mutable std::map<int, std::function<bool(Parameters...)>> callbacks_;
  mutable int current_id_;

  mutable std::mutex mutex_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_SIGNAL_HPP */
