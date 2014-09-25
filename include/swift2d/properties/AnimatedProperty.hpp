////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ANIMATED_PROPERTY_HPP
#define SWIFT2D_ANIMATED_PROPERTY_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Logger.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A class for smooth value interpolation.                                    //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
template <typename T>
class SWIFT_DLL AnimatedProperty : public Property<T> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  // ------------------------------------------------------------------- signals
  Signal<> const& on_finish() const { return on_finish_; }

  // --------------------------------------------------------------------- enums
  enum Direction {
	  DIR_IN, DIR_OUT, DIR_IN_OUT, DIR_OUT_IN, DIR_LINEAR
  };

  enum Loop {
    NONE, REPEAT, TOGGLE
  };

  // ----------------------------------------------------- contruction interface
  AnimatedProperty()
    : Property<T>()
    , direction_(DIR_IN_OUT)
    , start_()
    , end_()
    , state_(0.0)
    , duration_(0)
    , exp_(0.0)
    , delay_(0.0) {}

  AnimatedProperty(T const& value)
    : Property<T>(value)
    , direction_(DIR_IN_OUT)
    , start_(value)
    , end_(value)
    , state_(-1.0)
    , duration_(0)
    , exp_(0.0)
    , delay_(0.0) {}

  AnimatedProperty(T const& start, T const& end, double duration = 1.0,
	  Direction direction = DIR_IN_OUT, Loop loop = NONE,
                   double exponent = 0.0)
    : Property<T>(start)
    , direction_(direction)
    , loop_(loop)
    , start_(start)
    , end_(end)
    , state_(0.0)
    , duration_(duration)
    , exp_(exponent)
    , delay_(0.0) {}

  // ------------------------------------------------------------ public methods
  void set(T const& value, double duration, double delay = 0.0) {
    start_ = this->get();
    end_ = value;
    duration_ = duration;
    state_ = 0.0;
    delay_ = delay;
  }

  void set(T const& value) {
    start_ = this->get();
    end_ = value;
    duration_ = 0.0;
    state_ = -1.0;
    delay_ = 0.0;
    Property<T>::set(value);
  }

  void set_direction(Direction dir) {
    direction_ = dir;
  }

  void set_loop(Loop loop) {
    loop_ = loop;
  }

  void update(double time) {
    if (state_ < 1 && state_ >= 0.0) {
      if (delay_ > 0) {
        delay_ -= time;
      } else {

        state_ += time / duration_;

        switch (direction_) {
          case DIR_LINEAR:
            Property<T>::set(updateLinear(state_, start_, end_));
            break;
          case DIR_IN:
            Property<T>::set(updateEaseIn(state_, start_, end_));
            return;
          case DIR_OUT:
            Property<T>::set(updateEaseOut(state_, start_, end_));
            return;
          case DIR_IN_OUT:
            Property<T>::set(updateEaseInOut(state_, start_, end_));
            return;
          case DIR_OUT_IN:
            Property<T>::set(updateEaseOutIn(state_, start_, end_));
            return;
        }
      }
    } else if (state_ != -1.0) {
      Property<T>::set(end_);
      state_ = -1.0;
      on_finish_.emit();

      if (loop_ == REPEAT) {
        Property<T>::set(start_);
        set(end_, duration_);

      } else if (loop_ == TOGGLE) {
        set(start_, duration_);
      }
    }
  }

  inline T const& start() const { return start_; }
  inline T const& end()   const { return end_; }

  virtual AnimatedProperty<T>& operator=(T const& rhs) {
    set(rhs);
    return *this;
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  T updateLinear(T const& t, T const& s, double e) {
    return (s + t * (e-s));
  }

  T updateEaseIn(T const& t, T const& s, double e) {
    return (s + (t * t * ((exp_+1) * t - exp_)) * (e-s));
  }

  T updateEaseOut(T const& t, T const& s, double e) {
    return (s + ((t-1) * (t-1) * ((exp_+1) * (t-1) + exp_) + 1) * (e-s));
  }

  T updateEaseInOut(T const& t, T const& s, double e) {
    if (state_ < 0.5f) return updateEaseIn(t*2, s, e - (e-s) * 0.5f);
    else               return updateEaseOut(t*2 - 1, s + (e-s) * 0.5f, e);
  }

  T updateEaseOutIn(T const& t, T const& s, double e) {
    if (state_ < 0.5f) return updateEaseOut(t*2, s, e - (e-s) * 0.5f);
    else               return updateEaseIn(t*2 - 1, s + (e-s) * 0.5f, e);
  }

  Signal<> on_finish_;
  Direction direction_;
  Loop loop_;

  T start_, end_;
  double state_;
  double duration_, exp_, delay_;
};

}

#endif  //SWIFT2D_ANIMATED_PROPERTY_HPP
