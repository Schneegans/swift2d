////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_SORTABLE_PROPERTY_HPP
#define SWIFT2D_EVENTS_SORTABLE_PROPERTY_HPP

#include <swift2d/properties/Property.hpp>

namespace swift {

template <typename T> class SortableProperty : public virtual Property<T> {

 public:
  SortableProperty() : Property<T>() {}

  SortableProperty(T const& value) : Property<T>(value) {}

  SortableProperty(SortableProperty<T> const& to_copy) : Property<T>(to_copy) {}

  virtual ~SortableProperty() {}

  bool operator>(SortableProperty<T> const& rhs) const {
    return Property<T>::get() > rhs.get();
  }

  bool operator>(T const& rhs) const { return Property<T>::get() > rhs; }

  bool operator>=(SortableProperty<T> const& rhs) const {
    return Property<T>::get() >= rhs.get();
  }

  bool operator>=(T const& rhs) const { return Property<T>::get() >= rhs; }

  bool operator<(SortableProperty<T> const& rhs) const {
    return Property<T>::get() < rhs.get();
  }

  bool operator<(T const& rhs) const { return Property<T>::get() < rhs; }

  bool operator<=(SortableProperty<T> const& rhs) const {
    return Property<T>::get() <= rhs.get();
  }

  bool operator<=(T const& rhs) const { return Property<T>::get() <= rhs; }

};

}

#endif /* SWIFT2D_EVENTS_SORTABLE_PROPERTY_HPP */
