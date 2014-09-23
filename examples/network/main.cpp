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
  // Network::get().connect("TestGame");

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

  std::map<uint64_t, SNetSocket_t> users_;

  Steam::get().on_message.connect(
    [&](Steam::MessageType type, uint64_t user_id, std::string const& join_message) {
      if (user_id != Steam::get().get_user_id()) {
        std::cout << "Join " << join_message  << " " << user_id<< std::endl;
        users_[user_id] = SteamNetworking()->CreateP2PConnectionSocket(user_id, 0, 5.f, true);





      }
  });

  // scene ---------------------------------------------------------------------
  auto scene = SpaceScene::create();
  auto camera = SceneManager::get().current()->Camera;
  camera->Size = math::vec2(2.f, 2.f);

  // player --------------------------------------------------------------------
  Player::init();
  Player player(true);

  swift::Peer peer;

  // main loop -----------------------------------------------------------------
  Application::get().on_frame.connect([&](double frame_time) {
    Steam::get().update();

    uint32 size;

    for (auto user : users_) {

      if (SteamNetworking()->IsDataAvailableOnSocket(user.second, &size)) {
        std::string result(size, ' ');
        uint32 actual_size;
        CSteamID sender;
        SteamNetworking()->RetrieveDataFromSocket(user.second, &(*result.begin()), size, &actual_size);

        std::cout << "got " << result << std::endl;

        uint32 raw_remote_ip;
        uint16 raw_remote_port;
        std::string remote_ip;
        std::string remote_port;

        int status;

        if (SteamNetworking()->GetSocketInfo(user.second, &sender, &status, &raw_remote_ip, &raw_remote_port)) {
          std::string a = std::to_string((raw_remote_ip >> 24) & 255);
          std::string b = std::to_string((raw_remote_ip >> 16) & 255);
          std::string c = std::to_string((raw_remote_ip >> 8)  & 255);
          std::string d = std::to_string(raw_remote_ip         & 255);

          remote_ip = a + "." + b + "." + c + "." + d;
          remote_port = std::to_string(raw_remote_port);

          // if (state.m_bUsingRelay) {
          //   std::cout << "    Message has been relayed." << std::endl;
          // } else {
          //   std::cout << "    Message has not been relayed." << std::endl;
          // }
        }

        if (result == "request_connect") {
          std::string msg("confirm_connect");
          SteamNetworking()->SendDataOnSocket(user.second, &msg[0], msg.length(), k_EP2PSendReliable);

        } else if (result == "confirm_connect") {
          std::cout << "confirm!" << std::endl;

          struct sockaddr_in localAddress;
          socklen_t addressLength = sizeof(localAddress);
          getsockname(user.second, (struct sockaddr*)&localAddress, &addressLength);
          printf("local address: %s\n", inet_ntoa(localAddress.sin_addr));
          printf("local port: %d\n", (int) ntohs(localAddress.sin_port));

          std::cout << "remote address: " << remote_ip << std::endl;
          std::cout << "remote port: " << remote_port << std::endl;
        }
      }
    }

    // Network::get().update();
    scene->update(frame_time);
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
          for (auto user : users_) {
            std::string msg("request_connect");
            std::cout << "send" << std::endl;
            SteamNetworking()->SendDataOnSocket(user.second, &msg[0], msg.length(), k_EP2PSendReliable);
          }
          break;
      }
    }
  });

  Application::get().start();

  // Network::get().disconnect();

  Application::get().clean_up();

  return 0;
}
