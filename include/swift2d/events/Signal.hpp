////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_SIGNAL_HPP
#define SWIFT2D_EVENTS_SIGNAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/platform.hpp>

#include <functional>
#include <mutex>
#include <map>

namespace swift {

// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// A signal object may call multiple callbacks with the same signature. You   //
// can connect functions to the signal which will be called when the emit()   //
// method on the signal object is invoked. Any argument passed to emit() will //
// be passed to the given function.                                           //
// Connect and disconnect methods may be called from different threads, but   //
// the callbacks will be called from the thread calling emit().               //
////////////////////////////////////////////////////////////////////////////////

template <typename... Parameters>
class Signal {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  Signal() : current_id_(0) {}

  // copy creates new signal
  Signal(Signal const& other) : current_id_(0) {}

  // ------------------------------------------------------------ public methods

  // connects a member function of a given object to this Signal
  template <typename F, typename... Args>
  int connect_member(F&& f, Args&& ... a) const {
    std::unique_lock<std::mutex> lock(mutex_);
    callbacks_.insert(std::make_pair(++current_id_, std::bind(f, a...)));
    return current_id_;
  }

  // connects a std::function to the signal. The returned value can be used to
  // disconnect the function again
  int connect(std::function<bool(Parameters...)> const& callback) const {
    std::unique_lock<std::mutex> lock(mutex_);
    callbacks_.insert(std::make_pair(++current_id_, callback));
    return current_id_;
  }

  // disconnects a previously connected function
  void disconnect(int id) const {
    std::unique_lock<std::mutex> lock(mutex_);
    callbacks_.erase(id);
  }

  // disconnects all previously connected functions
  void disconnect_all() const {
    std::unique_lock<std::mutex> lock(mutex_);
    callbacks_.clear();
    current_id_ = 0;
  }

  // calls all connected functions
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

  // assignment creates new Signal
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
