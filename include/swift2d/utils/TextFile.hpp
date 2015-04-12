////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TEXTFILE_HPP
#define SWIFT2D_TEXTFILE_HPP

#include <string>

////////////////////////////////////////////////////////////////////////////////
// This class is used to read text files.                                     //
////////////////////////////////////////////////////////////////////////////////

namespace swift {

class TextFile {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface

  // This constructs a TextFile without a file.
  TextFile();

  // This constructs a TextFile with a file.
  TextFile(std::string const& file_name);

  // ------------------------------------------------------------ public methods

  // Returns if the given file is valid.
  bool is_valid() const;

  // Returns the given file's content.
  std::string const& get_content() const;

  // Sets the given file's content.
  void set_content(std::string const& content);

  // Saves the file
  bool save() const;

  // Deletes the file from the file system
  void remove();

  // Returns the given file's name.
  std::string const& get_file_name() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::string         file_name_;
  mutable std::string content_;
  mutable bool        is_loaded_;

};

}

#endif  // SWIFT2D_TEXTFILE_HPP
