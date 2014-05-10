////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ADD_PROPERTY_HPP
#define SWIFT2D_ADD_PROPERTY_HPP

#define SWIFT2D_ADD_PROPERTY(CONTAINER, TYPE, NAME, VALUE) \
TYPE NAME = static_cast<TYPE::value_type>(VALUE);             \
CONTAINER & set_##NAME(TYPE::value_type const& value) {        \
  NAME = value;                                               \
  return *this;                                               \
}

#define SWIFT2D_ADD_VALUE(CONTAINER, TYPE, NAME, VALUE)    \
TYPE NAME = VALUE;                                            \
CONTAINER & set_##NAME(TYPE const& value) {                    \
  NAME = value;                                               \
  return *this;                                               \
}



#define SWIFT2D_ADD_CONFIG_PROPERTY(TYPE, NAME, VALUE)     \
TYPE NAME = static_cast<TYPE::value_type>(VALUE);             \
Config& set_##NAME(TYPE::value_type const& value) {           \
  NAME = value;                                               \
  return *this;                                               \
}

#define SWIFT2D_ADD_CONFIG_VALUE(TYPE, NAME, VALUE)        \
TYPE NAME = VALUE;                                            \
Config& set_##NAME(TYPE const& value) {                       \
  NAME = value;                                               \
  return *this;                                               \
}


#endif  // SWIFT2D_ADD_PROPERTY_HPP
