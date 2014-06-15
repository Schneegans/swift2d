////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/swift2d.hpp>

class Player: public swift::NetworkObject {
 public:

  Player()
    : name_("Unnamed Player")
    , id_(0) {

    swift::Network::instance()->distribute_object(this);
    // distribute_member(&name_);
    // distribute_member(&id_);
    distribute_member(&pos_);
  }

  RakNet::RakString get_name() const {
    return RakNet::RakString("Player");
  };

  static void init() {
    swift::Network::instance()->register_type<Player>();
  }

 // private:
  std::string       name_;
  int               id_;
  swift::math::vec2 pos_;
};

// main ------------------------------------------------------------------------
int main(int argc, char** argv) {

  // initialize Swift2D
  swift::Application app(argc, argv);
  swift::Network::instance()->connect("myTestGame");

  auto window = swift::Window::create();
  window->Open = true;

  Player::init();

  Player player;
  player.pos_ = swift::math::vec2(42, 42);

  auto ticker(swift::Ticker::create(1.0 / 60.0));
  ticker->on_tick.connect([&]() {
    window->process_input();
    swift::Network::instance()->update();
  });

  ticker->start();

  window->on_close.connect([&](){
    app.stop();
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (action != 1) {
      switch(key) {
        case swift::Key::ESCAPE:
          app.stop();
          break;
        case swift::Key::SPACE:
          player.name_ = "Jim";
          player.id_ = 2.f;
          player.pos_ = swift::math::vec2(1, 2);
          break;
      }
    }
  });

  app.start();

  swift::Network::instance()->disconnect();

  return 0;
}

