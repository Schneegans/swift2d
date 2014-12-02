////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_QUEUE_HPP
#define SWIFT2D_QUEUE_HPP

// includes  -------------------------------------------------------------------
#include <queue>
#include <mutex>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
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

  void push(T const& val) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(val);
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
  std::mutex mutex_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_QUEUE_HPP
