////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/audio/Music.hpp>

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

std::wstring const& Music::get_title() {
  return title_;
}
std::wstring const& Music::get_artist() {
  return artist_;
}
std::wstring const& Music::get_album() {
  return album_;
}
std::wstring const& Music::get_year() {
  return year_;
}

////////////////////////////////////////////////////////////////////////////////

void Music::load_from_file(std::string const& file_name) {
  Sound::load_from_file(file_name);

  TagLib::FileRef ref(file_name.c_str());
  TagLib::Tag *tag(ref.tag());

  title_  = std::wstring(tag->title().toWString());
  artist_ = std::wstring(tag->artist().toWString());
  album_  = std::wstring(tag->album().toWString());
  year_   = std::to_wstring(tag->year());
}

////////////////////////////////////////////////////////////////////////////////

}
