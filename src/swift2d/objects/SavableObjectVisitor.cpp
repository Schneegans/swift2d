////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/objects/SavableObjectVisitor.hpp>

#include <swift2d/objects/SavableObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SavableObjectVisitor::SavableObjectVisitor()
  : loaded_object_(nullptr)
  , loaded_object_raw_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

SavableObjectVisitor::SavableObjectVisitor(std::string const& name)
  : loaded_object_(nullptr)
  , loaded_object_raw_(nullptr) {
  json_.put("Type", name);
}

////////////////////////////////////////////////////////////////////////////////

void SavableObjectVisitor::write_to_file(std::string const& path) {
  boost::property_tree::write_json(path, json_);
}

////////////////////////////////////////////////////////////////////////////////

std::string SavableObjectVisitor::write_to_buffer() {
  std::stringstream str;
  boost::property_tree::write_json(str, json_);
  return str.str();
}

////////////////////////////////////////////////////////////////////////////////

void SavableObjectVisitor::read_from_file(std::string const& path) {
  try {
    boost::property_tree::read_json(path, json_);

    loaded_object_ = std::dynamic_pointer_cast<SavableObject>(
      Object::create(json_.get<std::string>("Type"))
    );

    loaded_object_raw_ = loaded_object_.get();

    loaded_object_->accept(*this);
    loaded_object_->on_loaded();
  } catch(...) {}
}

////////////////////////////////////////////////////////////////////////////////

void SavableObjectVisitor::read_from_file(std::string const& path, SavableObject* target) {
  try {
    boost::property_tree::read_json(path, json_);

    if (json_.get<std::string>("Type") == target->get_type_name()) {
      loaded_object_raw_ = target;
      target->accept(*this);
      target->on_loaded();
    }

  } catch(...) {}
}

////////////////////////////////////////////////////////////////////////////////

void SavableObjectVisitor::read_from_buffer(std::string const& buffer) {
  try {
    std::stringstream str(buffer);
    boost::property_tree::read_json(str, json_);

    loaded_object_ = std::dynamic_pointer_cast<SavableObject>(
      Object::create(json_.get<std::string>("Type"))
    );

    loaded_object_raw_ = loaded_object_.get();

    loaded_object_->accept(*this);
    loaded_object_->on_loaded();
  } catch(...) {}
}

////////////////////////////////////////////////////////////////////////////////

void SavableObjectVisitor::read_from_buffer(std::string const& buffer, SavableObject* target) {
  try {
    std::stringstream str(buffer);
    boost::property_tree::read_json(str, json_);

    if (json_.get<std::string>("Type") == target->get_type_name()) {
      loaded_object_raw_ = target;
      target->accept(*this);
      target->on_loaded();
    }

  } catch(...) {}
}

////////////////////////////////////////////////////////////////////////////////

void SavableObjectVisitor::read_json(boost::property_tree::ptree const& json) {
  json_ = json;

  loaded_object_ = std::dynamic_pointer_cast<SavableObject>(
    Object::create(json_.get<std::string>("Type"))
  );

  loaded_object_raw_ = loaded_object_.get();

  loaded_object_->accept(*this);
  loaded_object_->on_loaded();
}

////////////////////////////////////////////////////////////////////////////////

boost::property_tree::ptree SavableObjectVisitor::to_json() {
  return json_;
}

////////////////////////////////////////////////////////////////////////////////

SavableObjectPtr SavableObjectVisitor::to_object() {
  return loaded_object_;
}

////////////////////////////////////////////////////////////////////////////////

}
