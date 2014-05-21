////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_LOGICAL_PROPERTY_HPP
#define SWIFT2D_EVENTS_LOGICAL_PROPERTY_HPP

#include <swift2d/properties/Property.hpp>

namespace swift {

template <typename T> class LogicalProperty : public virtual Property<T> {

 public:
  LogicalProperty() : Property<T>() {}

  LogicalProperty(T const& value) : Property<T>(value) {}

  LogicalProperty(LogicalProperty<T> const& to_copy) : Property<T>(to_copy) {}

  virtual ~LogicalProperty() {}

  bool operator&&(LogicalProperty<T> const& rhs) const {
    return Property<T>::get() && rhs.get();
  }

  bool operator&&(T const& rhs) const { return Property<T>::get() && rhs; }

  bool operator||(LogicalProperty<T> const& rhs) const {
    return Property<T>::get() || rhs.get();
  }

  bool operator||(T const& rhs) const { return Property<T>::get() || rhs; }

  bool operator&(LogicalProperty<T> const& rhs) const {
    return Property<T>::get() & rhs.get();
  }

  bool operator&(T const& rhs) const { return Property<T>::get() & rhs; }

  bool operator|(LogicalProperty<T> const& rhs) const {
    return Property<T>::get() | rhs.get();
  }

  bool operator|(T const& rhs) const { return Property<T>::get() | rhs; }

  LogicalProperty<T> operator!() const {
    return LogicalProperty<T>(!Property<T>::get());
  }
};

}

#endif /* SWIFT2D_EVENTS_LOGICAL_PROPERTY_HPP */