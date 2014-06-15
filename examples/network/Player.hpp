////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_HPP
#define PLAYER_HPP

// includes  -------------------------------------------------------------------
#include "Mover.hpp"

using namespace swift;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Player : public NetworkObject<Player> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Player(bool is_local = false)
    : NetworkObject<Player>("PlayerType") {

    auto scene = SceneManager::instance()->get_default();
    player_ = scene->add_object();
    player_->Transform = math::make_scale(0.1);

    if (is_local) {
      auto mover = std::make_shared<Mover>();
      player_->add(mover);
    }

    auto listener = player_->add<ListenerComponent>();
    listener->Volume = 1.0;

    auto ship = player_->add<SpriteComponent>();
    ship->Depth = 1.0f;
    ship->Material = MaterialDatabase::instance()->get("ship");

    auto light = player_->add<LightComponent>();
    light->Depth = 1.0f;
    light->Transform = math::make_scale(20);
    light->Material = MaterialDatabase::instance()->get("light");

    distribute_member(&player_->Transform);

    player_->Transform.on_change().connect([&](math::mat3 const& val){
      // std::cout << val << std::endl;
    });
  }

 ~Player() {
    auto scene = SceneManager::instance()->get_default();
    scene->remove(player_);
 }

 private:
  SceneObjectPtr player_;
};

#endif  // PLAYER_HPP
