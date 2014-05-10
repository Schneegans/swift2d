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

namespace swift {

template <typename T>
class Property {

 public:
  typedef T value_type;

  Property()
    : connection_(nullptr),
      connection_id_(-1) {}

  Property(T const& value)
    : value_(value),
      connection_(nullptr),
      connection_id_(-1) {}

  Property(Property<T> const& to_copy)
    : value_(to_copy.value_),
      connection_(nullptr),
      connection_id_(-1) {}

  virtual ~Property() {}

  virtual Signal<T> const& on_change() const { return on_change_; }

  virtual void set(T const& value) {
    if (value != value_) {
      value_ = value;
      on_change_.emit(value_);
    }
  }

  virtual T const& get() const { return value_; }

  virtual void connect_from(Property<T> const& source) {
    if (connection_) {
      connection_->on_change().disconnect(connection_id_);
    }
    connection_ = &source;
    connection_id_ = source.on_change().connect([&](T const& value){
      set(value);
    });
  }

  virtual void disconnect() {
    if (connection_) {
      connection_->on_change().disconnect(connection_id_);
      connection_id_ = -1;
      connection_ = nullptr;
    }
  }

  virtual void disconnect_autitors() {
    on_change_.disconnect_all();
  }



  Property<T>& operator=(T const& rhs) {
    set(rhs);
    return *this;
  }

  Property<T>& operator=(Property<T> const& rhs) {
    set(rhs.value_);
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

  Property<T> const* connection_;
  int connection_id_;
};

}

#endif /* SWIFT2D_EVENTS_PROPERTY_HPP */
