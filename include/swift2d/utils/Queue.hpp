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

#ifndef SWIFT2D_QUEUE_HPP
#define SWIFT2D_QUEUE_HPP

// includes  -------------------------------------------------------------------
#include <queue>
#include <mutex>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A thread safe queue.                                                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
template<class T>
class SWIFT_DLL Queue {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  bool empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  unsigned size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
  }

  void push(T const& val, unsigned count = 1) {
    std::unique_lock<std::mutex> lock(mutex_);
    for (int i(0); i<count; ++i) {
      queue_.push(val);
    }
  }

  void push(std::vector<T> const& val) {
    std::unique_lock<std::mutex> lock(mutex_);
    for (auto const& t:val) {
      queue_.push(t);
    }
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    T val = queue_.front();
    queue_.pop();
    return val;
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::queue<T> queue_;
  mutable std::mutex mutex_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_QUEUE_HPP
