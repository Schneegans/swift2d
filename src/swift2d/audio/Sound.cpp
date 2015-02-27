////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/audio/Sound.hpp>
#include <sndfile.h>
#include <vector>
#include <array>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Sound::Sound() {
  FileName.on_change().connect([this](std::string const& file) {
    load_from_file(file);
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void Sound::load_from_file(std::string const& file_name) {
  SF_INFO info;
  SNDFILE* file = sf_open(file_name.c_str(), SFM_READ, &info);

  int error(sf_error(file));
  if (error) {
    LOG_WARNING << "Error loading audio file \"" << file_name << "\": "
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

void Sound::load(oalplus::Source* source) {
  source->Buffer(buffer_);
}

////////////////////////////////////////////////////////////////////////////////

void Sound::unload(oalplus::Source* source) {
  source->DetachBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Sound::accept(SavableObjectVisitor& visitor) {
  AudioBuffer::accept(visitor);
  visitor.add_member("FileName", FileName);
}

////////////////////////////////////////////////////////////////////////////////

}
