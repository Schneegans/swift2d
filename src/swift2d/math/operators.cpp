////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/math/operators.hpp>

////////////////////////////////////////////////////////////////////////////////

namespace oglplus {

////////////////////////////////////////////////////////////////////////////////

#define INSTANCIATE_TYPE(T, C)                                                                        \
  template Vector<T, C>  operator*  <T, C>    (Vector<T, C> const& a, Vector<T, C> const& b);         \
  template Vector<T, C>  operator/  <T, C>    (Vector<T, C> const& a, Vector<T, C> const& b);         \
  template std::ostream& operator<< <T, C>    (std::ostream& out_stream, Vector<T, C> const& vec);    \
  template std::istream& operator>> <T, C>    (std::istream& in_stream, Vector<T, C>& vec);           \
  template std::ostream& operator<< <T, C, C> (std::ostream& out_stream, Matrix<T, C, C> const& mat); \
  template std::istream& operator>> <T, C, C> (std::istream& in_stream, Matrix<T, C, C>& mat);

INSTANCIATE_TYPE(float, 2)
INSTANCIATE_TYPE(float, 3)
INSTANCIATE_TYPE(float, 4)

INSTANCIATE_TYPE(int, 2)
INSTANCIATE_TYPE(int, 3)
INSTANCIATE_TYPE(int, 4)

INSTANCIATE_TYPE(unsigned, 2)
INSTANCIATE_TYPE(unsigned, 3)
INSTANCIATE_TYPE(unsigned, 4)

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

