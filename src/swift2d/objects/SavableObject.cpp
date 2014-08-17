////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/objects/SavableObject.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void SavableObject::save_to_file(std::string const& path) {
  SavableObjectVisitor visitor(get_type_name());
  accept(visitor);
  visitor.write_json(path);
}

////////////////////////////////////////////////////////////////////////////////

SavableObjectPtr SavableObject::create_from_file(std::string const& path) {
  SavableObjectVisitor visitor;
  visitor.read_json(path);
  return visitor.to_object();
}

////////////////////////////////////////////////////////////////////////////////

}
