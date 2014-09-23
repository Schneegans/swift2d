////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/swift2d.hpp>
#include "SpaceScene.hpp"
#include "Player.hpp"

#include <steam/steam_api.h>

using namespace swift;

// main ------------------------------------------------------------------------
int main(int argc, char** argv) {

  // initialize Swift2D
  Application::get().init(argc, argv);

  if (!Steam::get().init()) {
    Application::get().clean_up();
    return 1;
  }

  Steam::get().update_room_list();

  Steam::get().on_updated_room_list.connect(
    [](std::unordered_map<uint64_t, Steam::RoomData> const& rooms) {
      for (auto const& room : rooms) {
        if (room.second.name == "ichmachemaleinfachnureinenraumauf") {
          Steam::get().join_room(room.first);
          return;
        }
      }

      Steam::get().create_room("ichmachemaleinfachnureinenraumauf");
  });

  Steam::get().on_message.connect(
    [&](Steam::MessageType type, uint64_t user_id, std::string const& join_message) {

      auto self      (Steam::get().get_user_id());
      bool self_join (type == Steam::JOIN && user_id == self);
      bool owner     (Steam::get().get_room_owner() == self);

      if (self_join) {
        if (owner) {
          std::cout << "Im lobby host." << std::endl;
          Steam::get().set_room_data("host", std::to_string(Network::get().get_own_id()));
        } else {
          auto host = Steam::get().get_room_data("host");
          std::cout << "Current lobby host: " << host << std::endl;
          Network::get().connect(std::from_string<uint64_t>(host));
        }
      }
  });

  // scene ---------------------------------------------------------------------
  auto scene = SpaceScene::create();
  SceneManager::get().current()->Camera->Size = math::vec2(2.f, 2.f);

  // player --------------------------------------------------------------------
  Player::init();
  Player player(true);

  // main loop -----------------------------------------------------------------
  Application::get().on_frame.connect([&](double frame_time) {
    Steam::get().update();
    Network::get().update();
    scene->update(frame_time);
  });

  auto window = WindowManager::get().current();
  window->on_close.connect([&](){
    Application::get().stop();
  });

  Application::get().start();
  Application::get().clean_up();

  return 0;
}
