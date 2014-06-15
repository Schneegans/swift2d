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

  static void init() {
    NetworkObject<Player>::init("PlayerType");
  }

  Player(bool is_local = false)
    : update_counter_(0) {

    auto scene = SceneManager::instance()->get_default();
    player_ = scene->add_object();
    player_->Transform = math::make_scale(0.1);

    MoveBehaviorPtr mover;
    if (is_local) mover = std::make_shared<Mover>();
    else          mover = MoveBehavior::create();
    player_->add(mover);

    auto listener = player_->add<ListenerComponent>();
    listener->Volume = 1.0;

    auto ship = player_->add<SpriteComponent>();
    ship->Depth = 1.0f;
    ship->Material = MaterialDatabase::instance()->get("ship");

    auto light = player_->add<LightComponent>();
    light->Depth = 1.0f;
    light->Transform = math::make_scale(20);
    light->Material = MaterialDatabase::instance()->get("light");

    if (is_local) {
      player_->Transform.on_change().connect([&](math::mat3 const& val){
        if (++update_counter_%100 == 0) {
          transform_update_.set(val);
        }
      });
    } else {
      transform_update_.on_change().connect([&](math::mat3 const& val){
        player_->Transform.set(val);
      });
    }

    distribute_member(&mover->LinearSpeed);
    distribute_member(&mover->AngularSpeed);
    distribute_member(&transform_update_);
  }

 ~Player() {
    auto scene = SceneManager::instance()->get_default();
    scene->remove(player_);
 }

 private:
  SceneObjectPtr player_;
  int            update_counter_;
  Mat3           transform_update_;
};

#endif  // PLAYER_HPP
