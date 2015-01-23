////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_PROPERTY_HPP
#define SWIFT2D_EVENTS_PROPERTY_HPP

#include <swift2d/events/Signal.hpp>
#include <swift2d/utils/platform.hpp>

namespace swift {

template <typename T>
class Property {

 public:
  typedef T value_type;

  Property()
    : connection_(nullptr),
      connection_id_(-1) {}

  Property(T const& val)
    : value_(val),
      connection_(nullptr),
      connection_id_(-1) {}

  Property(Property<T> const& to_copy)
    : value_(to_copy.value_),
      connection_(nullptr),
      connection_id_(-1) {}

  virtual ~Property() {
    if (connection_) {
      connection_->on_change().disconnect(connection_id_);
    }
  }

  virtual Signal<T> const& on_change() const { return on_change_; }
  virtual Signal<T> const& before_change() const { return before_change_; }

  virtual void set(T const& value) {
    if (value != value_) {
      before_change_.emit(value_);
      value_ = value;
      on_change_.emit(value_);
    }
  }

  void set_with_no_emit(T const& value) {
    value_ = value;
  }

  virtual T const& get() const { return value_; }

  virtual void connect_from(Property<T> const& source) {
    if (connection_) {
      connection_->on_change().disconnect(connection_id_);
    }
    connection_ = &source;
    connection_id_ = source.on_change().connect([&](T const& value){
      set(value);
      return true;
    });
    set(source.get());
  }

  virtual void disconnect() {
    if (connection_) {
      connection_->on_change().disconnect(connection_id_);
      connection_id_ = -1;
      connection_ = nullptr;
    }
  }

  virtual void disconnect_auditors() {
    on_change_.disconnect_all();
    before_change_.disconnect_all();
  }

  virtual Property<T>& operator=(Property<T> const& rhs) {
    set(rhs.value_);
    return *this;
  }

  virtual Property<T>& operator=(T const& rhs) {
    set(rhs);
    return *this;
  }

  bool operator==(Property<T> const& rhs) const {
    return Property<T>::get() == rhs.get();
  }

  bool operator==(T const& rhs) const { return Property<T>::get() == rhs; }

  bool operator!=(Property<T> const& rhs) const {
    return Property<T>::get() != rhs.get();
  }

  bool operator!=(T const& rhs) const { return Property<T>::get() != rhs; }

  T const& operator()() const { return Property<T>::get(); }

 private:

  T value_;
  Signal<T> on_change_;
  Signal<T> before_change_;

  Property<T> const* connection_;
  int connection_id_;
};

template<typename T>
std::ostream& operator<<(std::ostream& out_stream, Property<T> const& val) {
  out_stream << val.get();
  return out_stream;
}

template<typename T>
std::istream& operator>>(std::istream& in_stream, Property<T>& val) {
  T tmp;
  in_stream >> tmp;
  val.set(tmp);
  return in_stream;
}

}

#endif /* SWIFT2D_EVENTS_PROPERTY_HPP */
