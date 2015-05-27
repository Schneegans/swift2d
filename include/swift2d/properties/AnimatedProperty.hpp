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

#ifndef SWIFT2D_ANIMATED_PROPERTY_HPP
#define SWIFT2D_ANIMATED_PROPERTY_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Logger.hpp>
#include <swift2d/utils/stl_helpers.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/objects/SavableObject.hpp>
#include <swift2d/objects/SavableObjectVisitor.hpp>

namespace swift {

class SavableObjectVisitor;

////////////////////////////////////////////////////////////////////////////////
// A class for smooth value interpolation.                                    //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
template <typename T>
class AnimatedProperty : public Property<T>,
                         public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  // --------------------------------------------------------------------- enums
  enum Direction {
	  DIR_IN, DIR_OUT, DIR_IN_OUT, DIR_OUT_IN, DIR_LINEAR
  };

  enum Loop {
    NONE, REPEAT, TOGGLE
  };

  int       direction;
  int       loop;
  double    duration;
  double    exponent;
  double    delay;

  // ------------------------------------------------------------------- signals
  Signal<> on_finish;

  // ----------------------------------------------------- contruction interface
  AnimatedProperty()
    : Property<T>()
    , direction(DIR_IN_OUT)
    , start_()
    , end_()
    , state_(0.0)
    , duration(0)
    , exponent(0.0)
    , delay(0.0) {}

  AnimatedProperty(T val)
    : Property<T>(val)
    , direction(DIR_IN_OUT)
    , start_(val)
    , end_(val)
    , state_(0.0)
    , duration(0)
    , exponent(0.0)
    , delay(0.0) {}

  AnimatedProperty(T start, T end, double dur = 1.0,
	  Direction dir = DIR_IN_OUT, Loop looping = NONE,
                   double exp = 0.0)
    : Property<T>(start)
    , direction(dir)
    , loop(looping)
    , start_(start)
    , end_(end)
    , state_(0.0)
    , duration(dur)
    , exponent(exp)
    , delay(0.0) {}

  // ------------------------------------------------------------ public methods
  void set(T value, double dur, double del = 0.0) {
    start_ = this->get();
    end_ = value;
    duration = dur;
    state_ = 0.0;
    delay = del;
  }

  void set(T value) {
    start_ = value;
    end_ = value;
    duration = 0.0;
    state_ = -1.0;
    delay = 0.0;
    Property<T>::set(value);
  }

  void update(double time) {

    if (duration == 0.0) {
      state_ = 1.0;
    }

    if (state_ < 1 && state_ >= 0.0) {
      if (delay > 0) {
        delay -= time;
      } else {

        state_ += time / duration;

        switch (direction) {
          case DIR_LINEAR:
            Property<T>::set(updateLinear(state_, start_, end_));
            break;
          case DIR_IN:
            Property<T>::set(updateEaseIn(state_, start_, end_));
            break;
          case DIR_OUT:
            Property<T>::set(updateEaseOut(state_, start_, end_));
            break;
          case DIR_IN_OUT:
            Property<T>::set(updateEaseInOut(state_, start_, end_));
            break;
          case DIR_OUT_IN:
            Property<T>::set(updateEaseOutIn(state_, start_, end_));
            break;
        }
      }
    } else if (state_ != -1.0) {
      Property<T>::set(end_);
      state_ = -1.0;
      on_finish.emit();

      if (loop == REPEAT) {
        Property<T>::set(start_);
        set(end_, duration);

      } else if (loop == TOGGLE) {
        set(start_, duration);
      }
    }
  }

  inline void     restart()     { Property<T>::set(start_); set(end_, duration); }
  inline T const& start() const { return start_; }
  inline T const& end()   const { return end_; }

  virtual AnimatedProperty<T>& operator=(T const& rhs) {
    set(rhs);
    return *this;
  }

  virtual void accept(SavableObjectVisitor& visitor) {
    visitor.add_member("Direction", direction);
    visitor.add_member("Loop",      loop);
    visitor.add_member("Duration",  duration);
    visitor.add_member("Exponent",  exponent);
    visitor.add_member("Delay",     delay);
    visitor.add_member("Start",     start_);
    visitor.add_member("End",       end_);

    this->set_with_no_emit(start_);
  };

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 protected:
  T updateLinear(T const& t, T const& s, double e) {
    return clamp((T)(s + t * (e-s)), start_, end_);
  }

  T updateEaseIn(T const& t, T const& s, double e) {
    return s + (t * t * ((exponent+1) * t - exponent)) * (e-s);
  }

  T updateEaseOut(T const& t, T const& s, double e) {
    return s + ((t-1) * (t-1) * ((exponent+1) * (t-1) + exponent) + 1) * (e-s);
  }

  T updateEaseInOut(T const& t, T const& s, double e) {
    if (state_ < 0.5f) return updateEaseIn(t*2, s, e - (e-s) * 0.5f);
    else               return updateEaseOut(t*2 - 1, s + (e-s) * 0.5f, e);
  }

  T updateEaseOutIn(T const& t, T const& s, double e) {
    if (state_ < 0.5f) return updateEaseOut(t*2, s, e - (e-s) * 0.5f);
    else               return updateEaseIn(t*2 - 1, s + (e-s) * 0.5f, e);
  }

  T start_, end_;
  double state_;
};

class AnimatedFloat : public AnimatedProperty<float> {
 public:
  AnimatedFloat() : AnimatedProperty<float>(0.f) {}
  AnimatedFloat(float val) : AnimatedProperty<float>(val) {}
  AnimatedFloat(float const& start, float const& end, double duration = 1.0,
    Direction direction = DIR_IN_OUT, Loop loop = NONE, double exponent = 0.0)
    : AnimatedProperty<float>(start, end, duration, direction, loop, exponent) {}

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "AnimatedFloat"; }

  AnimatedFloat& operator=(AnimatedFloat const& other) {
    direction = other.direction;
    loop = other.loop;
    duration = other.duration;
    exponent = other.exponent;
    delay = other.delay;
    start_ = other.start_;
    end_ = other.end_;
    state_ = other.state_;

    Property<float>::set(other.get());

    return *this;
  }
};

class AnimatedDouble : public AnimatedProperty<double> {
 public:
  AnimatedDouble() : AnimatedProperty<double>(0.0) {}
  AnimatedDouble(double val) : AnimatedProperty<double>(val) {}
  AnimatedDouble(double const& start, double const& end, double duration = 1.0,
    Direction direction = DIR_IN_OUT, Loop loop = NONE, double exponent = 0.0)
    : AnimatedProperty<double>(start, end, duration, direction, loop, exponent) {}

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "AnimatedDouble"; }

  AnimatedDouble& operator=(AnimatedDouble const& other) {
    std::cout << "assignemnt animated double" << std::endl;
    direction = other.direction;
    loop = other.loop;
    duration = other.duration;
    exponent = other.exponent;
    delay = other.delay;
    start_ = other.start_;
    end_ = other.end_;
    state_ = other.state_;

    Property<double>::set(other.get());

    return *this;
  }
};

}

#endif  //SWIFT2D_ANIMATED_PROPERTY_HPP
