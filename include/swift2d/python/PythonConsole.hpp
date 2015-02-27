////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PYTHON_CONSOLE_HPP
#define SWIFT2D_PYTHON_CONSOLE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/properties.hpp>

#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

// forward declares ------------------------------------------------------------

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL PythonConsole : public Singleton<PythonConsole> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------- signals
  Signal<std::string> on_result;

  // ---------------------------------------------------------------- properties

  // ------------------------------------------------------------ public methods
  void evaluate(std::string const& command);

  friend class Singleton<PythonConsole>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  PythonConsole();
  ~PythonConsole();

  class PythonStdIoRedirect {
    public: void write(std::string const& str);
  };

  boost::python::object main_module_;
  boost::python::object main_namespace_;
  boost::circular_buffer<std::string> output_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_PYTHON_CONSOLE_HPP
