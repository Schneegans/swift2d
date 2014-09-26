////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/events/MainLoop.hpp>
#include <iostream>

// boost asio implmentation
#include <boost/asio.hpp>
#include <boost/asio/impl/src.hpp>

namespace swift {

namespace {
  boost::asio::io_service io_service;
}

////////////////////////////////////////////////////////////////////////////////

void MainLoop::start() {
  io_service.run();
}

////////////////////////////////////////////////////////////////////////////////

void MainLoop::stop() {
  io_service.stop();
}

////////////////////////////////////////////////////////////////////////////////

boost::asio::io_service& MainLoop::get_io_service() { 
  return io_service; 
}

////////////////////////////////////////////////////////////////////////////////

}
