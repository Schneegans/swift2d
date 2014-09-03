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
  Network::get().connect("TestGame");

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

  std::set<uint64_t> user_ids;

  Steam::get().on_message.connect(
    [&](Steam::MessageType type, uint64_t user_id, std::string const& join_message) {
      std::cout << join_message << std::endl;
      user_ids.insert(user_id);
      std::string huhu("huhu");
      SteamNetworking()->SendP2PPacket(user_id, huhu.c_str(), huhu.length(), k_EP2PSendReliable);
  });

  // scene ---------------------------------------------------------------------
  auto scene = SpaceScene::create();
  auto camera = scene->add<CameraComponent>();
  camera->Size = math::vec2(2.f, 2.f);

  // player --------------------------------------------------------------------
  Player::init();
  Player player(true);

  // rendering pipeline --------------------------------------------------------
  auto window = WindowManager::get().get_default();

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();
  Application::get().on_frame.connect([&]() {
    Steam::get().update();

    uint32 size;
    if (SteamNetworking()->IsP2PPacketAvailable(&size)) {
      std::string result(size, ' ');
      uint32 actual_size;
      CSteamID sender;
      SteamNetworking()->ReadP2PPacket(&(*result.begin()), size, &actual_size, &sender);
      std::cout << "Got message: " << result << " from " << Steam::get().get_user_name(sender.ConvertToUint64()) << std::endl;
    }

    double time(timer.get_elapsed());
    timer.reset();

    window->process_input();
    Network::get().update();
    scene->update(time);
    Application::get().display(scene, camera);
  });

  window->on_close.connect([&](){
    Application::get().stop();
  });

  window->on_key_press.connect([&](Key key, int scancode, int action, int mods) {
    if (action != 1) {
      switch(key) {
        case Key::ESCAPE:
          Application::get().stop();
          break;
        case Key::ENTER:
          for (auto user_id : user_ids) {
            std::string msg("Huhu, ich habe Enter gedrückt!");
            SteamNetworking()->SendP2PPacket(user_id, msg.c_str(), msg.length(), k_EP2PSendReliable);
          }
          break;
      }
    }
  });

  Application::get().start();
  Application::get().clean_up();

  return 0;
}
