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
        std::cout << "Join " << join_message << std::endl;
        users_[user_id] = SteamNetworking()->CreateP2PConnectionSocket(user_id, 0, 5.f, false);

        struct sockaddr_in localAddress;
        socklen_t addressLength = sizeof(localAddress);
        getsockname(users_[user_id], (struct sockaddr*)&localAddress, &addressLength);
        printf("local address: %s\n", inet_ntoa(localAddress.sin_addr));
        printf("local port: %d\n", (int) ntohs(localAddress.sin_port));
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
    if (SteamNetworking()->IsP2PPacketAvailable(&size)) {
      std::string result(size, ' ');
      uint32 actual_size;
      CSteamID sender;
      SteamNetworking()->ReadP2PPacket(&(*result.begin()), size, &actual_size, &sender);

      std::string remote_ip;
      std::string remote_port;

      P2PSessionState_t state;
      if (SteamNetworking()->GetP2PSessionState(sender, &state)) {
        std::string a = std::to_string((state.m_nRemoteIP >> 24) & 255);
        std::string b = std::to_string((state.m_nRemoteIP >> 16) & 255);
        std::string c = std::to_string((state.m_nRemoteIP >> 8)  & 255);
        std::string d = std::to_string(state.m_nRemoteIP         & 255);

        remote_ip = a + "." + b + "." + c + "." + d;
        remote_port = std::to_string(state.m_nRemotePort);

        std::cout << "    From " << Steam::get().get_user_name(sender.ConvertToUint64())
                  << " (" << remote_ip << ":" << remote_port << ")" << std::endl;

        if (state.m_bUsingRelay) {
          std::cout << "    Message has been relayed." << std::endl;
        } else {
          std::cout << "    Message has not been relayed." << std::endl;
        }
      }

      if (result == "request_connect") {
        std::string msg("confirm_connect");
        SteamNetworking()->SendDataOnSocket(users_[sender.ConvertToUint64()], &msg[0], msg.length(), k_EP2PSendReliable);

      } else if (result == "confirm_connect") {

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
