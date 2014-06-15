////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ANY_VALUE_HPP
#define SWIFT2D_ANY_VALUE_HPP

// includes  -------------------------------------------------------------------
#include <raknet/src/VariableDeltaSerializer.h>

#include <boost/any.hpp>
#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SerializableReference {

  // forward declares ----------------------------------------------------------
  class Serializer;

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  SerializableReference(): serializer_(nullptr) {}

  template<class T> SerializableReference(T const& value)
    : serializer_(new SerializerImpl<T>())
    , value_(value) {}

  SerializableReference(SerializableReference const& a)
    : serializer_(a.serializer_ ? a.serializer_->clone() : nullptr)
    , value_(a.value_) {}

  template <class T>
  SerializableReference& operator=(T const& r) {
    SerializableReference(r).swap(*this);
    return *this;
  }

  ~SerializableReference() {
    delete serializer_;
  }

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* serilizer) {
    serializer_->serialize(ctx, serilizer, value_);
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                   RakNet::VariableDeltaSerializer* serilizer) {
    serializer_->deserialize(ctx, serilizer, value_);
  }

  void print() const {
    serializer_->print(value_);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Serializer* serializer_;
  boost::any  value_;

  // ---------------------------------------------------------------------------
  struct Serializer {
    virtual void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                   RakNet::VariableDeltaSerializer* s,
                   boost::any const& a) const = 0;

    virtual void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                   RakNet::VariableDeltaSerializer* s,
                   boost::any const& a) const = 0;

    virtual Serializer* clone() const = 0;
    virtual void print(boost::any const& a) const = 0;
  };


  // ---------------------------------------------------------------------------
  template <class T>
  struct SerializerImpl: Serializer {

    void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                   RakNet::VariableDeltaSerializer* s,
                   boost::any const& a) const {

      s->SerializeVariable(ctx, *boost::any_cast<T>(a));
    }

    void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                   RakNet::VariableDeltaSerializer* s,
                   boost::any const& a) const {

      std::cout << "Got Value!" << std::endl;
      T target(boost::any_cast<T>(a));
      if(s->DeserializeVariable(ctx, *target)) {
        // std::cout << "Value changed: " << *target << std::endl;
      }
    }

    Serializer* clone() const {
      return new SerializerImpl<T>();
    }

    void print(boost::any const& a) const {
      // std::cout << *boost::any_cast<T>(a) << std::endl;
    }
  };

  void swap(SerializableReference& r) {
    std::swap(serializer_, r.serializer_);
    std::swap(value_, r.value_);
  }

};

}

#endif  // SWIFT2D_ANY_VALUE_HPP
