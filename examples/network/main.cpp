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
      }
    }
  });

  Application::get().start();
  Application::get().clean_up();

  return 0;
}
