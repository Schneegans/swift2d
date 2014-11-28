////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SAVABLE_OBJECT_VISITOR_HPP
#define SWIFT2D_SAVABLE_OBJECT_VISITOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/SavableObject.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SavableObject;

// -----------------------------------------------------------------------------
class SWIFT_DLL SavableObjectVisitor {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  SavableObjectVisitor();
  SavableObjectVisitor(std::string const& name);

  void write_json(std::string const& path);
  void read_json(std::string const& path);
  void read_json(std::string const& path, SavableObject* target);
  void read_json(boost::property_tree::ptree const& json);

  boost::property_tree::ptree to_json();
  SavableObjectPtr to_object();

  template <class T>
  void add_object(std::string const& name, T& target) {
    if (loaded_object_raw_) {
      auto child(json_.get_child_optional(name));

      if (child) {
        SavableObjectVisitor sub_visitor;
        sub_visitor.read_json(child.get());
        target = *std::static_pointer_cast<T>(sub_visitor.to_object());
      }

    } else {
      SavableObjectVisitor visitor(target.get_type_name());
      target.accept(visitor);
      json_.push_back(std::make_pair(name, visitor.to_json()));
    }
  }

  template <class T>
  void add_object_ptr(std::string const& name, T& target) {
    if (loaded_object_raw_) {
      auto child(json_.get_child_optional(name));

      if (child) {
        SavableObjectVisitor sub_visitor;
        sub_visitor.read_json(child.get());
        target = std::static_pointer_cast<typename T::element_type>(sub_visitor.to_object());
      }

    } else {
      SavableObjectVisitor visitor(target->get_type_name());
      target->accept(visitor);
      json_.push_back(std::make_pair(name, visitor.to_json()));
    }
  }

  template <class T>
  void add_object_property(std::string const& name, T& target) {
    if (loaded_object_raw_) {
      auto child(json_.get_child_optional(name));

      if (child) {
        SavableObjectVisitor sub_visitor;
        sub_visitor.read_json(child.get());
        target.set(std::dynamic_pointer_cast<typename T::value_type::element_type>(sub_visitor.to_object()));
      }

    } else if (target()) {
      SavableObjectVisitor visitor(target()->get_type_name());
      target()->accept(visitor);
      json_.push_back(std::make_pair(name, visitor.to_json()));
    }
  }

  template <class T>
  void add_member(std::string const& name, T& value) {
    if (loaded_object_raw_) {
      auto value_tmp = json_.get_optional<T>(name);
      if (value_tmp) {
        value = value_tmp.get();
      }
    } else {
      json_.put(name, value);
    }
  }

  template <class T>
  void add_array(std::string const& name, T& container) {
    if (loaded_object_raw_) {
      auto array(json_.get_child_optional(name));
      if (array) {
        for (auto const& elem : array.get()) {
          SavableObjectVisitor sub_visitor;
          sub_visitor.read_json(elem.second);
          std::insert_iterator<T> inserter(container, container.end());
          inserter = *std::static_pointer_cast<typename T::value_type>(sub_visitor.to_object());
        }
      }
    } else {
      if (container.size() > 0) {
        boost::property_tree::ptree targets;
        for (auto& elem: container) {
          SavableObjectVisitor visitor(elem.get_type_name());
          elem.accept(visitor);
          targets.push_back(std::make_pair("", visitor.to_json()));
        }
        json_.add_child(name, targets);
      }
    }
  }

  template <class T>
  void add_ptr_array(std::string const& name, T& container) {
    if (loaded_object_raw_) {
      auto array(json_.get_child_optional(name));
      if (array) {
        for (auto const& elem : array.get()) {
          SavableObjectVisitor sub_visitor;
          sub_visitor.read_json(elem.second);
          std::insert_iterator<T> inserter(container, container.end());
          inserter = std::static_pointer_cast<typename T::value_type::element_type>(sub_visitor.to_object());
        }
      }
    } else {
      if (container.size() > 0) {
        boost::property_tree::ptree targets;
        for (auto& elem: container) {
          SavableObjectVisitor visitor(elem->get_type_name());
          elem->accept(visitor);
          targets.push_back(std::make_pair("", visitor.to_json()));
        }
        json_.add_child(name, targets);
      }
    }
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  boost::property_tree::ptree json_;
  SavableObjectPtr            loaded_object_;
  SavableObject*              loaded_object_raw_;
};

}

#endif  // SWIFT2D_SAVABLE_OBJECT_VISITOR_HPP
