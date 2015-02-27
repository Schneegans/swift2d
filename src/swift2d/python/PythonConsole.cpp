////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/python/PythonConsole.hpp>

#include <Python.h>
#include <iostream>

namespace bp = boost::python;

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void PythonConsole::PythonStdIoRedirect::write(std::string const& str) {
  PythonConsole::get().output_.push_back(str);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void PythonConsole::evaluate(std::string const& command) {
  // Skip empty lines
  if (command.length() == 0) {
    std::cout << "empty" << std::endl;
    return; // empty command
  }
  size_t firstNonSpacePos = command.find_first_not_of(" \t\r\n");
  if (firstNonSpacePos == std::string::npos) {
    std::cout << "blanks" << std::endl;
    return; // all blanks command
  }
  if (command[firstNonSpacePos] == '#') {
    std::cout << "comment" << std::endl;
    return; // comment line
  }

  try {
    // First compile the expression without running it.
    bp::object compiledCode(bp::handle<>(Py_CompileString(
        (command + "\n").c_str(),
        "<stdin>",
        Py_single_input)));
    if (! compiledCode.ptr()) {
      // command failed
      std::cout << "fail" << std::endl;
      return;
    }

    bp::object result(bp::handle<>( PyEval_EvalCode(
        compiledCode.ptr(),
        main_namespace_.ptr(),
        main_namespace_.ptr())));
  }
  catch( bp::error_already_set )
  {
    // Distinguish incomplete input from invalid input
    char *msg = NULL;
    PyObject *exc, *val, *obj, *trb;
    if (PyErr_ExceptionMatches(PyExc_SyntaxError))
    {
      PyErr_Fetch (&exc, &val, &trb);        /* clears exception! */
      if (PyArg_ParseTuple (val, "sO", &msg, &obj) &&
          !strcmp (msg, "unexpected EOF while parsing")) /* E_EOF */
      {
        Py_XDECREF (exc);
        Py_XDECREF (val);
        Py_XDECREF (trb);
        std::cout << "incomplete" << std::endl;
        return;
      }
      PyErr_Restore (exc, val, trb);
    }

    PyErr_Print();
  }

  if (output_.size() > 1) {
    std::string ret;
    std::stringstream ss;
    for(auto it = output_.begin(); it != output_.end(); ++it) {
      ss << *it;
    }
    ret = ss.str();
    on_result.emit(ret);
  }
  output_.clear();

}

////////////////////////////////////////////////////////////////////////////////

PythonConsole::PythonConsole() {
  Py_Initialize();

  main_module_ = bp::object((bp::handle<>(bp::borrowed(PyImport_AddModule("__main__")))));
  main_namespace_ = main_module_.attr("__dict__");

  output_.resize(100);

  main_namespace_["PythonStdIoRedirect"] = bp::class_<PythonStdIoRedirect>("PythonStdIoRedirect", bp::init<>())
    .def("write", &PythonStdIoRedirect::write);

  PythonStdIoRedirect python_stdio_redirector;
  boost::python::import("sys").attr("stderr") = python_stdio_redirector;
  boost::python::import("sys").attr("stdout") = python_stdio_redirector;
}

////////////////////////////////////////////////////////////////////////////////

PythonConsole::~PythonConsole() {
  Py_Finalize();
}

////////////////////////////////////////////////////////////////////////////////

}

