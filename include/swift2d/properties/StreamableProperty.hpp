////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_STREAMABLE_PROPERTY_HPP
#define SWIFT2D_EVENTS_STREAMABLE_PROPERTY_HPP

#include <swift2d/properties/Property.hpp>

#include <ostream>

namespace swift {

template <typename T> class StreamableProperty : public virtual Property<T> {

 public:
  StreamableProperty() : Property<T>() {}

  StreamableProperty(T const& value) : Property<T>(value) {}

  StreamableProperty(StreamableProperty<T> const& to_copy)
      : Property<T>(to_copy) {}

  virtual ~StreamableProperty() {}

};

template <typename T>
    std::ostream& operator<<(std::ostream& os,
                             StreamableProperty<T> const& rhs) {
  return (os << rhs.get());
}

}

#endif /* SWIFT2D_EVENTS_STREAMABLE_PROPERTY_HPP */
