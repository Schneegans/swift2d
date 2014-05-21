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

  std::vector<uint16_t> data;

  std::array<int16_t, 4096> read_buf;
  size_t read_size = 0;
  while((read_size = sf_read_short(file, read_buf.data(), read_buf.size())) != 0) {
    data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
  }

  buffer_.Data(info.channels == 1? oalplus::DataFormat::Mono16 : oalplus::DataFormat::Stereo16, &data.front(), data.size() * sizeof(uint16_t), info.samplerate);

  sf_close(file);
}

////////////////////////////////////////////////////////////////////////////////

}
