////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/audio/Sound.hpp>
#include <sndfile.h>
#include <vector>
#include <array>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void Sound::load_from_file(std::string const& file_name) {
  SF_INFO info;
  SNDFILE* file = sf_open(file_name.c_str(), SFM_READ, &info);

  auto tmp(sf_get_string(file, SF_STR_TITLE));
  if (tmp) title_  = std::string(tmp);
  tmp = sf_get_string(file, SF_STR_ARTIST);
  if (tmp) artist_ = std::string(tmp);
  tmp = sf_get_string(file, SF_STR_ALBUM);
  if (tmp) album_  = std::string(tmp);
  tmp = sf_get_string(file, SF_STR_DATE);
  if (tmp) year_   = std::string(tmp);

  int error(sf_error(file));
  if (error) {
    Logger::LOG_WARNING << "Error loading audio file \"" << file_name << "\": "
                        << sf_error_number(error) << std::endl;
  }

  std::vector<short> data(info.channels * info.frames);
  sf_readf_short(file, data.data(), info.frames);

  buffer_.Data(info.channels == 1 ? oalplus::DataFormat::Mono16 :
                                    oalplus::DataFormat::Stereo16,
               &data.front(), data.size() * sizeof(short),
               info.samplerate);

  sf_close(file);
}

////////////////////////////////////////////////////////////////////////////////

std::string const& Sound::get_title() {
  return title_;
}
std::string const& Sound::get_artist() {
  return artist_;
}
std::string const& Sound::get_album() {
  return album_;
}
std::string const& Sound::get_year() {
  return year_;
}

////////////////////////////////////////////////////////////////////////////////

}
