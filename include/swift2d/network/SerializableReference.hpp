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
#include <swift2d/properties.hpp>

#include <boost/any.hpp>
#include <iostream>

namespace swift {



// ---------------------------------------------------------------------------
struct Serializer {
  virtual void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const = 0;

  virtual void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const = 0;

  virtual Serializer* clone() const = 0;
};


// ---------------------------------------------------------------------------
template <typename T>
struct SerializerImpl: Serializer {

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    s->SerializeVariable(ctx, *boost::any_cast<T>(a));
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    T target(boost::any_cast<T>(a));
    s->DeserializeVariable(ctx, *target);
  }

  Serializer* clone() const {
    return new SerializerImpl<T>();
  }
};

// ---------------------------------------------------------------------------
template <typename T>
struct SerializerImpl<NumericProperty<T>*>: Serializer {

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    s->SerializeVariable(ctx, boost::any_cast<NumericProperty<T>*>(a)->get());
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    T val;
    if (s->DeserializeVariable(ctx, val)) {
      boost::any_cast<NumericProperty<T>*>(a)->set(val);
    }
  }

  Serializer* clone() const {
    return new SerializerImpl<NumericProperty<T>*>();
  }
};

// ---------------------------------------------------------------------------
template <typename T>
struct SerializerImpl<LogicalProperty<T>*>: Serializer {

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    s->SerializeVariable(ctx, boost::any_cast<LogicalProperty<T>*>(a)->get());
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    T val;
    if (s->DeserializeVariable(ctx, val)) {
      boost::any_cast<LogicalProperty<T>*>(a)->set(val);
    }
  }

  Serializer* clone() const {
    return new SerializerImpl<NumericProperty<T>*>();
  }
};

// ---------------------------------------------------------------------------
template <typename T>
struct SerializerImpl<AnimatedProperty<T>*>: Serializer {

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    s->SerializeVariable(ctx, boost::any_cast<AnimatedProperty<T>*>(a)->get());
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    T val;
    if (s->DeserializeVariable(ctx, val)) {
      boost::any_cast<AnimatedProperty<T>*>(a)->set(val);
    }
  }

  Serializer* clone() const {
    return new SerializerImpl<AnimatedProperty<T>*>();
  }
};



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SerializableReference {

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

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  void swap(SerializableReference& r) {
    std::swap(serializer_, r.serializer_);
    std::swap(value_, r.value_);
  }

  Serializer* serializer_;
  boost::any  value_;
};

}

#endif  // SWIFT2D_ANY_VALUE_HPP
