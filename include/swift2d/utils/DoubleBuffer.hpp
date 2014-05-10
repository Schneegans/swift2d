////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DOUBLEBUFFER_HPP
#define SWIFT2D_DOUBLEBUFFER_HPP

// includes  -------------------------------------------------------------------
#include <thread>
#include <condition_variable>

namespace swift {

// -----------------------------------------------------------------------------
template <typename T> class DoubleBuffer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  DoubleBuffer()
      : front_(), back_(), updated_(false), copy_mutex_(), copy_cond_var_() {}

  void write_blocked(T const& scene_graphs) {
    {
      // blocks until ownership can be obtained for the current thread.
      std::lock_guard<std::mutex> lock(copy_mutex_);
      back_ = scene_graphs;
      updated_ = true;
    }
    copy_cond_var_.notify_one();
  }

  template <typename F> void with(F&& f) {
    // f(front_);
    {
      std::unique_lock<std::mutex> lock(copy_mutex_);
      while (!updated_) {
        copy_cond_var_.wait(lock);
      }
      updated_ = false;
      std::swap(front_, back_);
    }
    f(front_);
  }

  T read() {
    {
      std::unique_lock<std::mutex> lock(copy_mutex_);
      while (!updated_) {
        copy_cond_var_.wait(lock);
      }
      updated_ = false;
      std::swap(front_, back_);
    }
    return front_;
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  T front_;
  T back_;
  bool updated_;

  std::mutex copy_mutex_;
  std::condition_variable copy_cond_var_;
};

}

#endif  // SWIFT2D_DOUBLEBUFFER_HPP
