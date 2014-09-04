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
  auto scene = SceneManager::get().current_scene();
  auto camera = SceneManager::get().current_camera();
  camera->Size = math::vec2(2.f, 2.f);

  // player --------------------------------------------------------------------
  Player::init();
  Player player(true);

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
      std::cout << std::endl << "Got message: " << result << std::endl;

      P2PSessionState_t state;
      if (SteamNetworking()->GetP2PSessionState(sender, &state)) {
        std::string a = std::to_string((state.m_nRemoteIP >> 24) & 255);
        std::string b = std::to_string((state.m_nRemoteIP >> 16) & 255);
        std::string c = std::to_string((state.m_nRemoteIP >> 8)  & 255);
        std::string d = std::to_string(state.m_nRemoteIP         & 255);

        std::string ip = a + "." + b + "." + c + "." + d;
        std::string port = std::to_string(state.m_nRemotePort);

        std::cout << "    From " << Steam::get().get_user_name(sender.ConvertToUint64())
                  << " (" << ip << ":" << port << ")" << std::endl;

        if (state.m_bUsingRelay) {
          std::cout << "    Message has been relayed." << std::endl;
        } else {
          std::cout << "    Message has not been relayed." << std::endl;
        }
      }
    }

    double time(timer.get_elapsed());
    timer.reset();

    Network::get().update();
    scene->update(time);
  });

  auto window = WindowManager::get().current();
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

  Network::get().disconnect();

  Application::get().clean_up();

  return 0;
}
