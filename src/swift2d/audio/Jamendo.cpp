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

// includes  -------------------------------------------------------------------
#include <swift2d/audio/Jamendo.hpp>

#include <swift2d/utils/Logger.hpp>
#include <swift2d/application/Paths.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

namespace bpt = boost::property_tree;

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Jamendo::Jamendo() {

  http_.on_response.connect([&](std::string const& response) {
    try {
      bpt::ptree tree;
      std::istringstream is(response);
      bpt::read_xml(is, tree);

      auto track_list(tree.get_child("playlist.trackList"));

      std::for_each(std::begin(track_list), std::end(track_list), [this](bpt::ptree::value_type& kv) {
          album_.tracks.push_back({
            kv.second.get<std::string>("title"),
            kv.second.get<std::string>("creator"),
            kv.second.get<std::string>("title"),
            kv.second.get<std::string>("location")
          });
      });

      on_album_loaded.emit(album_);

    } catch(std::runtime_error const& e) {
      LOG_WARNING << "Failed to parse response: " << e.what() << std::endl;
    }
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void Jamendo::load_album(std::string const& id) {
  album_.tracks.clear();
  http_.get("api.jamendo.com/get2/stream/track/xspf/?album_id=" + id + "&order=numalbum_asc&n=all&streamencoding=ogg2",
            "api.jamendo.com", 80);
}

////////////////////////////////////////////////////////////////////////////////

void Jamendo::update() {
  http_.update();
}

////////////////////////////////////////////////////////////////////////////////

}
