////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/properties/Property.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

template<> Property<double>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0.0) {}

template<> Property<float>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0.f) {}

template<> Property<math::int8>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> Property<math::int16>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> Property<math::int32>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> Property<math::int64>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> Property<math::uint8>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> Property<math::uint16>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> Property<math::uint32>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> Property<math::uint64>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> Property<bool>::Property()
  : connection_(nullptr), connection_id_(-1), value_(false) {}

////////////////////////////////////////////////////////////////////////////////

}
