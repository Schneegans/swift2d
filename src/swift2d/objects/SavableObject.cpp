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
  visitor.write_to_file(path);
}

////////////////////////////////////////////////////////////////////////////////

std::string SavableObject::save_to_buffer() {
  SavableObjectVisitor visitor(get_type_name());
  accept(visitor);
  return visitor.write_to_buffer();
}

////////////////////////////////////////////////////////////////////////////////

void SavableObject::load_from_file(std::string const& path) {
  SavableObjectVisitor visitor;
  visitor.read_from_file(path, this);
}

////////////////////////////////////////////////////////////////////////////////

void SavableObject::load_from_buffer(std::string const& buffer) {
  SavableObjectVisitor visitor;
  visitor.read_from_buffer(buffer, this);
}

////////////////////////////////////////////////////////////////////////////////

SavableObjectPtr SavableObject::create_from_file(std::string const& path) {
  SavableObjectVisitor visitor;
  visitor.read_from_file(path);
  return visitor.to_object();
}

////////////////////////////////////////////////////////////////////////////////

SavableObjectPtr SavableObject::create_from_buffer(std::string const& buffer) {
  SavableObjectVisitor visitor;
  visitor.read_from_buffer(buffer);
  return visitor.to_object();
}

////////////////////////////////////////////////////////////////////////////////

}
