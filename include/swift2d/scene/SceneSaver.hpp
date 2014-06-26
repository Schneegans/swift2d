////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SCENE_SAVER_HPP
#define SWIFT2D_SCENE_SAVER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/network/SerializableReference.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SceneSaver {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  void save(std::string const& name, SerializableReference const& value);

  boost::property_tree::ptree to_json(std::string const& type_name);

 private:
  std::vector<std::pair<std::string, SerializableReference>> saved_members_;
};

}

#endif  // SWIFT2D_SCENE_SAVER_HPP
