////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DATABASE_HPP
#define SWIFT2D_DATABASE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Logger.hpp>

#include <memory>
#include <string>
#include <set>
#include <unordered_map>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A database for accessing data. It can store any type of Data. The data is  //
// mapped on strings, which then can be used to access this data.             //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
template <typename T>
class Database {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // type definition
  typedef std::string key_type;
  typedef std::shared_ptr<T> mapped_type;

  // Adds a new entry to the data base.
  void add(key_type const& k, mapped_type const& date) {
    data_[k] = date;
    keys_.insert(k);
  }

  // Adds a new entry to the data base.
  void remove(key_type const& k) {
    data_.erase(k);
    keys_.erase(k);
  }

  // Check for existance of a key.
  bool is_supported(key_type const& k) const {
    return keys_.find(k) != keys_.end();
  }

  // Gets an entry from the Database
  mapped_type lookup(key_type const& k) {
    auto result(data_.find(k));
    if (result == data_.end()) {
      load(k);

      result = data_.find(k);
    }

    if (result == data_.end()) {
      LOG_WARNING << "There is no entry " << k << " in the database!" << std::endl;
      return std::shared_ptr<T>();
    }

    return result->second;
  }

  // Lists all supported keys.
  inline std::set<key_type> const& list_all() const { return keys_; }

  // This function gets called when a not-stored item is requested.
  virtual void load(key_type const& k) {};

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:
  std::unordered_map<key_type, mapped_type> data_;
  std::set<key_type>                        keys_;

};

}

#endif  // SWIFT2D_DATABASE_HPP
