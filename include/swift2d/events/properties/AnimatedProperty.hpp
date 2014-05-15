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
#include <swift2d/events/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A class for smooth value interpolation.                                    //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
template <typename T>
class AnimatedProperty: public NumericProperty<T> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  // ------------------------------------------------------------------- signals
  Signal<> const& on_finish() const { return on_finish_; }

  // --------------------------------------------------------------------- enums
  enum Direction {
    IN, OUT, IN_OUT, OUT_IN, LINEAR
  };

  // ----------------------------------------------------- contruction interface
  AnimatedProperty()
    : NumericProperty<T>()
    , direction_(IN_OUT)
    , start_()
    , end_()
    , state_(0.0)
    , duration_(0)
    , exp_(0.0) {}

  AnimatedProperty(T value)
    : NumericProperty<T>(value)
    , direction_(IN_OUT)
    , start_(value)
    , end_(value)
    , state_(-1.0)
    , duration_(0)
    , exp_(0.0) {}

  AnimatedProperty(T start, T end, double duration = 1.0,
                   Direction direction = IN_OUT, double exponent = 0.0)
    : NumericProperty<T>(start)
    , direction_(direction)
    , start_(start)
    , end_(end)
    , state_(0.0)
    , duration_(duration)
    , exp_(exponent) {}

  // ------------------------------------------------------------ public methods
  void set(T value, double duration) {
    start_ = this->get();
    end_ = value;
    duration_ = duration;
    state_ = 0.0;
  }

  /* virtual */ void set(T value) {
    start_ = this->get();
    end_ = value;
    duration_ = 0.0;
    state_ = -1.0;
    NumericProperty<T>::set(value);
  }

  void update(double time) {
    if (state_ < 1 && state_ >= 0.0) {
      state_ += time / duration_;

      switch (direction_) {
        case LINEAR:
          NumericProperty<T>::set(updateLinear(state_, start_, end_));
          break;
        case IN:
          NumericProperty<T>::set(updateEaseIn(state_, start_, end_));
          return;
        case OUT:
          NumericProperty<T>::set(updateEaseOut(state_, start_, end_));
          return;
        case IN_OUT:
          NumericProperty<T>::set(updateEaseInOut(state_, start_, end_));
          return;
        case OUT_IN:
          NumericProperty<T>::set(updateEaseOutIn(state_, start_, end_));
          return;
      }
    } else if (state_ != -1.0) {
      NumericProperty<T>::set(end_);
      state_ = -1.0;
      on_finish_.emit();
    }
  }

  inline T start() const { return start_; }
  inline T end()   const { return end_; }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  T updateLinear(T t, T s, double e) {
    return (s + t * (e-s));
  }

  T updateEaseIn(T t, T s, double e) {
    return (s + (t * t * ((exp_+1) * t - exp_)) * (e-s));
  }

  T updateEaseOut(T t, T s, double e) {
    return (s + ((t-1) * (t-1) * ((exp_+1) * (t-1) + exp_) + 1) * (e-s));
  }

  T updateEaseInOut(T t, T s, double e) {
    if (state_ < 0.5f) return updateEaseIn(t*2, s, e - (e-s) * 0.5f);
    else               return updateEaseOut(t*2 - 1, s + (e-s) * 0.5f, e);
  }

  T updateEaseOutIn(T t, T s, double e) {
    if (state_ < 0.5f) return updateEaseOut(t*2, s, e - (e-s) * 0.5f);
    else               return updateEaseIn(t*2 - 1, s + (e-s) * 0.5f, e);
  }

  Signal<> on_finish_;
  Direction direction_;

  T start_, end_;
  double state_;
  double duration_, exp_;
};

}

#endif  //SWIFT2D_ANIMATED_PROPERTY_HPP
